#define BOOST_TEST_MODULE EllipseMergeTest
#include <boost/test/unit_test.hpp>
#include <boost/assign/std/vector.hpp>
#include "ellipsecontour.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace cv;
using namespace std;
using namespace boost::assign;

typedef int ValueType;
Size_<ValueType> imgSize(100, 100);
static const int ShapeMatchingMethod = 0;

Ellipse<ValueType> ellipse1(RotatedRect(Point2f(20.2, 10.2), Size2f(20.2, 10.2), 30.2));
Ellipse<ValueType> ellipse2(RotatedRect(Point2f(20.5, 10.5), Size2f(20.5, 10.5), 30.5));

BOOST_AUTO_TEST_CASE(ellipseSizeSimilarTest)
    {
    BOOST_CHECK(isSimilar(1.8, 2.2, 0.5));
    float norm1 = cv::norm(cv::Point2f(ellipse1.getAxes().width, ellipse1.getAxes().height));
    float norm2 = cv::norm(cv::Point2f(ellipse2.getAxes().width, ellipse2.getAxes().height));
    BOOST_CHECK_CLOSE(norm1, norm2, 10);
    BOOST_CHECK(isSimilar(norm1, norm2, 0.1));
    }

BOOST_AUTO_TEST_CASE(ellipseCenterSimilarTest)
    {
    BOOST_CHECK(isSimilar(1.8, 2.2, 0.5));
    float norm1 = cv::norm(ellipse1.getCenter() - ellipse2.getCenter());
    float norm2 = cv::norm(cv::Point2f(imgSize.width, imgSize.height));
    BOOST_CHECK(norm1/norm2 < 0.1);
    }

BOOST_AUTO_TEST_CASE(ellipseSimilarTest)
    {
    BOOST_CHECK(ellipse1.hasSimilarSize(ellipse2, 0.1));
    BOOST_CHECK(ellipse1.hasSimilarCenter(ellipse2, 0.1, imgSize));
    BOOST_CHECK(ellipse1.hasSimilarAngle(ellipse2, 0.1));
    BOOST_CHECK(ellipse1.isSimilar(ellipse2, 0.1, imgSize));
    BOOST_CHECK(ellipse1 == ellipse2);
    }

template <typename _Tp>
void removeDuplicates(vector<_Tp> &aValues)
    {
    bool cont;
    do
        {
        cont = false;
        for (int i = 0; i < ((int)aValues.size() - 1); ++i)
            {
            for (int j = i + 1; j < ((int)aValues.size() - 1); ++j)
                if (aValues.at(i) == aValues.at(j))
                    {
                    aValues.erase(aValues.begin() + j);
                    cont = true;
                    break;
                    }
            if (cont)
                break;
            }
        }
    while (cont);
    }

BOOST_AUTO_TEST_CASE(removeDuplicatesTest)
    {
    vector<int> v;
    v += 2, 1, 2, 4, 1;
    removeDuplicates(v);
    BOOST_CHECK((v.size() > 0) && (v[0] == 2));
    BOOST_CHECK((v.size() > 1) && (v[1] == 1));
    BOOST_CHECK((v.size() > 2) && (v[2] == 4));
    }

template <typename _Tp>
void mergeCongruent(vector<_Tp> &aValues)
    {
    bool cont;
    do
        {
        cont = false;
        for (int i = 0; i < ((int)aValues.size() - 1); ++i)
            if (aValues.at(i) == aValues.at(i + 1))
                {
                aValues[i] = aValues.at(i) + aValues.at(i + 1);
                aValues.erase(aValues.begin() + i + 1);
                cont = true;
                }
        }
    while (cont);
    }

BOOST_AUTO_TEST_CASE(ellipseMergeTest)
    {
    vector<int> v;
    v += 1, 1, 2, 4, 8;
    mergeCongruent(v);
    BOOST_CHECK((v.size() == 1) && (v[0] == 16));
    }

BOOST_AUTO_TEST_CASE(ellipseThetaTest1)
    {
    LineNormalForm<float> pointLine(Point(0, 10), Point(10, 20));
    BOOST_CHECK_CLOSE(pointLine.distanceFromOrigin(), 5*sqrt(2), 1);
    BOOST_CHECK_CLOSE(pointLine.angle(), 135, 1);

    LineNormalForm<float> axisLine(Point(0, 10), Point(10, 0));
    BOOST_CHECK_CLOSE(axisLine.distanceFromOrigin(), 5*sqrt(2), 1);
    BOOST_CHECK_CLOSE(axisLine.angle(), 45, 1);

    BOOST_CHECK_CLOSE(pointLine.angleFromLine(axisLine), 90, 1);
    }

BOOST_AUTO_TEST_CASE(ellipseThetaTest2)
    {
    LineNormalForm<int> pointLine(Point(0, 10), Point(10, 20));
    BOOST_CHECK_CLOSE((float)pointLine.distanceFromOrigin(), trunc(5*sqrt(2)), 1);
    BOOST_CHECK_CLOSE((float)pointLine.angle(), 135, 1);

    LineNormalForm<int> axisLine(Point(0, 10), Point(10, 0));
    BOOST_CHECK_CLOSE((float)axisLine.distanceFromOrigin(), trunc(5*sqrt(2)), 1);
    BOOST_CHECK_CLOSE((float)axisLine.angle(), 45, 1);

    BOOST_CHECK_CLOSE((float)pointLine.angleFromLine(axisLine), 90, 1);
    }

