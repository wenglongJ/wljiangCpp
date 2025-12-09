#include <cstdio>
#include "ServerOP.h"

int main()
{
	ServerOP op("/home/wljiang/workspace/wljiangCpp/server/src/server.json");
	op.startServer();

    return 0;
}