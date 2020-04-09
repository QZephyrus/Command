#include "../lib/Compression_Trace.h"
#include "../lib/DataBase.h"
#include <fstream>
#include <iostream>
#include <unordered_map>

using namespace std;

int main() {

  string host = "localhost";
  string user = "root";
  string passwd = "zenos";
  string DBName = "traces";
  string TableName = "Trace202004";
  double dist = 10;
  double time = 2;
  DataBase DB;
  if (DB.connect(host, user, passwd, DBName) != true) {
    cout << "connect Error" << endl;
    return 0;
  }
  if (DB.useDB(DBName) != true) {
    cout << "use Database Error" << endl;
    return 0;
  }
  vector<point> rec = DB.selectTrace(TableName);
  if (rec.empty()) {
    cout << "traces empty or table name error" << endl;
    return 0;
  }
  cout << rec.size() << endl;

  unordered_map<int, vector<point>> trace_map;
  unordered_map<int, vector<point>>::iterator getmap;
  for (auto &v : rec) {
    getmap = trace_map.find(v.PersonID * 10 + v.PersonModule);
    if (getmap == trace_map.end()) {
      vector<point> temp;
      temp.push_back(v);
      trace_map.insert(
          pair<int, vector<point>>((v.PersonID * 10 + v.PersonModule), temp));
    } else {
      getmap->second.push_back(v);
    }
  }

  unordered_map<int, vector<point>> comtrace_map;
  unordered_map<int, vector<point>> restrace_map;
  for (auto v : trace_map) {
    vector<point> comtemp;
    comtemp = Compression_OW_Relat(v.second, dist, time);
    comtrace_map.insert(pair<int, vector<point>>(v.first, comtemp));
  }

  // fstream file;
  // file.open("Compression_data.dat", ios::out | ios::binary);
  // if (!file) {
  //  cout << "error";
  //  return 0;
  //}
  /*
  for (auto v : comtrace_map) {
    bool first = true;
    for (auto m : v.second) {
      if (first) {
        //file.write(reinterpret_cast<char *>(&m.PersonID), sizeof(m.PersonID));
        file << "{" << m.PersonID << "," << m.PersonModule << "," <<
           m.DeviceID
             << ",{" << m.X << "," << m.Y << "," << m.Floor << "," << m.MapMark
             << "," << m.time << "}";
        first = false;
      } else {
        file << ",{" << m.X << "," << m.Y << "," << m.Floor << "," <<
           m.MapMark
             << "," << m.time << "}";
      }
    }
    // file << "}\n";
  }
  */
  /*
    fstream file;
    file.open("test.dat", ios::out | ios::binary);
    point2 a(101, 1, "D101", 100, 100, "1F", 1, "2020-02-09 14:08:00");
    // double t = 100;
    file.write((char *)&a, sizeof(a));
    file.write((char *)&a, sizeof(a));
    file.write((char *)&a, sizeof(a));
    file.write((char *)&a, sizeof(a));
    file.close();
  */
  // file.open("test.dat", ios::in);
  // double temp;
  fstream infile;
  infile.open("test.dat", ios::in | ios::binary);
  point2 c;
  try {
    while (!infile.eof()) {
      infile.read((char *)&c, sizeof(c));
      cout << c.PersonID << "," << c.PersonModule << "," << c.DeviceID << ","
           << c.X << "," << c.Y << "," << c.Floor << "," << c.MapMark << ","
           << c.time << endl;
    }
  } catch (char *s) {
  }

  infile.close();
  // infile.read(reinterpret_cast<char *>(&temp), sizeof(temp));
  // cout << temp << endl;
  /*
    for (auto v : comtrace_map) {
      vector<point> restemp;
      restemp = Restore_OW(v.second, time);
      restrace_map.insert(pair<int, vector<point>>(v.first, restemp));
    }
  */
  /*
  vector<point> comtrace = Compression_OW_Relat(rec, 2, 2);
  cout << comtrace.size() << endl;
  for (auto &v : comtrace) {
    cout << v.PersonID << "," << v.PersonModule << "," << v.DeviceID << ","
         << v.X << "," << v.Y << "," << v.Floor << "," << v.MapMark << ","
         << v.time << endl;
  }
  vector<point> restrace = Restore_OW(comtrace, 2);
  cout << restrace.size() << endl;
  for (auto &v : restrace) {
    cout << v.PersonID << "," << v.PersonModule << "," << v.DeviceID << ","
         << v.X << "," << v.Y << "," << v.Floor << "," << v.MapMark << ","
         << v.time << endl;
  }
  */
  return 1;
}