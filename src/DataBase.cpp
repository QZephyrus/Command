#include "../include/DataBase.h"

using namespace std;

DataBase::DataBase() {
  sql = mysql_init(nullptr);
  if (!sql) {
    cout << "init error!" << endl;
  }
  connect_flag = false;
}

DataBase::DataBase(MYSQL *mysql) {
  sql = mysql_init(mysql);
  if (!sql) {
    cout << "init error!" << endl;
  }
  connect_flag = false;
}

DataBase::~DataBase() {
  if (connect_flag) {
    disconnect();
  }
  sql = nullptr;
  mysql_free_result(res);
  res = nullptr;
}

bool DataBase::connect(const string host, const string user,
                       const string passwd) {
  const string database = "mysql";
  if (!sql) {
    cout << "sql has not been initialized!" << endl;
    return false;
  }
  if (!mysql_real_connect(sql, host.c_str(), user.c_str(), passwd.c_str(),
                          database.c_str(), 0, nullptr, 0)) {
    // cout << "connect error!" << endl;
    return false;
  }
  if (_FZIDT_DB_DEBUG) {
    cout << "connect success!" << endl;
  }

  return true;
}

bool DataBase::connect(const string host, const string user,
                       const string passwd, const string database) {
  if (!sql) {
    cout << "sql has not been initialized!" << endl;
    return false;
  }
  if (!mysql_real_connect(sql, host.c_str(), user.c_str(), passwd.c_str(),
                          database.c_str(), 0, nullptr, 0)) {
    // cout << "connect error!" << endl;
    return false;
  }
  if (_FZIDT_DB_DEBUG) {
    cout << "connect success!" << endl;
  }
  return true;
}

void DataBase::disconnect() {
  if (sql) {
    mysql_close(sql);
  }
  sql = nullptr;
  res = nullptr;
  connect_flag = false;
  if (_FZIDT_DB_DEBUG) {
    cout << "disconnect success!" << endl;
  }
}

bool DataBase::showDB() {
  string str = "show databases";
  if (mysql_query(sql, str.c_str())) {
    cout << "show databases error!" << endl;
    return false;
  }
  showres();
  return true;
}

bool DataBase::createDB(const string &database) {
  string str = "create database if not exists " + database;
  if (mysql_query(sql, str.c_str())) {
    cout << "create database " + database + " error!" << endl;
    return false;
  }
  if (_FZIDT_DB_DEBUG) {
    cout << "create database " + database + " success!" << endl;
  }
  return true;
}

bool DataBase::useDB(const string &database) {
  string str = "use " + database;
  if (mysql_query(sql, str.c_str())) {
    cout << "use database error!" << endl;
    return false;
  }
  if (_FZIDT_DB_DEBUG) {
    cout << "use " << database << " now!" << endl;
  }
  return true;
}

bool DataBase::autoCommitOff() {
  if (mysql_autocommit(sql, 0)) {
    cout << "autoCommit Off error!" << endl;
    return false;
  }
  return true;
}

bool DataBase::autoCommitOn() {
  if (mysql_autocommit(sql, 1)) {
    cout << "autoCommit On error!" << endl;
    return false;
  }
  return true;
}

bool DataBase::commit() {
  if (mysql_commit(sql)) {
    cout << "Commit error!" << endl;
    return false;
  } else {
    if (mysql_autocommit(sql, 1)) {
      cout << "autoCommit On error!" << endl;
      return false;
    }
  }
  return true;
}

bool DataBase::rollback() {
  if (mysql_rollback(sql)) {
    cout << "Commit error!" << endl;
    return false;
  } else {
    if (mysql_autocommit(sql, 1)) {
      cout << "autoCommit On error!" << endl;
      return false;
    }
  }
  return true;
}

bool DataBase::deleteDB(const string &database) {
  string str = "drop database " + database;
  if (mysql_query(sql, str.c_str())) {
    cout << "delete database " + database + " error!" << endl;
    return false;
  }
  if (_FZIDT_DB_DEBUG) {
    cout << database << " has been deleted!" << endl;
  }
  return true;
}

bool DataBase::showTB() {
  string str = "show tables";
  if (mysql_query(sql, str.c_str())) {
    cout << "show tables error!" << endl;
    return false;
  }
  showres();
  return true;
}

