#pragma once
#include <string>

/*
 * TcpSocket - 客户端/服务器共享的简单套接字封装头
 * 注意事项：
 * - 不要在头文件中使用 `using namespace std;`，以免污染包含该头的全局命名空间。
 * - 超时单位请在实现中确认（默认值为 `TIMEOUT`，建议为毫秒或明确文档）。
 */
// 超时的时间（默认值，建议明确单位）
static const int TIMEOUT = 10000;

class TcpSocket
{
public:
	enum ErrorType {ParamError = 3001, TimeoutError, PeerCloseError, MallocError};
	TcpSocket();
	// 使用一个可以用于通信的套接字实例化套接字对象
	TcpSocket(int connfd);
	~TcpSocket();

	// 连接服务器
	int connectToHost(std::string ip, unsigned short port, int timeout = TIMEOUT);
	// 发送数据
	int sendMsg(std::string sendData, int timeout = TIMEOUT);
	// 接收数据
	std::string recvMsg(int timeout = TIMEOUT);
	// 断开连接
	void disConnect();

private:
	// 设置I/O为非阻塞模式
	int setNonBlock(int fd);
	// 设置I/O为阻塞模式
	int setBlock(int fd);
	// 读超时检测函数，不含读操作
	int readTimeout(unsigned int wait_seconds);
	// 写超时检测函数, 不包含写操作
	int writeTimeout(unsigned int wait_seconds);
	// 带连接超时的connect函数
	int connectTimeout(struct sockaddr_in *addr, unsigned int wait_seconds);
	// 每次从缓冲区中读取n个字符
	int readn(void *buf, int count);
	// 每次往缓冲区写入n个字符
	int writen(const void *buf, int count);

private:
	int m_socket;        // 用于通信的套接字
};

