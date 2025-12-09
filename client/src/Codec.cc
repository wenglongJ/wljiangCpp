#include "Codec.h"
using namespace std;

// Codec: 抽象序列化/反序列化接口
// - encodeMsg 返回序列化的字节串
// - decodeMsg 返回指向反序列化对象的指针（通常为 protobuf message），调用者需按协议释放或管理该对象
Codec::Codec()
{
}

string Codec::encodeMsg()
{
	return string();
}

void* Codec::decodeMsg()
{
	// 返回值约定：若解析失败或未实现，返回 nullptr；调用方需检查返回值是否为 nullptr
	return nullptr;
}


Codec::~Codec()
{
}
