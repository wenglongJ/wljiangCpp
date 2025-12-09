#pragma once
#include <map>
#include <string>
#include "TcpServer.h"
#include "Message.pb.h"
#include "MySQLOP.h"
#include "SecKeyShm.h"

// ServerOP: 处理客户端请求的顶层类
// 说明与注意事项：
// - `startServer()` 会创建监听并为每个连接创建工作线程（调用 `workHard`）。
// - `m_list` 存储线程到 `TcpSocket*` 的映射；线程创建后由工作线程负责删除并释放对应的 `TcpSocket` 对象。
// - 共享资源（例如 `m_list`）在当前实现中未加锁；如果多线程同时操作同一数据结构，需要在外层增加互斥保护。
// - `ServerOP` 包含 `MySQLOP` 的实例（DB）和 `SecKeyShm*`（共享内存），请注意这两者的生命周期与并发约束。
//
class ServerOP
{
public:
	enum KeyLen {Len16=16, Len24=24, Len32=32};
	ServerOP(string json);
	// 启动服务器
	void startServer();
	// 线程工作函数 -> 推荐使用
	static void* working(void* arg);
	// 友元破坏了类的封装
	friend void* workHard(void* arg);
	// 秘钥协商
	string seckeyAgree(RequestMsg* reqMsg);
	// 秘钥注销
	string seckeyRevoke(RequestMsg* reqMsg);
	~ServerOP();

private:
	string getRandKey(KeyLen len);

private:
	string m_serverID;	// 当前服务器的ID
	string m_dbUser;
	string m_dbPwd;
	string m_dbConnStr;
	unsigned short m_port;
	map<pthread_t, TcpSocket*> m_list;
	TcpServer *m_server = NULL;
	// 创建数据库实例对象
	MySQLOP m_occi;
	SecKeyShm* m_shm;
};

void* workHard(void* arg);

