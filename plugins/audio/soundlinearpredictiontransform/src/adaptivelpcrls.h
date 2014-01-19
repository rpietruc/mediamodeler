/**
 * \file adaptivelpcrls.hpp
 * \class AdaptiveLpcRls
 * \brief Implementation of Recoursive Least Squares algorithm
 */

#ifndef ADAPTIVELPCRLS_HPP
#define ADAPTIVELPCRLS_HPP

#include "adaptivelpcbase.h"

#define DEF_LAMBDA        0.992f
#define BIG_NUM        1000.0f
#define TOOBIG_NUM   100000.0f

namespace acoustics
{
  class AdaptiveLpcRls
  :  public AdaptiveLpcBase
  {
  public:
    AdaptiveLpcRls(int taps, double weight = DEF_LAMBDA);
    virtual ~AdaptiveLpcRls(void);

    virtual double* update(const int16_t* x, int size, int16_t* d, int16_t* e);
    void prepareMatrix(void);

  private:
    double** R;
    double* xTR;
    double* Rx;
    double lambda;

  }; //AdaptiveLpcRls

} //acoustics

#endif //ADAPTIVELPCRLS_HPP
