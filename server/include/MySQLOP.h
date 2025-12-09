#pragma once
#include <string>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include "SeckKeyNodeInfo.h"

// MySQLOP: 简单的数据库操作封装（基于 Connector/C++）
// 约定：
// - 成员函数在内部对 Statement/PreparedStatement/ResultSet 使用 `new` 分配且负责 `delete`。
// - m_conn 由 connectDB 创建并在 closeDB 中释放（delete），请确保在析构前调用 closeDB 或依赖析构。
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