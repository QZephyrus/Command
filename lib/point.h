#pragma once

#include <cstring>
#include <string>
using namespace std;
using std::string;

class point {
public:
  int PersonID;
  short PersonModule;
  string DeviceID;
  double X;
  double Y;
  string Floor;
  int MapMark;
  string time;
  bool isCompress = true;
  point(int PerID, short PerModule, string DevID, double XF, double YF,
        string FloorF, int MMark, string t)
      : PersonID(PerID), PersonModule(PerModule), DeviceID(DevID), X(XF), Y(YF),
        Floor(FloorF), MapMark(MMark), time(t){};
  point(void){};
  void getPoint(double x, double y, string Time) {
    X = x;
    Y = y;
    time = Time;
  };
  void getPoint(int PerID, short PerModule, string DevID, double x, double y,
                string FloorF, int MMark, string t) {
    PersonID = PerID;
    PersonModule = PerModule;
    DeviceID = DevID;
    MapMark = MMark;
    X = x;
    Y = y;
    time = t;
    Floor = FloorF;
  };
};

class point2 {
public:
  int PersonID;
  short PersonModule;
  char DeviceID[20];
  double X;
  double Y;
  char Floor[5];
  int MapMark;
  char time[20];
  bool isCompress;
  point2(int PerID, short PerModule, string DevID, double XF, double YF,
         string FloorF, int MMark, string t)
      : PersonID(PerID), PersonModule(PerModule), X(XF), Y(YF), MapMark(MMark),
        isCompress(true) {
    strcpy(DeviceID, DevID.c_str());
    strcpy(time, t.c_str());
    strcpy(Floor, FloorF.c_str());
  };
  point2(void) : isCompress(true){};
  void getPoint(double x, double y, string Time) {
    X = x;
    Y = y;
    strcpy(time, Time.c_str());
    // time = Time.c_str();
  };
  void getPoint(int PerID, short PerModule, string DevID, double x, double y,
                string FloorF, int MMark, string t) {
    PersonID = PerID;
    PersonModule = PerModule;
    strcpy(DeviceID, DevID.c_str());
    MapMark = MMark;
    X = x;
    Y = y;
    strcpy(time, t.c_str());
    strcpy(Floor, FloorF.c_str());
    isCompress = true;
  };
  bool same(const point2 &p) {
    return (PersonID == p.PersonID && PersonModule == p.PersonModule &&
            strcmp(time, p.time) == 0);
  };
};