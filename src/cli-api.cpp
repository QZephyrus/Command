#include "../lib/command.h"
#include <iostream>
using namespace std;

void Compression(string host, string user, string passwd, string DBName,
                 string TableName, string filename, double dist, double time) {
  vector<point2> trace =
      readTraceFromMysql(host, user, passwd, DBName, TableName);
  unordered_map<int, vector<point2>> comtrace = compression(trace, dist, time);
  saveToFile(filename, comtrace);
};

void Restore(string host, string user, string passwd, string DBName,
             string TableName, string filename, double time) {
  unordered_map<int, vector<point2>> readtrace = readFromFile(filename);
  unordered_map<int, vector<point2>> restrace = restore(readtrace, time);
  saveToMySQL(host, user, passwd, DBName, TableName, restrace);
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
        host == argv[j];
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
        FileName = TableName + ".dat";
        flag_tb = true;
        cout << "tablename" << endl;
      } else if (strcmp(argv[j], "-f") == 0) {
        j++;
        if (j == argc) {
          cout << "Lack Input FileName!" << endl;
          return 0;
        }
        FileName = argv[j];
        FileName = FileName;
        cout << "Filename" << endl;
      } else if (strcmp(argv[j], "-t") == 0) {
        j++;
        if (j == argc) {
          cout << "Lack Input time interval!" << endl;
          return 0;
        }
        time = atof(argv[j]);
      } else if (strcmp(argv[j], "-d") == 0) {
        j++;
        if (j == argc) {
          cout << "Lack Input time distance!" << endl;
          return 0;
        }
        dist = atof(argv[j]);
      } else {
        cout << "Error input type " << argv[j] << " in compress!" << endl;
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
      cout << "Table:" + DBName << endl;
      cout << "compression filename:" << FileName << endl;
      cout << "compression distance:" << dist << endl;
      cout << "compression time interval:" << time << " second" << endl;
      cout << "Should we continue compress(Y/N):";
      string temp;
      cin >> temp;
      if (temp == "Y") {
        Compression(host, user, passwd, DBName, TableName, FileName, dist,
                    time);
        cout << "Compression complete" << endl;
      } else {
        cout << "Compression Cancel" << endl;
        return 0;
      }
    }
  } else if (type == "restore") {
    bool flag_fn = false;
    for (int j = i + 1; j < argc; j++) {
      if (strcmp(argv[j], "-h") == 0) {
        j++;
        if (j == argc) {
          cout << "Lack Input Host!" << endl;
          return 0;
        }
        host == argv[j];
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
      } else if (strcmp(argv[j], "-f") == 0) {
        j++;
        if (j == argc) {
          cout << "Lack Input FileName!" << endl;
          return 0;
        }
        FileName = argv[j];
        TableName = argv[j];
        TableName = TableName.substr(0, TableName.length() - 4);
        flag_fn = true;
      } else if (strcmp(argv[j], "-t") == 0) {
        j++;
        if (j == argc) {
          cout << "Lack Input time interval!" << endl;
          return 0;
        }
        time = atof(argv[j]);
      } else {
        cout << "Error input type " << argv[j] << " in restore!" << endl;
        cout << "You can use help for more options." << endl;
        return 0;
      }
    }
    if (!flag_fn) {
      cout << "No Restore filename Input" << endl;
    } else {
      cout << "Please confirm the traces info" << endl;
      cout << "Mysql host:" + host << endl;
      cout << "Mysql user:" + user << endl;
      cout << "Mysql password:" + passwd << endl;
      cout << "Restore Database:" + DBName << endl;
      cout << "Restore Table:" + TableName << endl;
      cout << "Traces filename:" << FileName << endl;
      cout << "Restore time interval:" << time << " second" << endl;
      cout << "Should we continue restore(Y/N):";
      string temp;
      cin >> temp;
      if (temp == "Y") {
        Restore(host, user, passwd, DBName, TableName, FileName, time);
        cout << "Restore complete" << endl;
      } else {
        cout << "Restore Cancel" << endl;
        return 0;
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
    return 0;
  }
  return 0;
}