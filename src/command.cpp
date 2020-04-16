#include "../lib/command.h"

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
  DB.disconnect();
  return rec;
}
unordered_map<int, vector<point2>> compression(vector<point2> org_trace,
                                               double dist, double time) {
  unordered_map<int, vector<point2>> trace_map;
  unordered_map<int, vector<point2>>::iterator getmap;
  for (auto &v : org_trace) {
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
  for (auto v : trace_map) {
    vector<point2> comtemp;
    comtemp = Compression_OW_Relat(v.second, dist, time);
    comtrace_map.insert(pair<int, vector<point2>>(v.first, comtemp));
  }

  return comtrace_map;
}
unordered_map<int, vector<point2>>
restore(unordered_map<int, vector<point2>> comtrace_map, double time) {
  unordered_map<int, vector<point2>> restrace_map;
  for (auto v : comtrace_map) {
    vector<point2> restemp;
    restemp = Restore_OW(v.second, time);
    restrace_map.insert(pair<int, vector<point2>>(v.first, restemp));
  }
  return restrace_map;
}

void saveToFile(string filename,
                unordered_map<int, vector<point2>> comtrace_map) {
  fstream file;
  file.open(filename, ios::out | ios::binary);
  for (auto m : comtrace_map) {
    for (auto v : m.second) {
      file.write((char *)&v, sizeof(v));
    }
  }
  file.close();
  return;
}
unordered_map<int, vector<point2>> readFromFile(string filename) {
  fstream infile;
  unordered_map<int, vector<point2>> comtrace_map;
  unordered_map<int, vector<point2>>::iterator getmap;
  infile.open(filename, ios::in | ios::binary);
  point2 p, pt;
  while (!infile.eof()) {
    infile.read((char *)&p, sizeof(p));
    if (!p.same(pt)) {
      getmap = comtrace_map.find(p.PersonID * 10 + p.PersonModule);
      if (getmap == comtrace_map.end()) {
        vector<point2> temp;
        temp.push_back(p);
        comtrace_map.insert(pair<int, vector<point2>>(
            (p.PersonID * 10 + p.PersonModule), temp));
      } else {
        getmap->second.push_back(p);
      }
      pt = p;
    }
  }
  infile.close();
  return comtrace_map;
}
void saveToMySQL(string host, string user, string passwd, string DBName,
                 string TableName, unordered_map<int, vector<point2>> &trace) {
  DataBase DB;
  if (DB.connect(host, user, passwd, DBName) != true) {
    cout << "connect Error" << endl;
    return;
  }
  if (DB.useDB(DBName) != true) {
    cout << "use Database Error" << endl;
    return;
  }
  if (!DB.createTB(
          TableName,
          "TraceID int UNSIGNED NOT NULL AUTO_INCREMENT,PersonID bigint "
          "NULL,PersonModule tinyint NULL,DeviceID varchar(255) NULL,X double "
          "NULL,Y double NULL,Floor char(4) NULL,MapMark bigint NULL,Time "
          "datetime NULL,PRIMARY KEY (TraceID)")) {
    cout << "creat Table Error" << endl;
    return;
  }
  bool frist = true;
  string value;
  for (auto m : trace) {
    for (auto v : m.second) {
      if (frist) {
        value = "(null," + to_string(v.PersonID) + ", " +
                to_string(v.PersonModule) + ", '" + v.DeviceID + "', " +
                to_string(v.X) + ", " + to_string(v.Y) + ", '" + v.Floor +
                "', " + to_string(v.MapMark) + ", '" + v.time + "')";
        frist = false;
      } else {
        value = value + ",(null," + to_string(v.PersonID) + ", " +
                to_string(v.PersonModule) + ", '" + v.DeviceID + "', " +
                to_string(v.X) + ", " + to_string(v.Y) + ", '" + v.Floor +
                "', " + to_string(v.MapMark) + ", '" + v.time + "')";
      }
    }
  }
  if (!DB.insertItem(TableName, value)) {
    cout << "insert Traces error" << endl;
    return;
  }
  // cout << "insert success!" << endl;
  DB.disconnect();
  return;
}
/*
int main() {

  string host = "localhost";
  string user = "root";
  string passwd = "zenos";
  string DBName = "traces";
  string TableName = "Trace202004";
  string FileName = "test.dat";
  double dist = 10;
  double time = 2;

  vector<point2> org_trace =
      readTraceFromMysql(host, user, passwd, DBName, TableName);
  cout << "Orgin Traces" << endl;
  cout << org_trace.size() << endl;

  unordered_map<int, vector<point2>> com_trace =
      compression(org_trace, dist, 2);

  saveToFile(FileName, com_trace);

  unordered_map<int, vector<point2>> read_trace = readFromFile(FileName);

  unordered_map<int, vector<point2>> res_trace = restore(read_trace, time);
  string TableNmae2 = "Trace202106";
  saveToMySQL(host, user, passwd, DBName, TableNmae2, res_trace);
  return 0;
}*/
