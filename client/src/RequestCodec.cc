#include <iostream>
#include "RequestCodec.h"

// RequestCodec - 将 RequestMsg（protobuf）与二进制字符串互转
// 说明：decodeMsg 返回指向内部成员 `m_msg` 的裸指针（非 new 分配），调用方不得 delete 返回指针。
// 如果需要跨作用域传递，请自行拷贝或改用智能指针封装。
RequestCodec::RequestCodec()
{
}

RequestCodec::RequestCodec(string encstr)
{
	initMessage(encstr);
}

RequestCodec::RequestCodec(RequestInfo *info)
{
	initMessage(info);
}

void RequestCodec::initMessage(string encstr)
{
	m_encStr = encstr;
}

void RequestCodec::initMessage(RequestInfo *info)
{
	m_msg.set_cmdtype(info->cmd);
	m_msg.set_clientid(info->clientID);
	m_msg.set_serverid(info->serverID);
	m_msg.set_sign(info->sign);
	m_msg.set_data(info->data);
}

string RequestCodec::encodeMsg()
{
	string output;
	m_msg.SerializeToString(&output);
	return output;
}

void* RequestCodec::decodeMsg()
{
	// 解析到内部 m_msg 并返回其地址；返回值为有效指针并由本对象管理
	m_msg.ParseFromString(m_encStr);
	return &m_msg;
}

RequestCodec::~RequestCodec()
{
}
