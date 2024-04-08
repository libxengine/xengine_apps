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
#include <XEngine_Include/XEngine_NetHelp/XSocket_Define.h>
#include <XEngine_Include/XEngine_NetHelp/XSocket_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_NetHelp/NetHelp_XSocket.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_XSocket/XSocket_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_XSocket/XSocket_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/NetHelp_XSocket.lib")
#endif
#endif

//Linux MACOS:g++ -std=c++17 -Wall -g NetHelp_APPXSocket.cpp -o NetHelp_APPXSocket.exe -lXEngine_BaseLib  -lNetHelp_XSocket

static void CALLBACK XSocket_Sniffer_Callback(XHANDLE xhToken, XSOCKET_PROTOCOLINFO* pSt_ProtoInfo, LPCXSTR lpszMsgBuffer, XPVOID lParam)
{
	printf("XSocket_Sniffer_Callback:Source:%s Dest:%s nHdrLen:%d nMsgLen:%d\n", pSt_ProtoInfo->tszSourceAddr, pSt_ProtoInfo->tszDestAddr, pSt_ProtoInfo->nHdrLen, pSt_ProtoInfo->nMsgLen);
	for (int i = 0; i < pSt_ProtoInfo->nMsgLen; i++)
	{
		printf("%02X ", (XBYTE)lpszMsgBuffer[pSt_ProtoInfo->nHdrLen + i]);
	}
	printf("\n\n");
}
int Test_NetSniffer()
{
	//网路嗅探
#ifdef _MSC_BUILD
	XHANDLE xhNet = XSocket_Sniffer_Start(_X("10.10.13.53"), XSocket_Sniffer_Callback);
#else
	XHANDLE xhNet = XSocket_Sniffer_Start(_X("any"), XSocket_Sniffer_Callback);
#endif
	std::this_thread::sleep_for(std::chrono::seconds(1));
	XSocket_Sniffer_Stop(xhNet);
	return 0;
}

int XSocket_TestSocket()
{
	int nListCount = 0;
	XSOCKET_CARDINFO** ppSt_ListIFInfo;
	XSocket_Api_GetCardInfo(&ppSt_ListIFInfo, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		printf("name:%s ip:%s board:%s dns:%s mac:%s type:%d\n", ppSt_ListIFInfo[i]->tszIFName, ppSt_ListIFInfo[i]->tszIPAddr, ppSt_ListIFInfo[i]->tszBroadAddr, ppSt_ListIFInfo[i]->tszDnsAddr, ppSt_ListIFInfo[i]->tszMacAddr, ppSt_ListIFInfo[i]->enCardType);
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListIFInfo, nListCount);

	if (XSocket_Api_IsPortOccupation(5001, NETHELP_XSOCKET_API_NETSTATE_PROTOCOL_TCP))
	{
		printf("占用\n");
	}
	else
	{
		printf("未占用\n");
	}
	XSOCKET_NETSTATE st_NetState;
	memset(&st_NetState, '\0', sizeof(XSOCKET_NETSTATE));

	st_NetState.dwProtocol = NETHELP_XSOCKET_API_NETSTATE_PROTOCOL_TCP;
	XSocket_Api_GetPortState(5001, &st_NetState);

	ENUM_NETHELP_XSOCKET_API_CONNECTTYPE enType;
	XSocket_Api_GetNetConnectType(&enType);

	if (enType & ENUM_NETHELP_XSOCKET_API_CONNECTTYPE_IPV4_INTERNET)
	{
		printf("Net Type: IPV4 \n");
	}
	XSOCKET_NETTABLE** ppSt_ListTCPProcess;
	XSOCKET_NETTABLE** ppSt_ListUDPProcess;

	int nTCPCount = 0;
	int nUDPCount = 0;
	XSocket_Api_NetList(&ppSt_ListTCPProcess, &ppSt_ListUDPProcess, &nTCPCount, &nUDPCount);

	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListTCPProcess, nTCPCount);
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListUDPProcess, nUDPCount);

	nListCount = 0;
	XCHAR** ppszListAddr;
	XSocket_Api_DomainToAddr("www.baidu.com", &ppszListAddr, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		printf("XSocket_Api_DomainToAddr:%s\n", ppszListAddr[i]);
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

	XSocket_TestSocket();
	Test_NetSniffer();
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}