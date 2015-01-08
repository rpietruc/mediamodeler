#include <iostream>
#include <boost/program_options.hpp>
#include "ellipsecontour.h"

using namespace cv;
using namespace std;
namespace po = boost::program_options;

static const int ShapeMatchingMethod = 0;

int main(int ac, char *av[])
    {
    int precision = -1; //degrees by default
    int compare = 0;

    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("precision,p", po::value<int>(&precision)->default_value(precision), "set result precision, negative for degrees")
        ("compare,c", po::value<int>(&compare)->default_value(compare), "set from 0 to 3, default 0")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);

    if (vm.count("help"))
        {
        cout << desc << "\n";
        return 1;
        }

    int pointsNo;
    cin >> pointsNo;
    cout << pointsNo << endl;

    if (precision >= 0)
        {
        typedef float ValueType;
        typedef Point_<ValueType> PointType;

        vector<PointType> points(pointsNo);
        for (int i = 0; i < (int)points.size(); ++i)
            {
            PointType point;
            cin >> point.x >> point.y;
            points[i] = point;
            }

        EllipseContour<ValueType, ShapeMatchingMethod> ellipse(points);
        LineContour<ValueType, ShapeMatchingMethod> lineSegment(points);
        vector<PointType> approx = (lineSegment < ellipse ? lineSegment.getApproxPoints() : ellipse.getApproxPoints());
        cout.precision(precision);
        for (int i = 0; i < (int)approx.size(); ++i)
            cout << fixed << approx[i].x << " " << approx[i].y << endl;
        }
    else
        {
        typedef int ValueType;
        typedef Point_<ValueType> PointType;

        vector<PointType> points(pointsNo);
        for (int i = 0; i < (int)points.size(); ++i)
            {
            PointType point;
            cin >> point.x >> point.y;
            points[i] = point;
            }

        EllipseContour<ValueType, ShapeMatchingMethod> ellipse(points);
        LineContour<ValueType, ShapeMatchingMethod> lineSegment(points);
        vector<PointType> approx = (lineSegment < ellipse ? lineSegment.getApproxPoints() : ellipse.getApproxPoints());

        for (int i = 0; i < (int)approx.size(); ++i)
            cout << fixed << approx[i].x << " " << approx[i].y << endl;
        }
    return 0;
    }
