#include "../lib/Compression_Trace_char.h"
#include "../lib/DataBase.h"
#include <fstream>
#include <iostream>
#include <unordered_map>

using namespace std;

vector<point2> readTraceFromMysql(string host, string user, string passwd,
                                  string DBName, string TableName) {
  DataBase DB;
  if (DB.connect(host, user, passwd, DBName) != true) {
    cout << "connect Error" << endl;
    return {};
  }
  if (DB.useDB(DBName) != true) {
    cout << "use Database Error" << endl;
    return {};
  }
  vector<point2> rec = DB.selectTrace2(TableName);
  return rec;
}
vector<point2> Compression(vector<point2> org_trace, double dist, double time) {

}

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
  vector<point2> rec = DB.selectTrace2(TableName);
  if (rec.empty()) {
    cout << "traces empty or table name error" << endl;
    return 0;
  }
  cout << rec.size() << endl;

  unordered_map<int, vector<point2>> trace_map;
  unordered_map<int, vector<point2>>::iterator getmap;
  for (auto &v : rec) {
    getmap = trace_map.find(v.PersonID * 10 + v.PersonModule);
    if (getmap == trace_map.end()) {
      vector<point2> temp;
      temp.push_back(v);
      trace_map.insert(
          pair<int, vector<point2>>((v.PersonID * 10 + v.PersonModule), temp));
    } else {
      getmap->second.push_back(v);
    }
  }

  unordered_map<int, vector<point2>> comtrace_map;
  unordered_map<int, vector<point2>> restrace_map;
  for (auto v : trace_map) {
    vector<point2> comtemp;
    comtemp = Compression_OW_Relat(v.second, dist, time);
    comtrace_map.insert(pair<int, vector<point2>>(v.first, comtemp));
  }

  for (auto v : comtrace_map) {
    vector<point2> restemp;
    restemp = Restore_OW(v.second, time);
    restrace_map.insert(pair<int, vector<point2>>(v.first, restemp));
  }

  fstream file;
  file.open("test.dat", ios::out | ios::binary);
  point2 a(101, 1, "D101", 100, 100, "1F", 1, "2020-02-09 14:08:00");
  file.write((char *)&a, sizeof(a));
  file.write((char *)&a, sizeof(a));
  file.write((char *)&a, sizeof(a));
  file.write((char *)&a, sizeof(a));
  file.close();

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

  return 1;
}