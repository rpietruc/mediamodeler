/**
 * \file adaptivelpcbase.hpp
 * \class AdaptiveLpcBase
 * \brief Abstract class for adaptive algorithms
 */

#ifndef ADAPTIVELPCBASE_HPP
#define ADAPTIVELPCBASE_HPP

#include <stdint.h>

namespace acoustics
{
  class AdaptiveLpcBase
  {
  public:
    AdaptiveLpcBase(int taps);
    virtual ~AdaptiveLpcBase(void);

    /** update
     * virtual function that updates filter parameters from data
     * @param x        signal sample buffer
     * @param size     input buffer size
     */
    virtual double* update(const int16_t* x, int size, int16_t* d = 0, int16_t* e = 0) = 0;
    static int16_t meanSquare(const int16_t* x, int size);

  protected:
    const int N;         // taps
    int16_t* xn;        // input buffer
    double* h;          // filter parameters
    double* y;          // filter parameters
    double* e;          // filter parameters
    unsigned int wsk_xn; // actual sample pointer

  }; //AdaptiveLpcBase

} //acoustics

#endif //ADAPTIVELPCBASE_HPP

