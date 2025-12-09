#include "RespondFactory.h"
using namespace std;

RespondFactory::RespondFactory(std::string enc) : CodecFactory()
{
	m_flag = false;
	m_encStr = enc;
}

RespondFactory::RespondFactory(RespondInfo * info) : CodecFactory()
{
	m_flag = true;
	m_info = info;
}

Codec * RespondFactory::createCodec()
{
	Codec* codec = NULL;
	if (m_flag)
	{
		codec = new RespondCodec(m_info);
		//m_ptr = make_shared<RespondCodec>(m_info);
	}
	else
	{
		codec = new RespondCodec(m_encStr);
		//m_ptr = make_shared<RespondCodec>(m_encStr);
	}
	return codec;
	//return m_ptr.get();
}


RespondFactory::~RespondFactory()
{
}

// 注意：返回的 Codec* 为裸指针，调用方在使用完毕后需要负责 delete，以避免内存泄漏。
