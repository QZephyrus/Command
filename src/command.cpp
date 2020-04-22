#include "../include/command.h"

using namespace std;
vector<point2> readTraceFromMysql(string host, string user, string passwd,
                                  string DBName, string TableName) {
  DataBase DB;
  if (DB.connect(host, user, passwd, DBName) != true) {
    cout << "Connect to [" << host
         << "] Error,please check host user password and databaseName!" << endl;
    DB.disconnect();
    return {};
  }
  if (DB.useDB(DBName) != true) {
    cout << "use Database [" << DBName
         << "] Error,please check the database name!" << endl;
    DB.disconnect();
    return {};
  }
  vector<point2> rec = DB.selectTrace2(TableName);
  if (rec.empty()) {
    cout << "select traces from Table [" << TableName
         << "] Error,please check the Table name!" << endl;
    DB.disconnect();
    return {};
  }
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
  // int i=0;
  for (auto v : comtrace_map) {
    vector<point2> restemp;
    //  i+=v.second.size();
    restemp = Restore_OW(v.second, time);
    restrace_map.insert(pair<int, vector<point2>>(v.first, restemp));
  }
  // cout<<i<<endl;
  return restrace_map;
}

void saveToFile(string filename,
                unordered_map<int, vector<point2>> comtrace_map) {
  fstream file;
  string t = filename;
  int pos = t.find_last_of('/', 0);
  cout << filename << endl;
  if (pos != -1) {
    string path = filename.substr(0, pos);
    char tmp[64] = {0};
    if (0 != access(path.c_str(), F_OK)) {
      snprintf(tmp, sizeof(tmp), "mkdir -p %s", path.c_str());
      if (system(tmp) < 0) {
        cout << "Can't create the filepath [" << path << "] !" << endl;
        return;
      }
    }
  }
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
  if (!infile) {
    cout << "no such file,please check the file name[" << filename << "]!"
         << endl;
    return {};
  }
  // point2 p, pt;
  point2 p;
  try {
    // while (!infile.eof()) {
    // cout << 1 << endl;
    //  infile.read((char *)&p, sizeof(p));
    //  if (!p.same(pt)) {
    //    getmap = comtrace_map.find(p.PersonID * 10 + p.PersonModule);
    //    if (getmap == comtrace_map.end()) {
    //      vector<point2> temp;
    //      temp.push_back(p);
    //      comtrace_map.insert(pair<int, vector<point2>>(
    //          (p.PersonID * 10 + p.PersonModule), temp));
    //    } else {
    //      getmap->second.push_back(p);
    //    }
    //    pt = p;
    //  }
    //}
    while (infile.read((char *)&p, sizeof(p))) {
      if (infile.eof()) {
        break;
      }
      getmap = comtrace_map.find(p.PersonID * 10 + p.PersonModule);
      if (getmap == comtrace_map.end()) {
        vector<point2> temp;
        temp.push_back(p);
        comtrace_map.insert(pair<int, vector<point2>>(
            (p.PersonID * 10 + p.PersonModule), temp));
      } else {
        getmap->second.push_back(p);
      }
    }
  } catch (std::bad_alloc) {
    cout << " read file [" << filename << "] Error please confirm!" << endl;
    exit({});
  }
  infile.close();
  return comtrace_map;
}
bool saveToMySQL(string host, string user, string passwd, string DBName,
                 string TableName, unordered_map<int, vector<point2>> &trace) {
  DataBase DB;
  if (DB.connect(host, user, passwd, DBName) != true) {
    cout << "Connect to [" << host
         << "] Error,please check host user password and databaseName!" << endl;
    DB.disconnect();
    return false;
  }
  if (DB.useDB(DBName) != true) {
    cout << "use Database [" << DBName
         << "] Error,please check the database name!" << endl;
    DB.disconnect();
    return false;
  }
  if (!DB.createTB(
          TableName,
          "TraceID int UNSIGNED NOT NULL AUTO_INCREMENT,PersonID bigint "
          "NULL,PersonModule tinyint NULL,DeviceID varchar(255) NULL,X double "
          "NULL,Y double NULL,Floor char(4) NULL,MapMark bigint NULL,Time "
          "datetime NULL,PRIMARY KEY (TraceID)")) {
    cout << "creat Table Error" << endl;
    DB.disconnect();
    return false;
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
    cout << "Insert Traces error please confirm the file!" << endl;
    exit(0);
    return false;
  }
  // cout << "insert success!" << endl;
  DB.disconnect();
  return true;
}