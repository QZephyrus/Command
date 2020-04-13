#include "../lib/Compression_Trace_char.h"

string ptime_to_string(ptime t) {
  string time;
  time = to_iso_extended_string(t);
  time = time.replace(time.find("T"), 1, " ");
  return time;
}

double getDist(const point2 &A, const point2 &B) {
  double dist, x, y;
  x = A.X - B.X;
  y = A.Y - B.Y;
  dist = sqrt(x * x + y * y);
  return dist;
}

// C为中心点
double getAngle(const point2 &A, const point2 &B, const point2 &C) {
  double angle = atan2(A.X - C.X, A.Y - C.Y) - atan2(B.X - C.X, B.Y - C.Y);
  if (angle > M_PI)
    angle -= 2 * M_PI;
  if (angle < -M_PI)
    angle += 2 * M_PI;
  angle = abs(angle * 180.0 / M_PI);
  return angle;
}

// C到AB直线的距离
double getpoint2ToLineDist(const point2 &A, const point2 &B, const point2 &C) {
  double dist;
  if (A.X == B.X) {
    dist = fabs(A.Y - B.Y);
  } else {
    double k = -((A.Y - B.Y) / (A.X - B.X));
    double b = (A.Y * B.X - B.Y * A.X) / (B.X - A.X);
    dist = fabs(k * C.X + C.Y + b) / sqrt(1 + k * k);
  }
  return dist;
}

// 将AB直线分成row段，C到第num个点的距离
double getpoint2ToLinePDist(const point2 &A, const point2 &B, const point2 &C,
                            int row, int num) {
  point2 temp;
  temp.X = A.X + num * (B.X - A.X) / row;
  temp.Y = A.Y + num * (B.Y - A.Y) / row;
  double dist = getDist(temp, C);
  return dist;
}

double getVelocity(const point2 &A, const point2 &B) {
  double dist, V;
  dist = getDist(A, B);
  time_duration time;
  time = time_from_string(B.time) - time_from_string(A.time);
  double t = time.total_seconds();
  V = dist / t;
  return V;
}

point2 getSimulatpoint2(const point2 &A, const point2 &B, double time) {
  double X, Y, S, D, V;
  string Time;
  D = getDist(A, B);
  V = getVelocity(A, B);
  S = V * time;
  X = A.X + (S * (B.X - A.X)) / D;
  Y = A.Y + (S * (B.Y - A.Y)) / D;
  time_duration t = seconds(time);
  ptime ti = time_from_string(A.time);
  ti = ti + t;
  Time = ptime_to_string(ti);
  point2 retpoint2(A.PersonID, A.PersonModule, A.DeviceID, X, Y, A.Floor,
                   A.MapMark, Time);
  return retpoint2;
}

vector<point2> Compression_OW_Relat(const vector<point2> &trace,
                                    double distance, double time) {
  point2 pt1, pt2;
  int row = trace.size();
  int n = 0;
  time_duration t = seconds(time * 2);
  vector<point2> retrace;
  for (int i = 0; i < row; i++) {
    if (i == 0) {
      retrace.push_back(trace[i]);
      pt1 = trace[i];
      n = i;
    } else {
      pt2 = trace[i];
      if (time_from_string(trace[i].time) -
              time_from_string(trace[i - 1].time) <
          t) {
        if (strcmp(pt2.Floor, pt1.Floor) == 0 && pt2.MapMark == pt1.MapMark) {
          bool flag = true;
          for (int j = n + 1; j < i; j++) {
            double dist =
                getpoint2ToLinePDist(pt1, pt2, trace[j], i - n, j - n);
            if (dist > distance) {
              flag = false;
              break;
            }
          }
          if (flag == false) {
            retrace.push_back(trace[i - 1]);
            pt1 = trace[i - 1];
            n = i - 1;
          }
          if (i == row - 1) {
            retrace.push_back(trace[i]);
          }
        } else {
          pt1 = trace[i];
          if (n == i - 1) {
            retrace.push_back(pt1);
          } else {
            retrace.push_back(trace[i - 1]);
            retrace.push_back(trace[i]);
          }
          n = i;
        }
      } else {
        pt1 = trace[i];
        pt2 = trace[i];
        pt2.isCompress = false;
        if (n == i - 1) {
          retrace.push_back(pt2);
        } else {
          retrace.push_back(trace[i - 1]);
          retrace.push_back(pt2);
        }
        n = i;
      }
    }
  }
  return retrace;
}

int getpoint2Num(const point2 &A, const point2 &B, double time) {
  ptime t1 = time_from_string(A.time);
  ptime t2 = time_from_string(B.time);
  time_duration td = t2 - t1;
  int num = td.total_seconds() / time;
  return num;
}

vector<point2> Restore_OW(const vector<point2> &trace, double time) {
  vector<point2> retrace;
  int num;
  point2 pt1, pt2, pt3;
  for (unsigned int i = 0; i < trace.size(); i++) {
    if (i == 0) {
      pt1 = trace[i];
      retrace.push_back(pt1);
    } else {
      pt2 = trace[i];
      if (pt2.isCompress == true) {
        num = getpoint2Num(pt1, pt2, time);
        for (int j = 1; j < num; j++) {
          pt3 = getSimulatpoint2(pt1, pt2, time);
          retrace.push_back(pt3);
          pt1 = pt3;
        }
      }
      retrace.push_back(pt2);
      pt1 = pt2;
    }
  }
  return retrace;
}

void creatTraces(point2 &A, int num, double XF, double YF, double time,
                 vector<point2> &ret) {
  mt19937 gen;
  uniform_real_distribution<double> dist(-5.0, 5.0);
  variate_generator<mt19937 &, uniform_real_distribution<double>> die(gen,
                                                                      dist);
  time_duration t = seconds(time);
  if (ret.empty()) {
    ret.push_back(A);
  }
  for (int i = 0; i < num; i++) {
    A.X = A.X + XF + die();
    A.Y = A.Y + YF + die();
    strcpy(A.time, ptime_to_string(time_from_string(A.time) + t).c_str());
    // A.time = ptime_to_string(time_from_string(A.time) + t);
    ret.push_back(A);
  }
}
