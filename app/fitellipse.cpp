#include <iostream>
#include "ellipsefitting.h"
#include <boost/chrono.hpp>

using namespace cv;
using namespace std;
namespace bc = boost::chrono;

typedef float _Tp;

int main(int /*ac*/, char */*av*/[])
    {
    int pointsNo;
    cin >> pointsNo;

    if (pointsNo >= 5)
        {
        vector<Point_<_Tp> > points;
        points.resize(pointsNo);
        for (int i = 0; i < (int)points.size(); ++i)
            cin >> points.at(i).x >> points.at(i).y;

        bc::system_clock::time_point start = bc::system_clock::now();
        RotatedRect rect = fitEllipse(points);
        bc::duration<double> sec = bc::system_clock::now() - start;
        cout << rect.center.x << endl;
        cout << rect.center.y << endl;
        cout << rect.size.width/2 << endl;
        cout << rect.size.height/2 << endl;
        cout << arma::datum::pi*rect.angle/180.0 << endl;
        cout << "took " << sec.count() << " seconds\n";
        }
    return 0;
    }
