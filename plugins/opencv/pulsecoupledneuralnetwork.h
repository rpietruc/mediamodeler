#ifndef PULSECOUPLEDNEURALNETWORK
#define PULSECOUPLEDNEURALNETWORK

#include <opencv/cv.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
  * PCNN structure according to:
  * Li, H., Jin, X., Yang, N., Yang, Z., The recognition of landed aircrafts based on PCNN model and affine moment invariants, Pattern Recognition Letters (2014)
  * 
  * Feeding input:  \f$ F_{i,j}[n] = e^{-\alpha_F} F_{i,j}[n - 1] + V_F \sum_{k,l} m_{i, j, k, l} Y_{i,j}[n - 1] + S_{i,j} \f$.
  * Linking input:  \f$ L_{i,j}[n] = e^{-\alpha_L} L_{i,j}[n - 1] + V_L \sum_{k,l} w_{i, j, k, l} Y_{i,j}[n - 1] \f$.
  * Linking:        \f$ U_{i,j}[n] = F_{i,j}[n](1 + \beta L_{i,j}[n]) \f$.
  * Threshold:      \f$ T_{i,j}[n] = e^{-\alpha_T} T_{i,j}[n - 1] + V_T Y_{i,j}[n - 1] \f$.
  * Pulse:
  * \f[
  *     Y_{i,j}[n] = \left\{ \begin{array}{l l}
  *         1 & \quad U_{i,j}[n] \geq T_{i,j}[n] \\
  *         0 & \quad \text{otherwise}
  *     \end{array} \right.
  * \f]
  * 
  * @param grayImg input image in gray scale
  * @param output output image in gray scale
  * @param m feeding filter kernel
  * @param mSize feeding filter kernel size mSize*mSize
  * @param w linking filter kernel
  * @param wSize linking filter kernel size wSize*wSize
  * @param N iterations
  * @param alfa_F feeding attenuation
  * @param V_F feeding amplification
  * @param alfa_L linking attenuation
  * @param V_L linking amplification
  * @param beta linking coefficient
  * @param alfa_T threshold attenuation
  * @param V_T threshold amplification
  */
void pcnn(const IplImage* grayImg, IplImage* output, float *m, int mSize, float *w, int wSize, int N, double alfa_F, double V_F, double alfa_L, double V_L, double beta, double alfa_T, double V_T);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // PULSECOUPLEDNEURALNETWORK