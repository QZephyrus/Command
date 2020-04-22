#include "../include/command.h"
#include <boost/program_options.hpp>
#include <iostream>
using namespace std;
using namespace boost;
using namespace boost::program_options;

bool Compression(string host, string user, string passwd, string DBName,
                 string TableName, string filename, double dist, double time) {
  vector<point2> trace =
      readTraceFromMysql(host, user, passwd, DBName, TableName);
  if (trace.empty()) {
    return false;
  }
  unordered_map<int, vector<point2>> comtrace = compression(trace, dist, time);

  saveToFile(filename, comtrace);
  return true;
};

bool Restore(string host, string user, string passwd, string DBName,
             string TableName, string filename, double time) {
  unordered_map<int, vector<point2>> readtrace = readFromFile(filename);
  if (readtrace.empty()) {
    return false;
  }
  unordered_map<int, vector<point2>> restrace = restore(readtrace, time);

  bool flag;
  flag = saveToMySQL(host, user, passwd, DBName, TableName, restrace);
  return flag;
};

int main(int argc, char const *argv[]) {

  string host = "localhost";
  string user = "root";
  string passwd = "zenos";
  string DBName = "traces";
  string TableName;
  string FileName;
  double dist = 10;
  double time = 2;
  options_description opts("all iptions");
  variables_map vm;
  opts.add_options()("help,h", "Help message");
  opts.add_options()("compress,c", "Compression commands");
  opts.add_options()("restore,r", "Restore commands");
  opts.add_options()("host", value<std::string>(), "MySQL host");
  opts.add_options()("user,u", value<std::string>(), "MySQL user");
  opts.add_options()("passwd,p", value<std::string>(), "MySQL passwd");
  opts.add_options()("database,d", value<std::string>(), "MySQL use database");
  opts.add_options()("sheet,s", value<std::string>(),
                     "MySQL compress or restore table");
  opts.add_options()("filename,f", value<std::string>(),
                     "the file name which want to save or read");
  opts.add_options()("time,t", value<double>(), "traces time interval");
  opts.add_options()("limit,l", value<double>(), "compression limit distance");
  try {
    store(parse_command_line(argc, argv, opts), vm);
    notify(vm);
  } catch (std::exception &e) {
    cerr << "error: " + string(e.what()) << endl;
    return -1;
  } catch (...) {
    std::cout << "Exception of unknown type！\n";
    return -1;
  }
  bool _flag = false;
  if (vm.count("help")) {
    _flag = true;
    cout << opts << endl;
  }
  if (vm.count("host")) {
    host = vm["host"].as<std::string>();
  }
  if (vm.count("user")) {
    user = vm["user"].as<std::string>();
  }
  if (vm.count("passwd")) {
    passwd = vm["passwd"].as<std::string>();
  }
  if (vm.count("database")) {
    DBName = vm["database"].as<std::string>();
  }
  if (vm.count("time")) {
    time = vm["time"].as<double>();
  }

  if (vm.count("compress")) {
    _flag = true;
    bool flag = false;
    if (vm.count("filename")) {
      FileName = vm["filename"].as<std::string>();
      flag = true;
    }
    if (vm.count("sheet")) {
      TableName = vm["sheet"].as<std::string>();
      if (!flag) {
        FileName = TableName;
      }
    } else {
      cout << "You must Input Compression TableName by -tb [tablename]" << endl;
      return -1;
    }
    if (vm.count("limit")) {
      dist = vm["limit"].as<double>();
    }
    cout << "Please confirm the traces info" << endl;
    cout << "Mysql host:" + host << endl;
    cout << "Mysql user:" + user << endl;
    cout << "Mysql password:" + passwd << endl;
    cout << "Database:" + DBName << endl;
    cout << "Table:" + TableName << endl;
    cout << "compression filename:" << FileName << endl;
    cout << "compression limit distance:" << dist << endl;
    cout << "compression time interval: " << time << " second" << endl;
    cout << "Should we continue compress(Y/N):";
    string temp;
    cin >> temp;
    if (temp == "Y") {
      bool flag_com = Compression(host, user, passwd, DBName, TableName,
                                  FileName, dist, time);
      if (flag_com) {
        cout << "Compression complete" << endl;
      } else {
        cout << "Compression false" << endl;
        return -1;
      }
    } else {
      cout << "Compression Cancel" << endl;
      return 0;
    }
    return 0;
  }
  if (vm.count("restore")) {
    _flag = true;
    bool flag = false;
    if (vm.count("sheet")) {
      TableName = vm["sheet"].as<std::string>();
      flag = true;
    }
    if (vm.count("filename")) {
      FileName = vm["filename"].as<std::string>();
      if (!flag) {
        TableName = FileName;
        int pos = TableName.find_last_of('/');
        TableName = TableName.substr(pos + 1);
        pos = TableName.find_last_of('.');
        TableName = TableName.substr(0, pos);
      }
    } else {
      cout << "You must Input Restore FileName by -f [filename]" << endl;
      return -1;
    }
    cout << "Please confirm the traces info" << endl;
    cout << "Mysql host: " + host << endl;
    cout << "Mysql user: " + user << endl;
    cout << "Mysql password: " + passwd << endl;
    cout << "Restore Database: " + DBName << endl;
    cout << "Restore Table: " + TableName << endl;
    cout << "Traces filename: " << FileName << endl;
    cout << "Restore time interval: " << time << " second" << endl;
    cout << "Should we continue restore(Y/N):";
    string temp;
    cin >> temp;
    if (temp == "Y") {
      bool flag_res =
          Restore(host, user, passwd, DBName, TableName, FileName, time);
      if (flag_res) {
        cout << "Restore complete" << endl;
      } else {
        cout << "Restore false" << endl;
        return -1;
      }
    } else {
      cout << "Restore Cancel" << endl;
      return 0;
    }
    return 0;
  }
  if (!_flag) {
    cout << "You need use -c[--compress] OR -r[--restore] to compress traces "
            "or restore traces!"
         << endl;
    cout << "You can use -h[--help] for more infomation!" << endl;
  }
  return 0;
}