#include "CodecFactory.h"



CodecFactory::CodecFactory()
{
}

Codec * CodecFactory::createCodec()
{
	// 返回 nullptr 表示未实现或创建失败
	return nullptr;
}


CodecFactory::~CodecFactory()
{
}
// 说明：CodecFactory 是工厂基类，具体工厂（RequestFactory/RespondFactory）负责创建具体的
// Codec 对象并返回裸指针（`new`）。调用方负责 delete 返回的指针以释放资源。
