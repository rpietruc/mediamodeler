/**
 * \file filterconverter.h
 * \class CFilterSpectrum
 * \class CARSpectrum
 * \class CFilterConverter
 * \brief transversal to lattice filter conversion,
 *        lattice to resonanse cavities conversion,
 *        spectrum calculation.
 */

#ifndef _GUARD_FILTERCONVERTER_H
#define _GUARD_FILTERCONVERTER_H


/******************************************************************
    Includes
 ******************************************************************/

#include<vector>
// #define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <complex>
#include <algorithm>

/******************************************************************
    Definitions
 ******************************************************************/

#ifndef M_PI
#  define M_PI 3.141592654f
#endif // M_PI


/******************************************************************
 * Module namspace: begin
 ******************************************************************/

namespace acoustics
{

/******************************************************************
    Class declaration
 ******************************************************************/

template <class TVAL, class TVECT, class CVECT>
class CFilterSpectrum;

template <class TVAL, class TVECT>
class CARSpectrum;

template <class TVAL, class TVECT, class TMTX>
class CFilterConverter;


/******************************************************************
    Class definition
 ******************************************************************/

template <class TVAL,
          class TVECT = std::vector<TVAL>,
          class CVECT = std::vector< std::complex<TVAL> > >
class CFilterSpectrum
{

/******************************************************************
    Interface
 ******************************************************************/

public:

    /// C-tor
    CFilterSpectrum(unsigned int n, unsigned int m)
    : N(n), M(m) {
        if (N > 0 && M > 0) {
            b.resize(N, 0);
            a.resize(M, 0);
        }
    }

    /// C-tor
    CFilterSpectrum(const TVECT& rb, const TVECT& ra)
    :   N(rb.size()), M(ra.size()), b(rb), a(ra) {}

    /// Parametrized c-tor
    bool Parametrize(TVAL* pb, TVAL* pa, unsigned int n, unsigned int m) {
         if (N >= n && M >= m && pb && pa) {
            for (unsigned int i = 0; i < N; i++) b[i] = pb[i];
            for (unsigned int i = 0; i < M; i++) a[i] = pa[i];
        }
        return false;
    } // c-tor

    /// Empty d-ctor
    virtual ~CFilterSpectrum(void) {}

    /// Calculates spectrum from following model 
    //
    //             0       -jw             -jNw
    //        b[0]e + b[1]e   + ... + b[M]e 
    // H(e) = ----------------------------------
    //             0       -jw             -jMw
    //        a[0]e + a[1]e   + ... + a[M]e 
    //
    // where:
    //         -jnw
    //       e     = cos(wn) - jsin(wn)
    //
    inline bool CalculateSpectrum(unsigned int m) {
        TVAL step = M_PI / static_cast<TVAL>(m), rad = 0;
        spectrum.clear();
        for (unsigned int i = 0; i < m; i++, rad += step) {
            spectrum.push_back(ZTransform(b, rad)/ZTransform(a, rad));
        }
        return true;
    }

    /// Gets AR spectrum amplitudes
    inline unsigned int GetAmplitude(TVECT& rAbs) const {
        if (rAbs.size() < spectrum.size())
            rAbs.resize(spectrum.size());
        for (unsigned int n = 0; n < spectrum.size(); n++) {
            rAbs[n] = abs(spectrum[n]);
        }
        return spectrum.size();
    }

/******************************************************************
    Members
 ******************************************************************/

protected:

    inline std::complex<TVAL> ZTransform(TVECT& h, TVAL rad) {
        std::complex<TVAL> ret = 0;
        for (size_t n = 0; n < h.size(); n++) {
            ret += std::complex<TVAL>(h[n]*static_cast<TVAL>(cos(rad*n)),
                                    - h[n]*static_cast<TVAL>(sin(rad*n)));
        }
        return ret;
    } // ZTransform

    const unsigned int N;
    const unsigned int M;

    TVECT b;
    TVECT a;

