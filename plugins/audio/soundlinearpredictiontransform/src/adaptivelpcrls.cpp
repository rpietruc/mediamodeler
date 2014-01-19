/**
 * \file adaptivelpcrls.cpp
 */

#include "adaptivelpcrls.h"
#include <string.h>

using namespace acoustics;

AdaptiveLpcRls::AdaptiveLpcRls(int taps, double weight) :
    AdaptiveLpcBase(taps),
    lambda(weight)
    {
    Rx = new double[N];
    if (Rx)
        {
        memset(Rx, 0, N*sizeof(double));
        xTR = new double[N];
        if (xTR)
            {
            memset(xTR, 0, N*sizeof(double));
            R = new double*[N];
            if (R)
                {
                for (int i = 0; i < N; i++)
                if ((R[i] = new double[N]) == NULL)
                    return;
                prepareMatrix();
                }
            }
        }
    }

AdaptiveLpcRls::~AdaptiveLpcRls(void)
    {
    delete [] Rx;
    delete [] xTR;
    if (R)
        {
        for (int i = 0; i < N; i++)
            delete [] R[i];
        delete [] R;
        }
    }

void AdaptiveLpcRls::prepareMatrix(void)
    {
    for (int i = 0; i < N; i++)
        {
        memset(R[i], 0, N*sizeof(double));
        R[i][i] = BIG_NUM;
        }
    }

double* AdaptiveLpcRls::update(const int16_t* x, int size, int16_t* d, int16_t* e)
    {
    if (R[0][0] > TOOBIG_NUM)
        prepareMatrix();

    for (int n = 0; n < size; n++)
        {
        double yn = 0;                    // yn = h * xn';
        for (int i = 0; i < N; i++)
            yn += h[i]*xn[(wsk_xn+i)%N];
        double en = x[n] - yn;            // en = x(n) - yn;
        if (d) d[n] = yn;
        if (e) e[n] = en;

        memset(xTR, 0, sizeof(double)*N);
        for (int i = 0; i < N; i++)
            {
            double *Rp = R[i];
            for (int j = 0; j < N; j++)      // xTR = x'*R
                xTR[j] += Rp[j]*xn[(wsk_xn+i)%N];
            }
        double alfa = lambda;               // alfa = lambda + x'*R*x
        for (int i = 0; i < N; i++)
            alfa = alfa+xTR[i]*xn[(wsk_xn+i)%N];
        alfa = 1.0f/alfa;
        for (int i = 0; i < N; i++)          // Rx = R*x
            {
            Rx[i] = 0;
            double *Rp = R[i];
            for (int j = 0; j < N; j++)
                Rx[i] += alfa*Rp[j]*xn[(wsk_xn+j)%N];
            }
        for (int i = 0; i < N; i++)          // h = h + en*R*x/mianownik
            h[i] = h[i] + en*Rx[i];
        for (int i = 0; i < N; i++)          // R = (R-R*x*x'*R/mianownik)/lambda
            {
            double *Rp = R[i];
            for (int j = 0; j < N; j++)
                Rp[j] = (1.0f/lambda)*(Rp[j] - Rx[i]*xTR[j] );
            }
        wsk_xn = (wsk_xn+N-1)%N;
        xn[wsk_xn] = x[n];
        }
    return h;
    }
