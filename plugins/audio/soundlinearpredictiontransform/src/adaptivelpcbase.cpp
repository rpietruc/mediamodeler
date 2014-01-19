/**
 * \file adaptivelpcbase.cpp
 */

#include "adaptivelpcbase.h"
#include <string.h>
#include <math.h>

using namespace acoustics;

AdaptiveLpcBase::AdaptiveLpcBase(int taps)
: N(taps),
  xn(0),
  h(0),
  wsk_xn(0)
{
  xn = new int16_t[N];
  if (xn)
    memset(xn, 0, N*sizeof(int16_t));
  h = new double[N];
  if (h)
    memset(h, 0, N*sizeof(double));
} //AdaptiveLpcBase

AdaptiveLpcBase::~AdaptiveLpcBase(void)
{
  delete [] xn;
  delete [] h;
} //~

int16_t AdaptiveLpcBase::meanSquare(const int16_t* x, int size)
{
  double ret = 0;
  if (size > 0)
    ret = x[0]*x[0];
  for (int n = 0; n < size; n++)
    ret += x[n]*x[n];
  if (ret > 0)
    ret = sqrt(ret/size);
  return static_cast<int16_t>(ret);
} //meanSquare

