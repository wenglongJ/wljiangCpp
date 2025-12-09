#pragma once
#include "BaseShm.h"
#include <cstring>
#include <string>
#include "SeckKeyNodeInfo.h"

// SecKeyShm: 共享内存中保存 NodeSecKeyInfo 的简单容器
// 重要约定：
// - 本类使用 BaseShm 的 map/unmap 操作直接读写内存，假定 NodeSecKeyInfo 是可 memcpy 的平凡类型。
// - 共享内存并发访问需额外同步（信号量或文件锁），本类不做同步保护。
class SecKeyShm : public BaseShm
{
public:
	SecKeyShm(int key, int maxNode);
	SecKeyShm(std::string pathName, int maxNode);
	~SecKeyShm();

	void shmInit();
	int shmWrite(NodeSecKeyInfo* pNodeInfo);
	NodeSecKeyInfo shmRead(std::string clientID, std::string serverID);

private:
	int m_maxNode;
};

