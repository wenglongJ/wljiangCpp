#include "CodecFactory.h"

// CodecFactory: 抽象工厂基类，用于创建具体的 Codec 实例
// 注意：当前接口返回裸指针（Codec*），调用方负责释放返回的对象（见代码中 delete c; 的用法）。
// 若后续迁移到智能指针，请在工厂与调用端统一所有权语义。
CodecFactory::CodecFactory()
{
}

Codec * CodecFactory::createCodec()
{
	return NULL;
}


CodecFactory::~CodecFactory()
{
}
}
