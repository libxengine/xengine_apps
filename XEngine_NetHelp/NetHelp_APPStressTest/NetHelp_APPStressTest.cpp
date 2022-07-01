#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/NetHelp_StressTest.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_StressTest/StressTest_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_StressTest/StressTest_Error.h"

//Linux::g++ -std=c++17 -Wall -g NetHelp_APPStressTest.cpp -o NetHelp_APPStressTest.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp -lXEngine_BaseLib -lXClient_Socket -lNetHelp_StressTest -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g NetHelp_APPStressTest.cpp -o NetHelp_APPStressTest.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_Client -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_NetHelp -lXEngine_BaseLib -lXClient_Socket -lNetHelp_StressTest -Wl,-rpath,@executable_path/../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -Wl,-rpath,@executable_path/../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_Client -Wl,-rpath,@executable_path/../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_NetHelp

void CALLBACK CBDatas_TestStress(XNETHANDLE xhNet, LPCSTR lpszAddr, int nPort, int nClient, int nNumber, int nMsgLen, LPVOID lParam)
{
	printf("Token:%lld,地址：%s:%d，索引:%d 发送次数：%d\n", xhNet, lpszAddr, nPort, nClient, nNumber);
}

int main()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	XNETHANDLE xhToken;
	LPCTSTR lpszAddr = _T("192.168.1.8");
	//TCP连接测试
	NETHELP_STRESSTEST_RECONNECT st_TCPConnect;
	memset(&st_TCPConnect, '\0', sizeof(NETHELP_STRESSTEST_RECONNECT));

	st_TCPConnect.nPort = 5402;
	st_TCPConnect.nCloseWaitContTime = 0;
	st_TCPConnect.nConnectCount = 10;
	st_TCPConnect.nConnectTest = 100;
	strcpy(st_TCPConnect.tszAddr, lpszAddr);

	if (!StressTest_TCPSocket_StartConnect(&xhToken, &st_TCPConnect, CBDatas_TestStress))
	{
		printf(_T("StressTest_TCPSocket_StartConnect启动失败\n"));
		return -1;
	}
	BOOL bIsRun = TRUE;
	while (bIsRun)
	{
		StressTest_TCPSocket_GetConnect(xhToken, &bIsRun);
	}
	StressTest_TCPSocket_StopConnect(xhToken);
	//TCP数据报测试
	NETHELP_STRESSTEST_DATAS st_TCPDatas;
	memset(&st_TCPDatas, '\0', sizeof(NETHELP_STRESSTEST_DATAS));

	st_TCPDatas.nClientCount = 20;
	st_TCPDatas.nMsgLen = 5;
	st_TCPDatas.nPort = 5402;
	st_TCPDatas.nTestNumber = 10;
	st_TCPDatas.ptszMsgBuffer = (TCHAR*)malloc(5);
	strcpy(st_TCPDatas.tszAddr, lpszAddr);
	strcpy(st_TCPDatas.ptszMsgBuffer, "hello");

	if (!StressTest_TCPSocket_StartDatas(&xhToken, 2, &st_TCPDatas, CBDatas_TestStress))
	{
		printf(_T("StressTest_TCPSocket_StartDatas启动失败\n"));
		return -1;
	}
	bIsRun = TRUE;
	while (bIsRun)
	{
		StressTest_TCPSocket_GetDatas(xhToken, &bIsRun);
	}
	StressTest_TCPSocket_StopDatas(xhToken);
	//UDP数据报测试
	if (!StressTest_UDPSocket_StartDatas(&xhToken, 2, &st_TCPDatas, CBDatas_TestStress))
	{
		printf(_T("StressTest_UDPSocket_StartDatas启动失败\n"));
		return -1;
	}
	bIsRun = TRUE;
	while (bIsRun)
	{
		StressTest_UDPSocket_GetDatas(xhToken, &bIsRun);
	}
	StressTest_UDPSocket_StopDatas(xhToken);
#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}