    CVECT spectrum;
};


//******************************************************************
//  Auto regressive vocal tract model
//******************************************************************
//
//  x[n]                                         y[n]
//  ---->(+)----------- - - ->(+)--------------------->
//        ^                    ^               |
//        |                    |               |
//       /_\ a[p] = -h[p-1]   /_\ a[1] = -h[0] | a[0] = 1
//        |     ____           |     ____      |     
//         ----|  -1|<- - - --------|  -1|<----
//             |_z__|               |_z__|      
//
//
//
/******************************************************************
    Class definition
 ******************************************************************/

template <class TVAL,
          class TVECT = std::vector<TVAL> >
class CARSpectrum
:   public CFilterSpectrum<TVAL>
{

/******************************************************************
    Interface
 ******************************************************************/

public:

    /// Parametrized c-tor
    /// Auto-regressive model 
    //     jw       1                    1
    // H(e)  = ---------- = -----------------------------
    //                 jw            -jw             -jnw
    //         1 - A(e)     1 - h[0]e   -...- h[p-1]e
    //
    //                     b[0]
    //    = ------------------------------------
    //            0       -jw              -jnw
    //       a[0]e + a[1]e    + ... + a[M]e 
    //
    CARSpectrum(unsigned int n)
    :   CFilterSpectrum<TVAL>(1, n + 1), p(n) {
        this->b[0] = 1; this->a[0] = 1;
    }

    /// C-tor
    CARSpectrum(const TVECT& rh)
    :   CFilterSpectrum<TVAL>(TVECT(1), TVECT(1) - rh)
    ,   p(rh.size()) {}

    /// Empty d-ctor
    virtual ~CARSpectrum(void) {}

    /// Sets filter parameters
    // a[p][0] + a[p][1] + a[p][2] ... + a[p][p]
    //     = 1 - h[p][1] - h[p][2] ... - h[p][p] 
    virtual bool SetLPC(TVAL* h, unsigned int n) {
        if (h == NULL || n > p) return false;
        for (unsigned int i = 1; i <= p; i++)
            this->a[i] = -h[i - 1];
        return true;
    }

    inline unsigned int GetLPC(TVAL* h, unsigned int n) {
        unsigned int i = 0;
        if (h != NULL && n >= p) {
            for (i = 0; i < p; i++)
                h[i] = -this->a[i + 1];
        }
        return i;
    }

protected:


/******************************************************************
    Members
 ******************************************************************/

    const unsigned int p;


}; // CARSpectrum


//******************************************************************
//  Filter Coverter
//******************************************************************
//
//                  ----------------------
//      SetAreas -> | Resonanse cavities | -> GetAreas
//                  |        Areas       |
//                  ----------------------
//     ReflectionToAreas  /\     ||  AreasToReflection
//                        ||     \/
//                  ----------------------
// SetReflection -> | Reflection coeff.  | -> GetReflection
//                  | Tube model         |
//                  ----------------------
//    PARCORToReflection  /\     ||  ReflectionToPARCOR
//                        ||     \/
//                  ----------------------
//     SetPARCOR -> | Lattice filter     | -> GetPARCOR
//                  | PARCOR parameters  |
//                  ----------------------
//           LPCToPARCOR  /\     ||  PARCORToLPC
//                        ||     \/
//                  ----------------------
//        SetLPC -> | Transversal filter | -> GetLPC
//                  | LPC parameters     |
//                  ----------------------
//
/******************************************************************
    Class definition
 ******************************************************************/

template <class TVAL,
          class TVECT = std::vector<TVAL>,
          class TMTX  = std::vector< std::vector<TVAL> > >
class CFilterConverter
:   public CARSpectrum<TVAL>
{

/******************************************************************
    Interface
 ******************************************************************/

public:
    /// Empty c-tor
    CFilterConverter(unsigned int n)
    :   CARSpectrum<TVAL>(n) {
        Resize();
        A[this->p] = this->a;
    }

    CFilterConverter(const TVECT& rh)
    :   CARSpectrum<TVAL>(rh) {
        Resize();
        LPCToPARCOR();
        PARCORToReflection();
        ReflectionToAreas();
    }

    /// Empty d-ctor
    virtual ~CFilterConverter(void) {}

    /// Sets filter parameters
    // a[p][0] + a[p][1] + a[p][2] ... + a[p][p]
    //     = 1 - h[p][1] - h[p][2] ... - h[p][p] 
    virtual bool SetLPC(TVAL* h, unsigned int n) {
        if (h == NULL || n > this->p) return false;
        for (unsigned int i = 1; i <= this->p; i++)
            this->a[i] = -h[i - 1];
        return LPCToPARCOR()
            && PARCORToReflection()
            && ReflectionToAreas();
    }
    virtual bool SetLPC(const TVECT& h) {
        if (h.size() < this->p) return false;
        for (unsigned int i = 1; i <= this->p; i++)
            this->a[i] = -h[i - 1];
        return LPCToPARCOR()
            && PARCORToReflection()
            && ReflectionToAreas();
    }
    /// Sets lattice filter parameters
    inline bool SetPARCOR(TVAL* pP, unsigned int n) {
        if (pP == NULL || n > this->p) return false;
        for (unsigned int i = 1; i <= this->p; i++)
            this->P[i] = pP[i - 1];
        return PARCORToLPC()
            && PARCORToReflection()
            && ReflectionToAreas();
    }

   /// Sets reflection coefficients of resonanse cavities
   inline bool SetReflection(TVAL* pr, unsigned int n) {
        if (pr == NULL || n > this->p) return false;
        for (unsigned int i = 1; i <= this->p; i++)
            this->r[i] = pr[i - 1];
        return ReflectionToPARCOR()
            && PARCORToLPC()
            && ReflectionToAreas();
    }

    /// Sets resonanse cavities areas
    inline bool SetAreas(TVAL* pd, unsigned int n) {
        if (pd == NULL || n > this->p) return false;
        for (unsigned int i = 0; i < this->p; i++)
            s[i] = pd[i];
        return AreasToReflection()
            && ReflectionToPARCOR()
            && PARCORToLPC();
    }

    /// Gets lattice filter parameters
    inline unsigned int GetPARCOR(TVAL* pP, unsigned int m) const {
        unsigned int i = 0;
        if (pP && m >= this->p) {
            for (; i < this->p; i++)
                pP[i] = P[i + 1];
        }
        return i;
    }

    /// Gets reflection coefficients of resonanse cavities
    inline unsigned int GetReflection(TVAL* pr, unsigned int m) const {
        unsigned int i = 0;
        if (pr && m >= this->p) {
            for (; i < this->p; i++)
                pr[i] = r[i + 1];
        }
        return i;
    }

    /**
     * Gets resonanse cavities areas
     * @param SL lips area
     */
    inline unsigned int GetAreas(TVAL* pd, unsigned int m) const {
        unsigned int i = 0;
        if (pd && m >= this->p) {
            for (; i < this->p; i++)
                pd[i] = s[i];
        }
        return i;
    }

#ifndef WIN32
    friend void PRINTF(CFilterConverter<float> *pConv);
#endif // WIN32

/******************************************************************
    Members
 ******************************************************************/

protected:

    /// Converts transversal filter parameters to lattice
    inline bool LPCToPARCOR(void) {
        A[this->p] = this->a;
        for (int i = this->p; i >= 1; i--) {
            P[i] = -A[i][i];
            for (int j = 1; j <= i - 1; j++) {
                A[i - 1][j] = (A[i][j] + P[i]*A[i][i - j])
                            / (1 - P[i]*P[i]);
            }
        }
        return true;
    }

    /// Converts lattice parameters to transversal filter
    inline bool PARCORToLPC(void)
    {
        for (unsigned int i = 1; i <= this->p; i++) {
            A[i][i] = -P[i];
            for (unsigned int j = 1; j <= i - 1; j++) {
                A[i][j] = A[i - 1][j] - P[i]*A[i - 1][i - j];
            }
        }
        this->a = A[this->p];
        return true;
    }

    /// Converts reflection coefficients to resonanse cavities Areas
    /*
     *  glottis,  1st cavity,   ... , lips,  space
     *  s[p] = 1, s[p-1],       ... , s[1],  s[0]
     *         \   /       \   /         \   /
     *          r[p]      r[p-1]         r[1]
     *         /   \       /   \         /   \
     *           1 - r[i]
     *  s[i-1] = -------- s[i]
     *           1 + r[i]
     */
    inline bool ReflectionToAreas(void) {
        s[this->p] = 1; // at glottis
        for (unsigned int i = this->p; i > 0; i--) {
            s[i-1] = s[i]*(1 - r[i])
                         /(1 + r[i]);
        }
        return true;
    }

    /// Converts resonanse cavities areas to reflection coefficients
    /*
     *  glottis,  1st cavity,   ... , lips,  space
     *  s[p] = 1, s[p-1],       ... , s[1],  s[0]
     *         \   /       \   /         \   /
     *          r[p]      r[p-1]         r[1]
     *         /   \       /   \         /   \
     *          z[i-1] - z[i] |     1 /   |   s[i] - s[i-1]
     *  r[i] =  ------------- | z ~  /    | = -------------
     *          z[i-1] + z[i] |     / s   |   s[i] + s[i-1]
     */
    inline bool AreasToReflection(void) {
        s[this->p] = 1; // at glottis
        for (unsigned int i = this->p; i > 0; i--) {
            r[i] = (s[i] - s[i-1])
                 / (s[i] + s[i-1]);
        }
        return true;
    }

    /// Converts reflection coefficients to lattice parameters
    inline bool ReflectionToPARCOR(void) {
        for (unsigned int i = 1; i <= this->p; i++) {
            P[i] = -r[i];
        }
        return true;
    }

    /// Converts lattice parameters to reflection coefficients
    inline bool PARCORToReflection(void) {
        for (unsigned int i = 1; i <= this->p; i++) {
            r[i] = -P[i];
        }
        return true;
    }

private:

    inline void Resize(void) {
        r.resize(this->M, 0);
        P.resize(this->M, 0);
        s.resize(this->M, 0);
        A.resize(this->M, TVECT(this->M, 0));
    } // Resize

    TMTX  A;
    TVECT P;
    TVECT r;
    TVECT s;
};

/***************************************************************************
 * Module namspace: end
 **************************************************************************/
}

#endif // _GUARD_FILTERCONVERTER_H


