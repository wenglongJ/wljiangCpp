#include <cstdio>
#include "ServerOP.h"

int main()
{
	// 注意：此处使用了硬编码的配置文件路径，部署时请修改为相对路径或通过命令行参数传入
	ServerOP op("/home/wljiang/workspace/wljiangCpp/server/src/server.json");
	op.startServer();

    return 0;
}