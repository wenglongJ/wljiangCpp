#include "RequestFactory.h"

RequestFactory::RequestFactory(std::string enc) : CodecFactory()
{
	m_flag = false;
	m_encStr = enc;
}

RequestFactory::RequestFactory(RequestInfo * info) : CodecFactory()
{
	m_flag = true;
	m_info = info;
}

Codec * RequestFactory::createCodec()
{
	Codec* codec = NULL;
	if (m_flag)
	{
		codec = new RequestCodec(m_info);
	}
	else
	{
		codec = new RequestCodec(m_encStr);
	}
	return codec;
}


RequestFactory::~RequestFactory()
{
}

// 注意：返回的 Codec* 为裸指针，调用方负责释放（delete）返回的对象。将来可改为 std::unique_ptr 以明确所有权。
