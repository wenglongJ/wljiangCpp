#pragma once
#include <string>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include "SeckKeyNodeInfo.h"

using namespace std;

class MySQLOP
{
public:
    MySQLOP();
    ~MySQLOP();

    // 初始化环境连接数据库
    bool connectDB(const string& user, const string& passwd, const string& host, 
                   unsigned int port, const string& database);
    
    // 得到keyID -> 根据实际业务需求封装的小函数
    int getKeyID();
    bool updateKeyID(int keyID);  // 修正拼写：updata -> update
    bool writeSecKey(NodeSecKeyInfo* pNode);
    void closeDB();

private:
    // 获取当前时间，并格式化为字符串
    string getCurTime();

private:
    sql::mysql::MySQL_Driver* m_driver;
    sql::Connection* m_conn;
};