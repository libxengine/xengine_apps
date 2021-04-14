#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#include <locale.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_Pool.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#endif
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Pool/XClientPool_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Pool/XClientPool_Error.h"

//g++ -std=gnu++17 -Wall -g XClient_PoolConnect.cpp -o XClient_PoolConnect.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client -lXEngine_BaseLib -lXClient_Pool -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client,--disable-new-dtags
int main()
{
	XNETHANDLE xhToken = 0;
	if (!XClient_PoolConnect_OPen(&xhToken, "192.168.4.37", 5001, 10, XClient_CBRecv, NULL))
	{
		printf("XClient_PoolConnect_OPen:%X\n", XClient_GetLastError());
		return -1;
	}

	while (1)
	{
		Sleep(1000);
	}
	XClient_PoolConnect_Close(xhToken);
	return 0;
}