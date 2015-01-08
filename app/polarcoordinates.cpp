#include "linecontour.h"
#include <iostream>
#include <boost/program_options.hpp>

using namespace cv;
using namespace std;
namespace po = boost::program_options;

static const int ShapeMatchingMethod = 0;

int main(int ac, char *av[])
    {
    int precision = -1; //unsigned by default

    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("precision,p", po::value<int>(&precision)->default_value(precision), "set result precision, negative for degrees")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);

    if (vm.count("help"))
        {
        cout << desc << "\n";
        return 1;
        }

    int lines;
    cin >> lines;
    cout << lines << endl;
    do
        {
        if (precision >= 0)
            {
            typedef float ValueType;
            typedef Point_<ValueType> PointType;

            PointType p1, p2;
            cin >> p1.x >> p1.y >> p2.x >> p2.y;

            vector<PointType> v;
            v.push_back(p1);
            v.push_back(p2);
            LineContour<ValueType, ShapeMatchingMethod> lineContour(v);

            cout.precision(precision);
            cout << fixed << ((LineNormalForm<ValueType>)lineContour).distanceFromOrigin()
                 << " "   << ((LineNormalForm<ValueType>)lineContour).angle() << endl;
            }
        else
            {
            typedef int ValueType;
            typedef Point_<ValueType> PointType;

            PointType p1, p2;
            cin >> p1.x >> p1.y >> p2.x >> p2.y;

            vector<PointType> v;
            v.push_back(p1);
            v.push_back(p2);
            LineContour<ValueType, ShapeMatchingMethod> lineContour(v);

            cout << fixed << ((LineNormalForm<ValueType>)lineContour).distanceFromOrigin()
                 << " "   << ((LineNormalForm<ValueType>)lineContour).angle()<< endl;
            }
        }
    while (--lines);
    return 0;
    }
