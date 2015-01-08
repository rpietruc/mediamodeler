#ifndef MULTIPLECHANNELPCNN
#define MULTIPLECHANNELPCNN

#include <opencv/cv.h>
#include <vector>

namespace mpcnn
{
/**
  * Literature:
  * 
  * Hualiang Zhuang and Kay-Soon Low and Wei-Yun Yau,
  * Multichannel Pulse-Coupled-Neural-Network-Based Color Image Segmentation for Object Detection
  * IEEE Transactions on Industrial Electronics, 59(8), 2012, pp. 3299-3308
  *
  * @todo to be defined
  */

/**
  * getSeededPixels
  *
  * @brief  M-PCNN initialization function, finds all pixels for which:
  *
  * \f$ \mu_s < \theta_{\mu} \f$.
  * where:
  * \f$ \mu_s = \smash{\displaystyle\max_{j = 1, ..., 8}} \{ \| \mathbf{x}^s - \mathbf{c}^j \| \} \f$
  * and:
  * \f$ \| \mathbf{x} - \mathbf{c}^j \| = \sum_{i = 1}^{N_i} | x_i - c_i^j | \f$.
  *
  * @param rgbImg input image
  * @param theta_mu threshold value \f$ \theta_{\mu} \f$.
  *
  * @return binary seeded pixels image
  */
cv::Mat getSeededPixels(const cv::Mat& rgbImg, double theta_mu);

/**
  * sumChannels [helper function]
  *
  * @brief sums all image channels to single channel image
  *
  * @param mat image of \f$ x_i \f$ pixels
  *
  * @return image for which every pixel is \f$ \sum_{i = 1}^{N_i} x_i \f$.
  */
cv::Mat sumChannels(const cv::Mat& mat);

/**
  * getNeighbours [helper function]
  *
  * @brief creates 8 neighbour images for subtraction, where in j-th image i-th element is j-th neighbour of i-th input pixel
  *
  * @param rgbImg input image
  * @param neighbours output 8 images
  *
  */
void getAllUnitShifts(const cv::Mat& rgbImg, std::vector<cv::Mat> &neighbours);

/**
  * getDiffs [helper function]
  *
  * @brief for every neighbour image calculates absolute difference with input image
  *
  * @param rgbImg input image
  * @param neighbours neighbours images
  * @param absdiffs [output] images of absolute difference between input and each neighbour images 
  *
  */
void getAbsDiffs(const cv::Mat& rgbImg, const std::vector<cv::Mat> &neighbours, std::vector<cv::Mat> &absdiffs);

/**
  * getMaxChannelsSum [helper function]
  *
  * @brief for every pixel retrieves maximum sum of channels in every input image
  *
  * \f$ \smash{\displaystyle\max_{j = 1, ..., 8}} \{ \mathbf{c}^j \} \f$
  *
  * @param absdiffs input images
  *
  * @return image of maximum channels sum
  */
cv::Mat getMaxChannelsSum(const std::vector<cv::Mat> &absdiffs);

/**
  * getRBFOutput
  *
  * @brief implements pulse-based radial basis function (RBF)
  *
  * \f$ s_j \f$ integrated potentials of output neurons (single-channel images for every neighbour)
  * \f$ PSP_i^j \f$ postsynaptic potential
  *
  * \f[
  *     psp_i^j(t) = \left\{ \begin{array}{l l}
  *         t - \| x_i - c_i^j \| \text{,} & \quad \text{if} t \geq \| x_i - c_i^j \| \\
  *         0 \text{,} & \quad \text{otherwise}
  *     \end{array} \right.
  * \f]
  *
  * @param rgbImage input image
  * @param t time
  * @param theta threshold
  *
  * @return 8 output binary images of output pulses for every neighbour at time t 
  */
std::vector<cv::Mat> getRBFOutput(const cv::Mat& rgbImg, int t, double theta);

} // namespace mpcnn

#endif // MULTIPLECHANNELPCNN
