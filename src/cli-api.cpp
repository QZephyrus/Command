#include "../lib/command.h"
#include <node/node.h>

namespace demo {
using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Value;

void Compression(const FunctionCallbackInfo<Value> &args) {
  string host;
  string user;
  string passwd;
  string DBName;
  string TableName;
  string filename;
  double dist;
  double time;
  vector<point2> trace =
      readTraceFromMysql(host, user, passwd, DBName, TableName);
  unordered_map<int, vector<point2>> comtrace = compression(trace, dist, time);
  saveToFile(filename, comtrace);
};

void Restore(const FunctionCallbackInfo<Value> &args) {
  string host;
  string user;
  string passwd;
  string DBName;
  string TableName;
  string filename;
  double time;
  unordered_map<int, vector<point2>> readtrace = readFromFile(filename);
};
void Init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "compression", Compression);
  NODE_SET_METHOD(exports, "restore", Restore);
}
NODE_MODULE(AESencrypt, Init)

} // namespace demo