bool DataBase::createTB(const string &table, const string &elements) {
  string str = "create table if not exists " + table + "(" + elements + ")";
  if (mysql_query(sql, str.c_str())) {
    cerr << "create table " + table + " error!" << endl;
    return false;
  }
  if (_FZIDT_DB_DEBUG) {
    cout << "create table " + table + " success!" << endl;
  }
  return true;
}
bool DataBase::deleteTB(const string &table) {
  string str = "drop table " + table;
  if (mysql_query(sql, str.c_str())) {
    cout << "delete table " + table + " error!" << endl;
    return false;
  }
  if (_FZIDT_DB_DEBUG) {
    cout << table << " has been deleted!" << endl;
  }
  return true;
}
bool DataBase::clearTB(const string &table) {
  string str = "truncate table " + table;
  if (mysql_query(sql, str.c_str())) {
    cout << "clear table " + table + " error!" << endl;
    return false;
  }
  if (_FZIDT_DB_DEBUG) {
    cout << table << " has been cleared!" << endl;
  }
  return true;
}

vector<vector<string>> DataBase::selectItem(const string &table,
                                            const string &value) {
  string str = "select " + value + " from " + table;

  if (mysql_query(sql, str.c_str())) {
    if (_FZIDT_DB_DEBUG) {
      cout << str + " error!" << endl;
    }
    return {};
  }

  vector<vector<string>> ret;
  res = mysql_use_result(sql);
  while ((row = mysql_fetch_row(res)) != nullptr) {
    unsigned int i = 0;
    vector<string> temp;
    while (i < mysql_num_fields(res))
      temp.push_back(row[i++]);
    ret.push_back(temp);
  }
  mysql_free_result(res);
  res = nullptr;
  if (_FZIDT_DB_DEBUG) {
    cout << str + " success!" << endl;
  }
  return ret;
}

vector<vector<string>> DataBase::selectItem(const string &table,
                                            const string &value,
                                            const string &limits) {
  string str = "select " + value + " from " + table + " where " + limits;

  if (mysql_query(sql, str.c_str())) {
    if (_FZIDT_DB_DEBUG) {
      cout << str + " error!" << endl;
    }
    return {};
  }

  vector<vector<string>> ret;
  res = mysql_use_result(sql);
  while ((row = mysql_fetch_row(res)) != nullptr) {
    unsigned int i = 0;
    vector<string> temp;
    while (i < mysql_num_fields(res))
      temp.push_back(row[i++]);
    ret.push_back(temp);
  }
  mysql_free_result(res);
  res = nullptr;
  if (_FZIDT_DB_DEBUG) {
    cout << str + " success!" << endl;
  }
  return ret;
}

vector<vector<string>> DataBase::selectItem(const string &sentence) {
  string str = sentence;

  if (mysql_query(sql, str.c_str())) {
    // cout << "select error!" << endl;
    if (_FZIDT_DB_DEBUG) {
      cout << str + " error!" << endl;
    }
    return {};
  }

  vector<vector<string>> ret;
  res = mysql_use_result(sql);
  while ((row = mysql_fetch_row(res)) != nullptr) {
    unsigned int i = 0;
    vector<string> temp;
    while (i < mysql_num_fields(res))
      temp.push_back(row[i++]);
    ret.push_back(temp);
  }
  mysql_free_result(res);
  res = nullptr;
  if (_FZIDT_DB_DEBUG) {
    cout << str + " success!" << endl;
  }
  return ret;
}

vector<point> DataBase::selectTrace(const string &table) {
  // string str= "select
  // TraceID,PersonID,PersonModule,DeviceID,X,Y,Floor,MapMark,Time from " +
  // table + " where " + limits;
  string str =
      "select PersonID,PersonModule,DeviceID,X,Y,Floor,MapMark,Time from " +
      table;

  if (mysql_query(sql, str.c_str())) {
    if (_FZIDT_DB_DEBUG) {
      cout << str + " error!" << endl;
    }
    return {};
  }

  vector<point> ret;
  res = mysql_use_result(sql);
  while ((row = mysql_fetch_row(res)) != nullptr) {
    point temp;
    temp.PersonID = atoi(row[0]);
    temp.PersonModule = atoi(row[1]);
    temp.DeviceID = row[2];
    temp.X = atof(row[3]);
    temp.Y = atof(row[4]);
    temp.Floor = row[5];
    temp.MapMark = atoi(row[6]);
    temp.time = row[7];
    ret.push_back(temp);
  }

  mysql_free_result(res);
  res = nullptr;
  if (_FZIDT_DB_DEBUG) {
    cout << str + " success!" << endl;
  }
  return ret;
}

