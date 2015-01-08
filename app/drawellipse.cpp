 
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
int startAngle;
int endAngle;
RNG rng(12345);

void anglecallback(int, void*)
{
  Mat drawing = Mat::zeros(Size(800, 600), CV_8UC3);
  Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
  ellipse(drawing, Point(400, 300), Size(250, 200), 45, startAngle, endAngle, color);
  imshow("Ellipse", drawing);
}

int main( int argc, char** argv )
{
  namedWindow("Ellipse", CV_WINDOW_AUTOSIZE);

  createTrackbar("startAngle:", "Ellipse", &startAngle, 720, anglecallback );
  createTrackbar("endAngle:", "Ellipse", &endAngle, 720, anglecallback );
  anglecallback(0, 0);

  waitKey(0);
  return(0);
}
