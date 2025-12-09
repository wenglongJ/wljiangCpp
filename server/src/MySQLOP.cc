#include <iostream>
#include <string>
#include <ctime>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include "SeckKeyNodeInfo.h"
#include "MySQLOP.h"

using namespace std;
using namespace sql;

// MySQLOP - MySQL 操作封装
// 目的：封装基础的 DB 操作（连接、获取 keyID、更新 keyID、写入密钥记录）。
// 约定与注意事项：
// - 本文件使用 C++ Connector/C++ 原生 API，返回的 Statement/ResultSet/PreparedStatement
//   均由本类分配并在使用后通过 delete 释放。调用者无需也不应释放这些内部对象。
// - 所有对 m_conn 的使用均在连接存在性检查后进行，并在异常路径中释放局部对象。
// - 时间字符串由 getCurTime() 生成，格式为 "%Y-%m-%d %H:%M:%S"。

MySQLOP::MySQLOP() : m_driver(nullptr), m_conn(nullptr)
{
}

MySQLOP::~MySQLOP()
{
    closeDB();
}

bool MySQLOP::connectDB(const string& user, const string& passwd, const string& host, 
                        unsigned int port, const string& database)
{
    try {
        // 1. 获取 MySQL 驱动实例
        m_driver = sql::mysql::get_mysql_driver_instance();
        
        // 2. 构建连接字符串
        string connStr = "tcp://" + host + ":" + to_string(port) + "/" + database;

        // 3. 建立连接
        m_conn = m_driver->connect(connStr, user, passwd);
        
        if (m_conn == nullptr) {
            cerr << "数据库连接失败: 返回空指针" << endl;
            return false;
        }

        // 4. 设置字符集
        m_conn->setClientOption("characterEncoding", "utf8");
        m_conn->setClientOption("useUnicode", "true");

        cout << "✅ MySQL 数据库连接成功..." << endl;
        return true;
        
    } catch (sql::SQLException& e) {
        cerr << "❌ MySQL 连接异常: " << e.what() << endl;
        cerr << "错误代码: " << e.getErrorCode() << endl;
        cerr << "SQLState: " << e.getSQLState() << endl;
        return false;
    }
}

int MySQLOP::getKeyID()
{
    // 查询数据库
    string sql = "SELECT ikeysn FROM SECMNG.KEYSN LIMIT 1 FOR UPDATE";
    
    Statement* stat = nullptr;
    ResultSet* resSet = nullptr;
    
    try {
        // 保护性检查：确保连接已建立
        if (m_conn == nullptr) {
            cerr << "数据库未连接: m_conn 为 nullptr" << endl;
            return -1;
        }
        stat = m_conn->createStatement();
        
        // 开启事务（MySQL 的 FOR UPDATE 需要在事务中）
        m_conn->setAutoCommit(false);
        
        // 执行数据查询
        resSet = stat->executeQuery(sql);
        
        int keyID = -1;
        if (resSet->next()) {
            keyID = resSet->getInt("ikeysn");
        }
        
        // ✅ MySQL: 不需要 closeResultSet()，直接删除结果集
        // 删除结果集与语句对象，并置为空以避免悬挂指针
        delete resSet;   // 先删除结果集
        resSet = nullptr;
        delete stat;     // 再删除语句
        stat = nullptr;
        
        m_conn->commit();  // 提交事务
        m_conn->setAutoCommit(true);  // 恢复自动提交

        return keyID;
        
    } catch (sql::SQLException& e) {
        cerr << "❌ 查询 keyID 异常: " << e.what() << endl;
        
        // 清理资源
        if (resSet != nullptr) { delete resSet; resSet = nullptr; }
        if (stat != nullptr) { delete stat; stat = nullptr; }
        
        // 发生异常时回滚
        try { 
            m_conn->rollback(); 
            m_conn->setAutoCommit(true);
        } catch (...) {}
        
        return -1;
    }
}

bool MySQLOP::updateKeyID(int keyID)
{
    // 更新数据库
    string sql = "UPDATE SECMNG.KEYSN SET ikeysn = ? WHERE 1=1";
    
    PreparedStatement* pstmt = nullptr;
    
    try {
        if (m_conn == nullptr) {
            cerr << "数据库未连接: m_conn 为 nullptr" << endl;
            return false;
        }
        pstmt = m_conn->prepareStatement(sql);
        pstmt->setInt(1, keyID);
        
        // 执行更新
        int ret = pstmt->executeUpdate();
        
        // ✅ MySQL: 使用 delete 而不是 terminateStatement
        delete pstmt;  // ← 修改这里
        pstmt = nullptr;
        
        if (ret <= 0) {
            cerr << "更新 keyID 失败，影响行数: " << ret << endl;
            return false;
        }
        
        cout << "✅ 成功更新 keyID 为: " << keyID << endl;
        return true;
        
    } catch (sql::SQLException& e) {
        cerr << "❌ 更新 keyID 异常: " << e.what() << endl;
        
        // 清理资源
        if (pstmt != nullptr) { delete pstmt; pstmt = nullptr; }
        
        return false;
    }
}

bool MySQLOP::writeSecKey(NodeSecKeyInfo* pNode)
{
    if (pNode == nullptr) {
        cerr << "❌ pNode 为空指针" << endl;
        return false;
    }

    // 使用预处理语句防止 SQL 注入
    string sql = "INSERT INTO SECMNG.SECKEYINFO(clientid, serverid, keyid, createtime, state, seckey) "
                 "VALUES (?, ?, ?, STR_TO_DATE(?, '%Y-%m-%d %H:%i:%S'), ?, ?)";
    
    PreparedStatement* pstmt = nullptr;
    
    try {
        if (m_conn == nullptr) {
            cerr << "数据库未连接: 无法写入密钥" << endl;
            return false;
        }
        pstmt = m_conn->prepareStatement(sql);
        
        // 绑定参数
        pstmt->setString(1, pNode->clientID);
        pstmt->setString(2, pNode->serverID);
        pstmt->setInt(3, pNode->seckeyID);
        pstmt->setString(4, getCurTime());
        pstmt->setInt(5, 1);
        pstmt->setString(6, pNode->seckey);
        
        // 执行插入
        int ret = pstmt->executeUpdate();
        
        // ✅ MySQL: 使用 delete 而不是 terminateStatement
        delete pstmt;  // ← 修改这里
        pstmt = nullptr;
        
        if (ret > 0) {
            cout << "✅ 成功插入密钥记录，keyid: " << pNode->seckeyID << endl;
            return true;
        } else {
            cerr << "❌ 插入密钥记录失败" << endl;
            return false;
        }
        
    } catch (sql::SQLException& e) {
        cerr << "❌ 插入密钥异常: " << e.what() << endl;
        
        // 清理资源
        if (pstmt != nullptr) { delete pstmt; pstmt = nullptr; }
        
        return false;
    }
}

void MySQLOP::closeDB()
{
    try {
        if (m_conn != nullptr) {
            // ✅ MySQL: 直接删除连接对象
            delete m_conn;
            m_conn = nullptr;
            m_driver = nullptr;
        }
    } catch (sql::SQLException& e) {
        cerr << "关闭连接时发生异常: " << e.what() << endl;
    }
}

string MySQLOP::getCurTime()
{
    time_t timep;
    time(&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));
    return tmp;
}