#include <string>
#include <iostream>
#include <cstring>
#include "SecKeyShm.h"
using namespace std;

SecKeyShm::SecKeyShm(int key, int maxNode)
	: BaseShm(key, maxNode * sizeof(NodeSecKeyInfo))
	, m_maxNode(maxNode)
{
}

SecKeyShm::SecKeyShm(string pathName, int maxNode)
	: BaseShm(pathName, maxNode * sizeof(NodeSecKeyInfo))
	, m_maxNode(maxNode)
{
}

SecKeyShm::~SecKeyShm()
{
}

void SecKeyShm::shmInit()
{
	if (m_shmAddr != NULL)
	{
		memset(m_shmAddr, 0, m_maxNode * sizeof(NodeSecKeyInfo));
	}
}

int SecKeyShm::shmWrite(NodeSecKeyInfo * pNodeInfo)
{
	int ret = -1;
	// 关联共享内存
	NodeSecKeyInfo* pAddr = static_cast<NodeSecKeyInfo*>(mapShm());
	if (pAddr == NULL)
	{
		return ret;
	}

	// 判断传入的网点密钥是否已经存在
	NodeSecKeyInfo	*pNode = NULL;
	for (int i = 0; i < m_maxNode; i++)
	{
		// pNode依次指向每个节点的首地址
		pNode = pAddr + i;
		// 调试输出已移除；生产环境请使用统一日志并避免打印敏感信息
		if (strcmp(pNode->clientID, pNodeInfo->clientID) == 0 &&
			strcmp(pNode->serverID, pNodeInfo->serverID) == 0)
		{
			// 如果找到了该网点秘钥已经存在, 使用新秘钥覆盖旧的值
			memcpy(pNode, pNodeInfo, sizeof(NodeSecKeyInfo));
			unmapShm();
			return 0;
		}
	}

	// 若没有找到对应的信息, 找一个空节点将秘钥信息写入
	int i = 0;
	// 必须初始化 tmpNodeInfo 为全 0，否则 memcmp 会读取未初始化内存导致未定义行为
	NodeSecKeyInfo tmpNodeInfo{}; //空结点（值初始化）
	for (i = 0; i < m_maxNode; i++)
	{
		pNode = pAddr + i;
		if (memcmp(&tmpNodeInfo, pNode, sizeof(NodeSecKeyInfo)) == 0)
		{
			ret = 0;
			std::cout <<pNodeInfo->clientID<<" , "<<pNodeInfo->serverID<<std::endl;
			memcpy(pNode, pNodeInfo, sizeof(NodeSecKeyInfo));
			cout << "写数据成功: 在新的节点上添加数据!" << endl;
			break;
		}
	}
	if (i == m_maxNode)
	{
		ret = -1;
	}

	unmapShm();
	return ret;
}

NodeSecKeyInfo SecKeyShm::shmRead(const string &clientID, const string &serverID)
{
	int ret = 0;
	// 关联共享内存
	NodeSecKeyInfo *pAddr = NULL;
	pAddr = static_cast<NodeSecKeyInfo*>(mapShm());
	if (pAddr == NULL)
	{
		// 关联失败
		return NodeSecKeyInfo();
	}

	//遍历网点信息
	int i = 0;
	NodeSecKeyInfo info;
	NodeSecKeyInfo	*pNode = NULL;
	// 通过clientID和serverID查找节点
	std::cout << "开始查找节点: " << clientID << ", " << serverID << std::endl;
	for (i = 0; i < m_maxNode; i++)
	{
		std::cout << "检查节点索引: " << i << std::endl;
		pNode = pAddr + i;
		std::cout << "节点信息: " << pNode->clientID << ", " << pNode->serverID << std::endl;
		// 调试输出已移除
		if (strcmp(pNode->clientID, clientID.data()) == 0 &&
			strcmp(pNode->serverID, serverID.data()) == 0)
		{
			// 找到的节点信息, 拷贝到传出参数
			info = *pNode;
			cout << "++++++++++++++" << endl;
			cout << info.clientID << " , " << info.serverID << ", "
				<< info.seckeyID << ", " << info.status << ", "
				<< info.seckey << endl;
			cout << "===============" << endl;
			break;
		}
	}

	unmapShm();
	return info;
}
