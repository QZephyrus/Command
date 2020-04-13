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
  for (auto v : org_trace) {
    cout << v.PersonID << "," << v.PersonModule << "," << v.DeviceID << ","
         << v.X << "," << v.Y << "," << v.Floor << "," << v.MapMark << ","
         << v.time << endl;
  }

  unordered_map<int, vector<point2>> com_trace =
      compression(org_trace, dist, 2);
  cout << "Compression Traces" << endl;
  cout << com_trace.size() << endl;
  for (auto m : com_trace) {
    cout << m.first << "," << m.second.size() << endl;
    for (auto v : m.second) {
      cout << v.PersonID << "," << v.PersonModule << "," << v.DeviceID << ","
           << v.X << "," << v.Y << "," << v.Floor << "," << v.MapMark << ","
           << v.time << endl;
    }
  }

  saveToFile(FileName, com_trace);
  unordered_map<int, vector<point2>> read_trace = readFromFile(FileName);
  cout << "Read Traces" << endl;
  cout << read_trace.size() << endl;
  for (auto m : read_trace) {
    cout << m.first << "," << m.second.size() << endl;
    for (auto v : m.second) {
      cout << v.PersonID << "," << v.PersonModule << "," << v.DeviceID << ","
           << v.X << "," << v.Y << "," << v.Floor << "," << v.MapMark << ","
           << v.time << endl;
    }
  }

  unordered_map<int, vector<point2>> res_trace = restore(com_trace, time);
  cout << "Restore Traces" << endl;
  cout << res_trace.size() << endl;
  for (auto m : res_trace) {
    cout << m.first << "," << m.second.size() << endl;
    for (auto v : m.second) {
      cout << v.PersonID << "," << v.PersonModule << "," << v.DeviceID << ","
           << v.X << "," << v.Y << "," << v.Floor << "," << v.MapMark << ","
           << v.time << endl;
    }
  }

  return 0;
}