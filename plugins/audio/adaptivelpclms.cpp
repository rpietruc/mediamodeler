/**
 * \file adaptivelpclms.cpp
 */

#include "adaptivelpclms.h"
#include <math.h>

using namespace acoustics;

AdaptiveLpcLms::AdaptiveLpcLms(int taps, double weight) :
    AdaptiveLpcBase(taps),
    m_weight(weight)
    {
    }

AdaptiveLpcLms::~AdaptiveLpcLms()
    {
    }

double* AdaptiveLpcLms::update(const int16_t* x, int size, int16_t* d, int16_t* e)
    {
    for (int n = 0; n < size; n++)
        {
        int16_t yn = 0;
        int i;
        for (i = 0; i < N; i++)
            yn += h[i]*xn[(wsk_xn+i)%N];           // yn = h * xn';
        int16_t en = x[n] - yn;                    // en = d(n) - yn;
        if (d) d[n] = yn;
        if (e) e[n] = en;
        for (i = 0; i < N; i++)
            h[i] += 2*m_weight*en*xn[(wsk_xn+i)%N]; // h = h + 2*m_weight*en*xn;
        wsk_xn = (wsk_xn+N-1)%N;                    // xn = [ x(n) xn(1:M-1) ];
        xn[wsk_xn] = x[n];
        }
    return h;
    }
