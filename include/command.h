#include "Compression_Trace_char.h"
#include "DataBase.h"
#include <fstream>
#include <iostream>
#include <unordered_map>

vector<point2> readTraceFromMysql(string host, string user, string passwd,
                                  string DBName, string TableName);
unordered_map<int, vector<point2>> compression(vector<point2> org_trace,
                                               double dist, double time);
unordered_map<int, vector<point2>>
restore(unordered_map<int, vector<point2>> comtrace_map, double time);
bool saveToFile(string filename,
                unordered_map<int, vector<point2>> comtrace_map);
unordered_map<int, vector<point2>> readFromFile(string filename);
bool saveToMySQL(string host, string user, string passwd, string DBName,
                 string TableName, unordered_map<int, vector<point2>> &trace);