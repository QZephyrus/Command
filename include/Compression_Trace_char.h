#pragma once
#include "point.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/random.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
using std::string;
using std::vector;
using namespace boost::random;
using namespace boost::posix_time;
using namespace boost::gregorian;

class CountData {
public:
  int OrgN;
  int ComN;
  int ResN;
  double CR;
  double SP;
  double ReSR;
  double CS;
  time_duration ComTime;
  time_duration ReSTime;
  double AverDist;
  double Variance;
  CountData(double C, double S, double R, double A, double V)
      : CR(C), SP(S), ReSR(R), AverDist(A), Variance(V){};
  CountData()
      : OrgN(0), ComN(0), ResN(0), CR(0), SP(0), ReSR(0), AverDist(0),
        Variance(0){};
};

string ptime_to_string(ptime t);

double getDist(const point2 &A, const point2 &B);
double getAngle(const point2 &A, const point2 &B, const point2 &C);
double getPointToLineDist(const point2 &A, const point2 &B, const point2 &C);
double getVelocity(const point2 &A, const point2 &B);
point2 getSimulatPoint(const point2 &A, const point2 &B, double time);

vector<point2> Compression_OW_Relat(const vector<point2> &trace,
                                    double distance, double time);

int getPointNum(const point2 &A, const point2 &B);

vector<point2> Restore_OW(const vector<point2> &trace, double time);

void creatTraces(point2 &A, int num, double XF, double YF, double time,
                 vector<point2> &ret);
