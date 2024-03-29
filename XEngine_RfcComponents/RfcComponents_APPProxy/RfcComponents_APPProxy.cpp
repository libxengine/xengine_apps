﻿#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#else
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
using namespace std;

#if 1 == _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/NetCore_Define.h>
#include <XEngine_Include/XEngine_Core/NetCore_Error.h>
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/ProxyProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/ProxyProtocol_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_Core/XEngine_Core.lib")
#pragma comment(lib,"XEngine_Client/XClient_Socket.lib")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_ProxyProtocol.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Core/NetCore_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Core/NetCore_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_ProxyProtocol/ProxyProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_ProxyProtocol/ProxyProtocol_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Core.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_Socket.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_ProxyProtocol.lib")
#endif
#endif

//Linux::g++ -std=c++17 -Wall -g RfcComponents_APPProxy.cpp -o RfcComponents_APPProxy.exe -lXEngine_BaseLib -lXEngine_Core -lXClient_Socket -lRfcComponents_ProxyProtocol -lpthread

typedef struct
{
	XCHAR tszIPAddr[128];
	ENUM_RFCCOMPONENTS_PROXY_STATUS enStatus;
	XNETHANDLE xhClient;
	XSOCKET hSocket;
	bool bClose;
}PROXYPROTOCOL_CLIENTINFO;

int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

	LPCXSTR lpszClient1 = "123123";
	LPCXSTR lpszClient2 = "aaaddd";
	PROXYPROTOCOL_CLIENTINFO st_Client1;
	PROXYPROTOCOL_CLIENTINFO st_Client2;

	memset(&st_Client1, '\0', sizeof(PROXYPROTOCOL_CLIENTINFO));
	memset(&st_Client2, '\0', sizeof(PROXYPROTOCOL_CLIENTINFO));

	ProxyProtocol_TunnelCore_Create(lpszClient1);
	ProxyProtocol_TunnelCore_Create(lpszClient2);
	
	st_Client1.bClose = false;
	st_Client1.hSocket = 12;
	st_Client1.xhClient = 13;
	st_Client1.enStatus = ENUM_RFCCOMPONENTS_PROXY_STATUS_CREATE;
	strcpy(st_Client1.tszIPAddr, "dwaodnwaodn");
	
	st_Client2.bClose = true;
	st_Client2.hSocket = 14;
	st_Client2.xhClient = 15;
	st_Client2.enStatus = ENUM_RFCCOMPONENTS_PROXY_STATUS_FORWARD;
	strcpy(st_Client2.tszIPAddr, "2140140982");

	ProxyProtocol_TunnelCore_SetInfo(lpszClient1, &st_Client1, sizeof(PROXYPROTOCOL_CLIENTINFO));
	ProxyProtocol_TunnelCore_SetInfo(lpszClient2, &st_Client2, sizeof(PROXYPROTOCOL_CLIENTINFO));

	int nListCount = 0;
	PROXYPROTOCOL_CLIENTINFO **ppSt_ClientList;
	ProxyProtocol_TunnelCore_GetList((XPPPMEM)&ppSt_ClientList, &nListCount, sizeof(PROXYPROTOCOL_CLIENTINFO));
	for (int i = 0; i < nListCount; i++)
	{
		printf("%d %d %lld %d %s\n", ppSt_ClientList[i]->bClose, ppSt_ClientList[i]->hSocket, ppSt_ClientList[i]->xhClient, ppSt_ClientList[i]->enStatus, ppSt_ClientList[i]->tszIPAddr);
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ClientList, nListCount);
	ProxyProtocol_TunnelCore_Delete(lpszClient1);
	ProxyProtocol_TunnelCore_Delete(lpszClient2);

#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}