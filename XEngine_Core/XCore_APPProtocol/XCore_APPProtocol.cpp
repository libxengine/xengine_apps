#ifdef _WINDOWS
#include <stdio.h>
#include <Windows.h>
#include <tchar.h>
#include <locale.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Protocol.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#endif
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_Protocol/Protocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_Protocol/Protocol_Error.h"

//g++ -std=gnu++17 -Wall -g XCore_APPProtocol.cpp -o XCore_APPProtocol.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -lXEngine_BaseLib -lXEngine_Protocol -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core,--disable-new-dtags

BOOL CALLBACK XEngine_Protocol_Callback_Trace(LPCTSTR lpszSource, LPCTSTR lpszDestAddr, LPCTSTR lpszRecvAddr, int nTTL, NETENGINE_VALTIME st_VALTime, LPVOID lParam)
{
	printf("Src:%s Dst:%s Rcv:%s TTL:%d Time:%lld.%llu\n", lpszSource, lpszDestAddr, lpszRecvAddr, nTTL, st_VALTime.tv_sec, st_VALTime.tv_usec);
	return TRUE;
}
int Protocol_TestPing()
{
	LPCTSTR lpszSourceAddr = _T("192.168.1.10");
	LPCTSTR lpszDestAddr = _T("42.192.166.120");
	NETENGINE_VALTIME st_VALTime;

	memset(&st_VALTime, '\0', sizeof(NETENGINE_VALTIME));

	if (!Protocol_Icmp_Ping(lpszSourceAddr, lpszDestAddr, &st_VALTime))
	{
		printf("errno\n");
		return 0;
	}

	printf("%lld %llu\n", st_VALTime.tv_sec, st_VALTime.tv_usec);

	return 0;
}
int Protocol_TestTrace()
{
	LPCTSTR lpszSourceAddr = _T("192.168.1.10");
	LPCTSTR lpszDestAddr = _T("42.192.166.120");

	if (!Protocol_Icmp_Traceroute(lpszSourceAddr, lpszDestAddr, XEngine_Protocol_Callback_Trace))
	{
		printf("errno\n");
		return 0;
	}

	return 0;
}
int main()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	Protocol_TestPing();
	Protocol_TestTrace();
#ifdef _WINDOWS
	WSACleanup();
#endif
	std::this_thread::sleep_for(std::chrono::seconds(10));
	return 0;
}