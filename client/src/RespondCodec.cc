#include <iostream>
#include "RespondCodec.h"

// RespondCodec - 将 RespondMsg（protobuf）与二进制字符串互转
// 说明：decodeMsg 返回指向内部成员 `m_msg` 的指针（由本对象管理），调用方不得 delete 返回指针。
RespondCodec::RespondCodec()
{
}

RespondCodec::RespondCodec(string enc)
{
	initMessage(enc);
}

RespondCodec::RespondCodec(RespondInfo *info)
{
	initMessage(info);
}

void RespondCodec::initMessage(string enc)
{
	m_encstr = enc;
}

void RespondCodec::initMessage(RespondInfo *info)
{
	m_msg.set_status(info->status);
	m_msg.set_seckeyid(info->seckeyID);
	m_msg.set_clientid(info->clientID);
	m_msg.set_serverid(info->serverID);
	m_msg.set_data(info->data);
}

string RespondCodec::encodeMsg()
{
	string output;
	m_msg.SerializeToString(&output);
	return output;
}

void* RespondCodec::decodeMsg()
{
	// 解析到内部 m_msg 并返回其地址；返回值为有效指针并由本对象管理
	m_msg.ParseFromString(m_encstr);
	return &m_msg;
}

RespondCodec::~RespondCodec()
{
}
