#include "../include/command.h"
#include <iostream>
using namespace std;

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

int main(int argc, char *argv[]) {
  string host = "localhost";
  string user = "root";
  string passwd = "zenos";
  string DBName = "traces";
  string TableName;
  string FileName;
  double dist = 10;
  double time = 2;
  if (argc == 1) {
    cout << "You need to enter more info and you can use help for more options."
         << endl;
  }
  int i = 0;
  if (strcmp(argv[i], "./cli-api") == 0) {
    i++;
    if (i + 1 > argc) {
      cout
          << "you need to enter more info and you can use help for more option."
          << endl;
    }
  }
  string type = argv[i];
  if (type == "compress") {
    bool flag_tb = false;
    bool flag_fn = false;
    if (i + 1 == argc) {
      cout
          << "You need to enter more info and you can use help for more option."
          << endl;
      return 0;
    }
    for (int j = i + 1; j < argc; j++) {
      if (strcmp(argv[j], "-h") == 0) {
        j++;
        if (j == argc) {
          cout << "Lack Input Host!" << endl;
          return 0;
        }
        host = argv[j];
      } else if (strcmp(argv[j], "-u") == 0) {
        j++;
        if (j == argc) {
          cout << "Lack Input User!" << endl;
          return 0;
        }
        user = argv[j];
      } else if (strcmp(argv[j], "-p") == 0) {
        j++;
        if (j == argc) {
          cout << "Lack Input Password!" << endl;
          return 0;
        }
        passwd = argv[j];
      } else if (strcmp(argv[j], "-db") == 0) {
        j++;
        if (j == argc) {
          cout << "Lack Input DataBase Name!" << endl;
          return 0;
        }
        DBName = argv[j];
      } else if (strcmp(argv[j], "-tb") == 0) {
        j++;
        if (j == argc) {
          cout << "Lack Input TableName!" << endl;
          return 0;
        }
        TableName = argv[j];
        if (!flag_fn) {
          // FileName = TableName + ".dat";
          FileName = TableName;
        }
        flag_tb = true;
        // cout << "tablename" << endl;
      } else if (strcmp(argv[j], "-f") == 0) {
        j++;
        if (j == argc) {
          cout << "Lack Input FileName!" << endl;
          return 0;
        }
        FileName = argv[j];
        flag_fn = true;
        // FileName = FileName;
        // cout << "Filename" << endl;
      } else if (strcmp(argv[j], "-t") == 0) {
        j++;
        if (j == argc) {
          cout << "Lack Input time interval!" << endl;
          return 0;
        }
        stringstream sin(argv[j]);
        char c;
        if (!(sin >> time) || sin >> c) {
          cout << "Input time interval [" << argv[j] << "] not Number!" << endl;
          return 0;
        }
      } else if (strcmp(argv[j], "-d") == 0) {
        j++;
        if (j == argc) {
          cout << "Lack Input time distance!" << endl;
          return 0;
        }
        stringstream sin(argv[j]);
        char c;
        if (!(sin >> dist) || sin >> c) {
          cout << "Input time distance [" << argv[j] << "] not Number!" << endl;
          return 0;
        }
        // dist = atof(argv[j]);
      } else {
        cout << "Error input type [" << argv[j] << "] in compress!" << endl;
        cout << "You can use help for more options." << endl;
        return 0;
      }
    }
    if (!flag_tb) {
      cout << "No Compression TableName Input" << endl;
    } else {
      cout << "Please confirm the traces info" << endl;
      cout << "Mysql host:" + host << endl;
      cout << "Mysql user:" + user << endl;
      cout << "Mysql password:" + passwd << endl;
      cout << "Database:" + DBName << endl;
      cout << "Table:" + TableName << endl;
      cout << "compression filename:" << FileName << endl;
      cout << "compression distance:" << dist << endl;
      cout << "compression time interval:" << time << " second" << endl;
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
        }
      } else {
        cout << "Compression Cancel" << endl;
      }
    }
  } else if (type == "restore") {
    bool flag_fn = false;
    bool flag_tb = false;
    for (int j = i + 1; j < argc; j++) {
      if (strcmp(argv[j], "-h") == 0) {
        j++;
        if (j == argc) {
          cout << "Lack Input Host!" << endl;
          return 0;
        }
        host = argv[j];
      } else if (strcmp(argv[j], "-u") == 0) {
        j++;
        if (j == argc) {
          cout << "Lack Input User!" << endl;
          return 0;
        }
        user = argv[j];
      } else if (strcmp(argv[j], "-p") == 0) {
        j++;
        if (j == argc) {
          cout << "Lack Input Password!" << endl;
          return 0;
        }
        passwd = argv[j];
      } else if (strcmp(argv[j], "-db") == 0) {
        j++;
        if (j == argc) {
          cout << "Lack Input DataBase Name!" << endl;
          return 0;
        }
        DBName = argv[j];
      } else if (strcmp(argv[j], "-tb") == 0) {
        j++;
        if (j == argc) {
          cout << "Lack Input TableName!" << endl;
          return 0;
        }
        TableName = argv[j];
        flag_tb = true;
      } else if (strcmp(argv[j], "-f") == 0) {
        j++;
        if (j == argc) {
          cout << "Lack Input FileName!" << endl;
          return 0;
        }
        FileName = argv[j];
        if (!flag_tb) {
          TableName = argv[j];
          int pos = TableName.find_last_of('/');
          TableName = TableName.substr(pos + 1);
          pos = TableName.find_last_of('.');
          TableName = TableName.substr(0, pos);
        }
        flag_fn = true;
      } else if (strcmp(argv[j], "-t") == 0) {
        j++;
        if (j == argc) {
          cout << "Lack Input time interval!" << endl;
          return 0;
        }
        stringstream sin(argv[j]);
        char c;
        if (!(sin >> time) || sin >> c) {
          cout << "Input time interval [" << argv[j] << "] not Number!" << endl;
          return 0;
        }
        // time = atof(argv[j]);
      } else {
        cout << "Error input type [" << argv[j] << "] in restore!" << endl;
        cout << "You can use help for more options." << endl;
        return 0;
      }
    }
    if (!flag_fn) {
      cout << "No Restore filename Input" << endl;
    } else {
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
        }
      } else {
        cout << "Restore Cancel" << endl;
      }
    }
  } else if (type == "help") {
    cout << "You can use follow command:" << endl;
    cout << "Usage: compress [options] ,use to compress the traces" << endl;
    cout << "Options:" << endl;
    cout << "  -h,  --host[host]         mysql host" << endl;
    cout << "  -u,  --user[user]         mysql user" << endl;
    cout << "  -p,  --passwd[passwd]     mysql password" << endl;
    cout << "  -db, --database[database] traces database" << endl;
    cout << "  -tb, --table[table]       traces table[must enter]" << endl;
    cout << "  -f,  --filename[filename] save traces filename" << endl;
    cout << "  -t,  --time[time]         compression time interval" << endl;
    cout << "  -d,  --distance[distance] compression distance" << endl;
    cout << endl;
    cout << "Usage: restore [options] ,use to restore the traces" << endl;
    cout << "Options:" << endl;
    cout << "  -h,  --host[host]         mysql host" << endl;
    cout << "  -u,  --user[user]         mysql user" << endl;
    cout << "  -p,  --passwd[passwd]     mysql password" << endl;
    cout << "  -db, --database[database] save traces database" << endl;
    cout << "  -tb, --table[table]       save traces table" << endl;
    cout << "  -f,  --filename[filename] read traces filename[must enter]"
         << endl;
    cout << "  -t,  --time[time]         compression time interval" << endl;
    cout << endl;
    cout << "Usage: help ,display help for command" << endl;
  } else {
    cout << argv[i] << endl;
    cout << "Error input type " << argv[0] << "!" << endl;
    cout << "You can use help for more options." << endl;
  }
  return 0;
}