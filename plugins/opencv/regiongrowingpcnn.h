#ifndef REGIONGROWINGPCNN
#define REGIONGROWINGPCNN

#include <opencv/cv.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
  * Literature:
  * Robert D. Stewart, Iris Fermin, and Manfred Opper
  * Region Growing With Pulse-Coupled Neural Networks: An Alternative to Seeded Region Growing
  * IEEE TRANSACTIONS ON NEURAL NETWORKS, VOL. 13, NO. 6, NOVEMBER 2002
  *
  *
  * @todo Merge small regions with nearest neighbor.
  */
void regionGrowingPcnn(const IplImage* grayImg, IplImage* output, double omega, double beta_min, double beta_max, double beta_delta, double d, double SBmax);

/**
  * Working threshold
  * \f$ wt_t = max[G_y] \quad \farall y | P_y[t - 1] = 0 \f$.
  * @todo optimize mask array allocation
  */
double workingThreshold(const IplImage* G, const IplImage* P);

/**
  * Linking:
  * \f$ L_x[t] = \sum_{z \in N(x)} Y_z[t] - d \f$.
  */
void linking(const IplImage* Y, IplImage* L, double d);

/** 
  * Feeding:
  * \f$ U_x[t] = G_x\{1 + \beta_t L_x[t]\} \f$.
  */
void feeding(const IplImage* L, const IplImage* G, IplImage* U, double beta_t);

/**
  * Threshold
  * \f[
  *     T_x[t] = \left\{ \begin{array}{l l}
  *         w t_t \text{,} & \quad \text{if $P_x[t - 1] = 0$} \\
  *         \Omega \text{,} & \quad \text{otherwise}
  *     \end{array} \right.
  * \f]
  */
void threshold(const IplImage* P, double omega, double wt_t, IplImage* T);

/**
  * Pulse output
  * \f[
  *     Y_x[t] = \left\{ \begin{array}{l l}
  *         1 \text{,} & \quad \text{if $U_x[t] \geq T_x[t]$} \\
  *         0 \text{,} & \quad \text{otherwise}
  *     \end{array} \right.
  * \f]
  * 
  * @return true if there is any change in p ulsing activity
  * 
  * @todo optimize temporary array allocation
  */
void pulseOutput(const IplImage* U, const IplImage* T, IplImage* Y);

/**
  * pulseOutputAndCheckIfThereIsAnyChangeInPulsingActivity
  * @return true if there is any change in pulsing activity
  * @todo optimize temporary array allocation
  */
int pulseOutputAndCheckIfThereIsAnyChangeInPulsingActivity(const IplImage* U, const IplImage* T, IplImage* Y);

/**
  * Pulse matrix
  *  \f[
  *     P_x[t] = \left\{ \begin{array}{l l}
  *         t \text{,} & \quad \text{if $Y_x[t] = 1$} \\
  *         P_x[t - 1] \text{,} & \quad \text{otherwise}
  *     \end{array} \right.
  *  \f]
  *
  * @todo optimize temporary array allocation
  */
void pulseMatrix(const IplImage* Y, IplImage* P, int t);

int allNeuronsHavePulsed(const IplImage* P);

/**
  * Excessive beta value
  * 
  * the linking coefficient exceeds a maximum value \f$ \beta_{max} \f$.
  */
int excessiveBetaValue(double beta_t, double beta_max);

/**
  * Region engulfed
  * 
  * all neighbors of pulsing neurons are either pulsing or not active.
  */
int regionEngulfed(const IplImage* Y, const IplImage* P);

/**
  * Excessive mean difference
  * 
  * difference \f$ \delta \mu \f$ between mean of accepted region \f$ \mu_{old} \f$
  * and mean of newly proposed region \f$ \mu_{new} \f$ is greater than a threshold value \f$ S_{B_{max}} \f$.
  * 
  * @todo implement
  */
int excessiveMeanDifference(const IplImage* G, IplImage* Y, const IplImage* Yold, double SBmax);

/**
  * statisticalTerminationConditionMet
  *
  * -# Region engulfed
  * -# Excessive beta value
  * -# Excessive mean difference
  * 
  * @todo add 2 missing conditions
  */
int statisticalTerminationConditionMet(const IplImage* G, IplImage* Y, const IplImage* Yold, const IplImage* P, double beta_t, double beta_max, double SBmax);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // REGIONGROWINGPCNN