vector<point2> DataBase::selectTrace2(const string &table) {
  // string str= "select
  // TraceID,PersonID,PersonModule,DeviceID,X,Y,Floor,MapMark,Time from " +
  // table + " where " + limits;
  string str =
      "select PersonID,PersonModule,DeviceID,X,Y,Floor,MapMark,Time from " +
      table;

  if (mysql_query(sql, str.c_str())) {
    if (_FZIDT_DB_DEBUG) {
      cout << str + " error!" << endl;
    }
    return {};
  }

  vector<point2> ret;
  res = mysql_use_result(sql);
  while ((row = mysql_fetch_row(res)) != nullptr) {
    point2 temp;
    temp.PersonID = atoi(row[0]);
    temp.PersonModule = atoi(row[1]);
    strcpy(temp.DeviceID, row[2]);
    // temp.DeviceID = row[2];
    temp.X = atof(row[3]);
    temp.Y = atof(row[4]);
    strcpy(temp.Floor, row[5]);
    // temp.Floor = row[5];
    temp.MapMark = atoi(row[6]);
    strcpy(temp.time, row[7]);
    temp.isCompress = true;
    // temp.time = row[7];
    ret.push_back(temp);
  }

  mysql_free_result(res);
  res = nullptr;
  if (_FZIDT_DB_DEBUG) {
    cout << str + " success!" << endl;
  }
  return ret;
}

void DataBase::showres() {
  res = mysql_use_result(sql);
  cout << "****************The result is:****************" << endl;
  while ((row = mysql_fetch_row(res)) != nullptr) {
    unsigned int i = 0;
    while (i < mysql_num_fields(res))
      cout << row[i++] << "\t";
    cout << endl;
  }
  cout << "**********************************************" << endl;
  mysql_free_result(res);
  res = nullptr;
  return;
}

bool DataBase::insertItem(const string &table, const string &value) {
  string str = "insert into " + table + " values " + value;
  if (mysql_query(sql, str.c_str())) {
    if (_FZIDT_DB_DEBUG) {
      cout << str + " error!" << endl;
    }

    return false;
  }
  if (_FZIDT_DB_DEBUG) {
    cout << str + " success!" << endl;
  }
  return true;
}

bool DataBase::insertItem(const string &sentence) {
  string str = sentence;
  if (mysql_query(sql, str.c_str())) {
    // cout << "insert into " + table + " error!" << endl;
    if (_FZIDT_DB_DEBUG) {
      cout << str + " error!" << endl;
    }
    return false;
  }
  if (_FZIDT_DB_DEBUG) {
    // cout << "insert into " + table + " success!" << endl;
    cout << str + " success!" << endl;
  }
  return true;
}

bool DataBase::insertItem(const string &table, const string &value,
                          const string &col) {
  string str = "insert into " + table + " " + col + " values " + value;

  if (mysql_query(sql, str.c_str())) {
    // cout << "insert into " + table + " error!" << endl;
    if (_FZIDT_DB_DEBUG) {
      cout << str + " error!" << endl;
    }
    return false;
  }
  if (_FZIDT_DB_DEBUG) {
    // cout << "insert into " + table + " success!" << endl;
    cout << str + " success!" << endl;
  }
  return true;
}

bool DataBase::deleteItem(const string &table, const string &value) {
  string str = "delete from " + table + " where " + value;

  if (mysql_query(sql, str.c_str())) {
    // cout << "delete from " + table + " error!" << endl;
    if (_FZIDT_DB_DEBUG) {
      cout << str + " error!" << endl;
    }
    return false;
  }
  if (_FZIDT_DB_DEBUG) {
    // cout << "delete from " + table + " success!" << endl;
    cout << str + " success!" << endl;
  }
  return true;
}

bool DataBase::replaceItem(const string &table, const string &value) {
  string str = "replace " + table + " values" + value;
  if (mysql_query(sql, str.c_str())) {
    if (_FZIDT_DB_DEBUG) {
      cout << str + " error!" << endl;
    }
    return false;
  }
  if (_FZIDT_DB_DEBUG) {
    cout << str + " success!" << endl;
  }
  return true;
}

bool DataBase::updateItem(const string &table, const string &value,
                          const string &limits) {
  string str = "update " + table + " set " + value + " where " + limits;

  if (mysql_query(sql, str.c_str())) {
    // cout << "update " + table + " error!" << endl;
    if (_FZIDT_DB_DEBUG) {
      cout << str + " error!" << endl;
    }
    return false;
  }
  if (_FZIDT_DB_DEBUG) {
    // cout << "update " + table + " success!" << endl;
    cout << str + " success!" << endl;
  }
  return true;
}

bool DataBase::query(const string &command) {
  if (mysql_query(sql, command.c_str())) {
    cout << "query error!" << endl;
    return false;
  }
  if (_FZIDT_DB_DEBUG) {
    cout << "query success!" << endl;
  }
  return true;
}