/**
 * \file adaptivelpclms.hpp
 * \class AdaptiveLpcLms
 * \brief Implementation of Least Means Squares adaptive algorithm
 */

#ifndef ADAPTIVELPCLMS_HPP
#define ADAPTIVELPCLMS_HPP

#include "adaptivelpcbase.h"

#define DEFAULT_WEIGHT	0.1f

namespace acoustics
{
  class AdaptiveLpcLms
  :  public AdaptiveLpcBase
  {
  public:
    AdaptiveLpcLms(int taps, double weight = DEFAULT_WEIGHT);
    virtual ~AdaptiveLpcLms(void);

    virtual double* update(const int16_t* x, int size, int16_t* d, int16_t* e);

  private:
    double m_weight;

  }; //AdaptiveLpcLms

} //acoustics

#endif //ADAPTIVELPCLMS_HPP
