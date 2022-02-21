#ifdef _WINDOWS
#include <stdio.h>
#include <Windows.h>
#include <tchar.h>
#include <locale.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_NetXApi.lib")
#else
#include <sys/socket.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_NetXApi/NetXApi_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_NetXApi/NetXApi_Error.h"

//g++ -std=gnu++17 -Wall -g XCore_APPNetXApi.cpp -o XCore_APPNetXApi.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -lXEngine_BaseLib -lXEngine_NetXApi -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core,--disable-new-dtags

static void WINAPI NetXApi_Sniffer_Callback(XNETHANDLE xhNet, NETXAPI_PROTOCOLINFO* pSt_ProtoInfo, LPCTSTR lpszMsgBuffer, LPVOID lParam)
{
	printf("NetXApi_Sniffer_Callback:Source:%s Dest:%s nHdrLen:%d nMsgLen:%d\n", pSt_ProtoInfo->tszSourceAddr, pSt_ProtoInfo->tszDestAddr, pSt_ProtoInfo->nHdrLen, pSt_ProtoInfo->nMsgLen);
	for (int i = 0; i < pSt_ProtoInfo->nMsgLen; i++)
	{
		printf("%02X ", (UCHAR)lpszMsgBuffer[pSt_ProtoInfo->nHdrLen + i]);
	}
	printf("\n\n");
}
int Test_IPAddr()
{
	///*IP库
	TCHAR tszLocal[256];
	TCHAR tszArea[256];
#ifdef _WINDOWS
	if (!NetXApi_Address_OpenQQWry(_T("H:\\XEngine_Apps\\Debug\\qqwry.dat")))
#else
	if (!NetXApi_Address_OpenQQWry(_T("qqwry.dat")))
#endif
	{
		return -1;
	}
	if (!NetXApi_Address_IPtoAddr(_T("222.209.29.131"), tszLocal, tszArea))
	{
		return -2;
	}
	CHAR tszASLocal[2048];
	CHAR tszASArea[2048];

	int nLen = strlen(tszLocal);
	BaseLib_OperatorString_UTFToAnsi(tszLocal, tszASLocal, &nLen);
	nLen = strlen(tszArea);
	BaseLib_OperatorString_UTFToAnsi(tszArea, tszASArea, &nLen);
#ifdef _WINDOWS
	printf(_T("%s,%s\n"), tszASLocal, tszASArea);
#else
	printf(_T("%s,%s\n"), tszLocal, tszArea);
#endif

	if (!NetXApi_Address_IPtoAddr(_T("8.8.8.8"), tszLocal, tszArea))
	{
		return -2;
	}
	printf(_T("%s,%s\n"), tszLocal, tszArea);
	NetXApi_Address_CloseQQWry();

	return 0;
}
int Test_NetFlow()
{
	//网络流量

	for (int i = 0; i < 2; i++)
	{
		NETXAPI_FLOWSTATE st_FlowState;
		memset(&st_FlowState, '\0', sizeof(NETXAPI_FLOWSTATE));

#ifdef _WINDOWS
		NetXApi_NetFlow_GetAll(&st_FlowState, "Realtek PCIe GBE Family Controller");
		
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
	XNETHANDLE xhNet;
#ifdef _WINDOWS
	NetXApi_Sniffer_Start(&xhNet, _T("192.168.1.7"), NetXApi_Sniffer_Callback);
#else
	NetXApi_Sniffer_Start(&xhNet, _T("any"), NetXApi_Sniffer_Callback);
#endif
	std::this_thread::sleep_for(std::chrono::seconds(1));
	NetXApi_Sniffer_Stop(xhNet);
	return 0;
}
int Test_CtrlFlow()
{
	XNETHANDLE xhNet;
	XNETHANDLE xhFilter;
#ifdef _WINDOWS
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
	NETXAPI_IPSTATICS st_IPInfo;
	NETXAPI_TCPSTATICS st_TCPInfo;
	NETXAPI_UDPSTATICS st_UDPInfo;
	NETXAPI_ICMPSTATICS st_ICMPInfo;

	memset(&st_IPInfo, '\0', sizeof(NETXAPI_IPSTATICS));
	memset(&st_TCPInfo, '\0', sizeof(NETXAPI_TCPSTATICS));
	memset(&st_UDPInfo, '\0', sizeof(NETXAPI_UDPSTATICS));
	memset(&st_ICMPInfo, '\0', sizeof(NETXAPI_ICMPSTATICS));

	NetXApi_Socket_GetProtocolStatics(&st_IPInfo, &st_TCPInfo, &st_UDPInfo, &st_ICMPInfo);

	NETXAPI_PROCESSTABLE** ppSt_ListTCPProcess;
	NETXAPI_PROCESSTABLE** ppSt_ListUDPProcess;

	int nTCPCount = 0;
	int nUDPCount = 0;
	NetXApi_Socket_ProcessNet(&ppSt_ListTCPProcess, &ppSt_ListUDPProcess, &nTCPCount, &nUDPCount);

	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListTCPProcess, nTCPCount);
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListUDPProcess, nUDPCount);

	nListCount = 0;
	CHAR** ppszListAddr;
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
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	Test_NetSniffer();
	Test_IPAddr();
	Test_NetFlow();
	NetXApi_TestSocket();
	//Test_CtrlFlow();
#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}