BOOST_AUTO_TEST_CASE(fitEllipseTest)
    {
    vector<Point> v1;
    v1 += Point(0, 0), Point(0, 1), Point(1, 3), Point(2, 4), Point(3, 5), Point(4, 6), Point(5, 7), Point(7, 8),
          Point(8, 8), Point(8, 7), Point(7, 5), Point(6, 4), Point(5, 3), Point(4, 2), Point(3, 1), Point(1, 0);
    RotatedRect rect = fitEllipse(v1);
    BOOST_CHECK_CLOSE(rect.center.x, 4.0045, 1);
    BOOST_CHECK_CLOSE(rect.center.y, 4.0045, 1);
    BOOST_CHECK_CLOSE(rect.size.width, 2*1.5011, 1);
    BOOST_CHECK_CLOSE(rect.size.height, 2*5.6925, 1);
    BOOST_CHECK_CLOSE(rect.angle, RAD_TO_DEG*2.3562, 1);

    vector<Point2f> v2;
    v2 += Point2f(-2.8939, 4.1521),
          Point2f(-2.0614, 2.1684),
          Point2f(-0.1404, 1.9764),
          Point2f( 2.6772, 3.0323),
          Point2f( 5.1746, 5.7199),
          Point2f( 3.2535, 8.1196),
          Point2f(-0.1724, 6.8398);
    rect = fitEllipse(v2);
    BOOST_CHECK_CLOSE(rect.center.x, 1.2348, 1);
    BOOST_CHECK_CLOSE(rect.center.y, 4.9871, 1);
    BOOST_CHECK_CLOSE(rect.size.width, 2*2.3734, 1);
    BOOST_CHECK_CLOSE(rect.size.height, 2*4.6429, 1);
    BOOST_CHECK_CLOSE(rect.angle, RAD_TO_DEG*2.0849, 1);
    }

template <typename _Tp>
vector<Point_<_Tp> > readPointsFromFile(const char* aFilename)
    {
    vector<Point_<_Tp> > points;

    // Load table from file
    ifstream file(aFilename);
    int size;

    string line;
    getline(file, line);
    istringstream is(line);
    is >> size;
    points.resize(size);

    for (int i = 0; (i < (int)points.size()) && file; ++i)
        {
        getline(file, line);
        istringstream is(line);
        if (is) is >> points[i].x;
        if (is) is >> points[i].y;
        }
    return points;
    }

BOOST_AUTO_TEST_CASE(ellipseAproximateTest)
    {
    typedef int ValueType;
    typedef Point_<ValueType> PointType;

    vector<PointType> points; // = readPointsFromFile<ValueType>("ellipseapproximation_test.data");
    points += PointType(10, 10), PointType(10, 20), PointType(15, 30), PointType(25, 35), PointType(35, 35), PointType(45, 30);

    int algorithm = 1;
    EllipseContour<ValueType, ShapeMatchingMethod> ellipse(points, algorithm);
    LineContour<ValueType, ShapeMatchingMethod> lineSegment(points);
    vector<PointType> approx = (lineSegment < ellipse ? lineSegment.getApproxPoints() : ellipse.getApproxPoints());

    BOOST_CHECK(lineSegment > ellipse);
    BOOST_CHECK(approx.size() == points.size());

    for (int i = 0; (i < (int)points.size()) && (i < (int)approx.size()); ++i)
        {
        BOOST_CHECK_CLOSE((float)approx[i].x, (float)points[i].x, 15);
        BOOST_CHECK_CLOSE((float)approx[i].y, (float)points[i].y, 15);
        }

    BOOST_CHECK_CLOSE((float)((Ellipse<ValueType>)ellipse).getCenter().x, 30, 15);
    BOOST_CHECK_CLOSE((float)((Ellipse<ValueType>)ellipse).getCenter().y, 11, 15);
    BOOST_CHECK_CLOSE((float)((Ellipse<ValueType>)ellipse).getAxes().width, 21, 15);
    BOOST_CHECK_CLOSE((float)((Ellipse<ValueType>)ellipse).getAxes().height, 25, 15);
    BOOST_CHECK_CLOSE((float)((Ellipse<ValueType>)ellipse).getAngle(), -138, 15);
    BOOST_CHECK_CLOSE((float)ellipse.getThetas().arcFrom(), -39, 15);
    BOOST_CHECK_CLOSE((float)ellipse.getThetas().arcTo(), 193, 15);
    }

BOOST_AUTO_TEST_CASE(lineSegmentAproximateTest)
    {
    typedef float ValueType;
    typedef Point_<ValueType> PointType;

    vector<PointType> points; // = readPointsFromFile<ValueType>("lineapproximation_test.data");
    points += PointType(1, 1), PointType(1, 2), PointType(2, 2), PointType(2, 3), PointType(3, 3), PointType(3, 4);

    EllipseContour<ValueType, ShapeMatchingMethod> ellipse(points);
    LineContour<ValueType, ShapeMatchingMethod> lineSegment(points);
    vector<PointType> approx = (lineSegment < ellipse ? lineSegment.getApproxPoints() : ellipse.getApproxPoints());

    BOOST_CHECK((lineSegment < ellipse) || (!ellipse.getThetas().sorted()));
    BOOST_CHECK(approx.size() == points.size());

    for (int i = 0; (i < (int)points.size()) && (i < (int)approx.size()); ++i)
        {
        BOOST_CHECK_CLOSE(approx[i].x, points[i].x, 50);
        BOOST_CHECK_CLOSE(approx[i].y, points[i].y, 50);
        }

    LineNormalForm<ValueType> line(lineSegment);
    BOOST_CHECK_CLOSE(line.distanceFromOrigin(), -0.28, 5);
    BOOST_CHECK_CLOSE(line.angle(), 135, 10);
    }
