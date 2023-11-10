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
#include <locale.h>
#include <thread>
using namespace std;

#if 1 == _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/NetXApi_Define.h>
#include <XEngine_Include/XEngine_Core/NetXApi_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_Core/XEngine_NetXApi.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_NetXApi/NetXApi_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_NetXApi/NetXApi_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_NetXApi.lib")
#endif
#endif

//Linux:g++ -std=gnu++17 -Wall -g XCore_APPNetXApi.cpp -o XCore_APPNetXApi.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -lXEngine_BaseLib -lXEngine_NetXApi -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core,--disable-new-dtags
//Macos:g++ -std=gnu++17 -Wall -g XCore_APPNetXApi.cpp -o XCore_APPNetXApi.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_Core -lXEngine_BaseLib -lXEngine_NetXApi

static void WINAPI NetXApi_Sniffer_Callback(XHANDLE xhToken, NETXAPI_PROTOCOLINFO* pSt_ProtoInfo, LPCXSTR lpszMsgBuffer, XPVOID lParam)
{
	printf("NetXApi_Sniffer_Callback:Source:%s Dest:%s nHdrLen:%d nMsgLen:%d\n", pSt_ProtoInfo->tszSourceAddr, pSt_ProtoInfo->tszDestAddr, pSt_ProtoInfo->nHdrLen, pSt_ProtoInfo->nMsgLen);
	for (int i = 0; i < pSt_ProtoInfo->nMsgLen; i++)
	{
		printf("%02X ", (XBYTE)lpszMsgBuffer[pSt_ProtoInfo->nHdrLen + i]);
	}
	printf("\n\n");
}
int Test_NetFlow()
{
	//网络流量

	for (int i = 0; i < 2; i++)
	{
		NETXAPI_FLOWSTATE st_FlowState;
		memset(&st_FlowState, '\0', sizeof(NETXAPI_FLOWSTATE));

#ifdef _MSC_BUILD
		NetXApi_NetFlow_GetAll(&st_FlowState, "Intel(R) Wi-Fi 6E AX211 160MHz");
		
#else
		NetXApi_NetFlow_GetAll(&st_FlowState, "ens33");
#endif
		
		printf("Send:%llu Recv:%llu\n", st_FlowState.st_SendPackets.ullBytes, st_FlowState.st_RecvPackets.ullBytes);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	return 0;
}
int Test_NetSniffer()
{
	//网路嗅探
#ifdef _MSC_BUILD
	XHANDLE xhNet = NetXApi_Sniffer_Start(_X("10.10.13.53"), NetXApi_Sniffer_Callback);
#else
	XHANDLE xhNet = NetXApi_Sniffer_Start(_X("any"), NetXApi_Sniffer_Callback);
#endif
	std::this_thread::sleep_for(std::chrono::seconds(1));
	NetXApi_Sniffer_Stop(xhNet);
	return 0;
}
int Test_CtrlFlow()
{
	XNETHANDLE xhNet;
	XNETHANDLE xhFilter;
#ifdef _MSC_BUILD
	NetXApi_CtrlFlow_Init(&xhNet,NULL);
#else
	NetXApi_CtrlFlow_Init(&xhNet, "ens33");
#endif
	NetXApi_CtrlFlow_AddFlow(xhNet, &xhFilter, 1000000, 10000, 5000);

	std::this_thread::sleep_for(std::chrono::seconds(2));
	NetXApi_CtrlFlow_DelFlow(xhNet, xhFilter);
	NetXApi_CtrlFlow_Destory(xhNet);

	return 0;
}

int NetXApi_TestSocket()
{
	int nListCount = 0;
	NETXAPI_CARDINFO** ppSt_ListIFInfo;
	NetXApi_Socket_GetCardInfo(&ppSt_ListIFInfo, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		printf("name:%s ip:%s board:%s dns:%s mac:%s type:%d\n", ppSt_ListIFInfo[i]->tszIFName, ppSt_ListIFInfo[i]->tszIPAddr, ppSt_ListIFInfo[i]->tszBroadAddr, ppSt_ListIFInfo[i]->tszDnsAddr, ppSt_ListIFInfo[i]->tszMacAddr, ppSt_ListIFInfo[i]->enCardType);
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListIFInfo, nListCount);

	if (NetXApi_Socket_IsPortOccupation(5001, XENGINE_NETXAPI_SOCKET_NETSTATE_PROTOCOL_TCP))
	{
		printf("占用\n");
	}
	else
	{
		printf("未占用\n");
	}
	NETXAPI_NETSTATE st_NetState;
	memset(&st_NetState, '\0', sizeof(NETXAPI_NETSTATE));

	st_NetState.dwProtocol = XENGINE_NETXAPI_SOCKET_NETSTATE_PROTOCOL_TCP;
	NetXApi_Socket_GetPortState(5001, &st_NetState);

	ENUM_XENGINE_NETXAPI_SOCKET_CONNECTTYPE enType;
	NetXApi_Socket_GetNetConnectType(&enType);

	if (enType & ENUM_XENGINE_NETXAPI_SOCKET_CONNECTTYPE_IPV4_INTERNET)
	{
		printf("Net Type: IPV4 \n");
	}
	NETXAPI_NETTABLE** ppSt_ListTCPProcess;
	NETXAPI_NETTABLE** ppSt_ListUDPProcess;

	int nTCPCount = 0;
	int nUDPCount = 0;
	NetXApi_Socket_NetList(&ppSt_ListTCPProcess, &ppSt_ListUDPProcess, &nTCPCount, &nUDPCount);

	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListTCPProcess, nTCPCount);
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListUDPProcess, nUDPCount);

	nListCount = 0;
	XCHAR** ppszListAddr;
	NetXApi_Socket_DomainToAddr("www.baidu.com", &ppszListAddr, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		printf("NetXApi_Socket_DomainToAddr:%s\n", ppszListAddr[i]);
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppszListAddr, nListCount);
	return 0;
}

int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

	NetXApi_TestSocket();
	Test_NetSniffer();
	Test_NetFlow();
	//Test_CtrlFlow();
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}