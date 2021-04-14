#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/NetHelp_StressTesting.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_StressTesting/StressTest_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_StressTesting/StressTest_Error.h"

//g++ -std=c++17 -Wall -g NetHelp_APPStressTest.cpp -o NetHelp_APPStressTest.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp -lXEngine_BaseLib -lXEngine_Protocol -lXClient_Socket -lNetHelp_StressTesting -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp,--disable-new-dtags

bool bRun = false;
void CALLBACK CBDatas_TestStress(XNETHANDLE xhNet, LPCSTR lpszAddr, int nPort, int nClient, int nNumber, ENUM_NETHELP_STRESSTEST_STATUS enStatus, __int64 nTestTime, LPVOID lParam)
{
	printf("地址：%s:%d，发送次数：%d，%d\n", lpszAddr, nPort, nNumber, enStatus);
}

int main()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	XNETHANDLE xhToken;
	LPCTSTR lpszAddr = _T("192.168.1.10");
	
	NETHELP_STRESSTEST_RECONNECT st_StressTest;
	memset(&st_StressTest, '\0', sizeof(NETHELP_STRESSTEST_RECONNECT));

	st_StressTest.nPort = 5000;
	st_StressTest.nCloseWaitContTime = 5;
	st_StressTest.nConnectCount = 10;
	st_StressTest.nConnectTest = 100;
	strcpy(st_StressTest.tszAddr, lpszAddr);

	if (!StressTest_TCPSocket_StartConnect(&xhToken, &st_StressTest, CBDatas_TestStress))
	{
		printf(_T("启动失败！"));
	}
	bRun = true;
	std::this_thread::sleep_for(std::chrono::seconds(10));
	StressTest_TCPSocket_StopConnect(xhToken);
#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}

