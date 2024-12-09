#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#else
#include <sys/socket.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
using namespace std;
#if 1 == _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIFlow_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIFlow_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_NetHelp/NetHelp_APIFlow.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_APIFlow/APIFlow_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_APIFlow/APIFlow_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/NetHelp_APIFlow.lib")
#endif
#endif

//Linux MACOS:g++ -std=c++17 -Wall -g NetHelp_APPFlow.cpp -o NetHelp_APPFlow.exe -lXEngine_BaseLib  -lNetHelp_APIFlow

int Test_NetFlow()
{
	//网络流量

	for (int i = 0; i < 2; i++)
	{
		APIFLOW_STATUSINFO st_FlowInfo;
		memset(&st_FlowInfo, '\0', sizeof(APIFLOW_STATUSINFO));

#ifdef _MSC_BUILD
		APIFlow_NetGet_All(&st_FlowInfo, "Intel(R) Wi-Fi 6E AX211 160MHz");

#else
		APIFlow_NetGet_All(&st_FlowInfo, "ens33");
#endif

		printf("Send:%llu Recv:%llu\n", st_FlowInfo.st_SendPackets.ullBytes, st_FlowInfo.st_RecvPackets.ullBytes);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	return 0;
}
int Test_CtrlFlow()
{
	XNETHANDLE xhNet;
	XNETHANDLE xhFilter;
#ifdef _MSC_BUILD
	APIFlow_NetCtrl_Init(&xhNet, NULL);
#else
	APIFlow_NetCtrl_Init(&xhNet, "ens33");
#endif
	APIFlow_NetCtrl_AddFlow(xhNet, &xhFilter, 1000000, 10000, 5000);

	std::this_thread::sleep_for(std::chrono::seconds(2));
	APIFlow_NetCtrl_DelFlow(xhNet, xhFilter);
	APIFlow_NetCtrl_Destory(xhNet);

	return 0;
}

int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

	Test_NetFlow();
	Test_CtrlFlow();
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}