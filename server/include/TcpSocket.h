#pragma once
#include <iostream>
using namespace std; // 注意：在头文件中使用 `using namespace std;` 会污染包含该头的文件命名空间，建议尽早移除并改用 `std::` 前缀。


/*
 * TcpSocket - 简明说明
 * 封装了基于 POSIX socket 的简单同步阻塞/非阻塞 I/O，带超时检测的连接、读写实现。
 * 设计要点（可从实现中确认）：
 * - 超时参数单位和语义：本文件中默认 TIMEOUT 单位为毫秒（实现需验证），超时函数以秒或毫秒混用时要特别注意。
 * - 错误/返回值语义：部分函数返回 int（0/非0），recvMsg 返回 string；调用者需根据返回值或空字符串自行判断错误或对端关闭。
 * - 线程安全：类未提供内部同步，多个线程共享同一实例时需外部加锁。
 * - 资源管理：类持有原生 socket 描述符 `m_socket`，注意复制/移动语义以避免 FD 重复关闭（头文件未声明复制/移动行为）。
 */

// 超时的时间（默认值）
static const int TIMEOUT = 10000; // 建议明确单位（ms）或改用 `std::chrono` 类型

class TcpSocket
{
public:
	// 简单错误类型枚举（实现层可能还返回 errno）
	enum ErrorType {ParamError = 3001, TimeoutError, PeerCloseError, MallocError};

	// 构造/析构
	TcpSocket();
	// 使用一个已建立的 socket fd 来包装，注意：构造后对象负责该 fd 的生命周期（请在实现处确认）
	TcpSocket(int connfd);
	~TcpSocket();

	// 连接服务器
	// - ip: IPv4 字符串，例如 "127.0.0.1"
	// - port: 端口号
	// - timeout: 超时时间（默认 TIMEOUT），建议单位为毫秒或文档明确
	// 返回：0 表示成功，非 0 表示失败（具体错误请查看实现或 errno）
	int connectToHost(string ip, unsigned short port, int timeout = TIMEOUT);

	// 发送数据（阻塞或带超时），按值传入会发生拷贝，调用者可考虑传入 std::string&& 或改为 const std::string&
	// 返回：发送的字节数或错误码（取决于实现）
	int sendMsg(string sendData, int timeout = TIMEOUT);

	// 接收数据（带超时）。返回接收到的完整字符串；若出错或超时，可能返回空字符串，调用者需区分空有效消息与错误情况。
	string recvMsg(int timeout = TIMEOUT);

	// 断开连接并释放资源
	void disConnect();

private:
	// 设置 I/O 为非阻塞模式，返回 0 表示成功
	int setNonBlock(int fd);
	// 恢复为阻塞模式
	int setBlock(int fd);
	// 读超时检测（不执行实际读操作），wait_seconds 单位需在实现中确认
	int readTimeout(unsigned int wait_seconds);
	// 写超时检测
	int writeTimeout(unsigned int wait_seconds);
	// 带超时的 connect 实现，addr 为目标地址，wait_seconds 单位参见实现
	int connectTimeout(struct sockaddr_in *addr, unsigned int wait_seconds);
	// 从缓冲区按指定字节读取（循环读直到读够 count 或错误）
	int readn(void *buf, int count);
	// 循环写直到写入 count 字节或出错
	int writen(const void *buf, int count);

private:
	int m_socket;        // 用于通信的套接字描述符（-1 表示未连接）
};

