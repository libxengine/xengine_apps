#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Core.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_Socket.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_ProxyProtocol.lib")
#else
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
using namespace std;
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

//Linux::g++ -std=c++17 -Wall -g RfcComponents_APPProxy.cpp -o RfcComponents_APPProxy.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents -lXEngine_BaseLib -lXEngine_Core -lXClient_Socket -lRfcComponents_APPProxy -lpthread -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g RfcComponents_APPProxy.cpp -o RfcComponents_APPProxy.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_Core -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_Client -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_RfcComponents -lXEngine_BaseLib -lXEngine_Core -lXClient_Socket -lRfcComponents_APPProxy -lpthread

typedef struct
{
	TCHAR tszIPAddr[128];
	ENUM_RFCCOMPONENTS_PROXY_STATUS enStatus;
	XNETHANDLE xhClient;
	XSOCKET hSocket;
	BOOL bClose;
}PROXYPROTOCOL_CLIENTINFO;

int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

	LPCTSTR lpszClient1 = "123123";
	LPCTSTR lpszClient2 = "aaaddd";
	PROXYPROTOCOL_CLIENTINFO st_Client1;
	PROXYPROTOCOL_CLIENTINFO st_Client2;

	memset(&st_Client1, '\0', sizeof(PROXYPROTOCOL_CLIENTINFO));
	memset(&st_Client2, '\0', sizeof(PROXYPROTOCOL_CLIENTINFO));

	ProxyProtocol_TunnelCore_Create(lpszClient1);
	ProxyProtocol_TunnelCore_Create(lpszClient2);
	
	st_Client1.bClose = FALSE;
	st_Client1.hSocket = 12;
	st_Client1.xhClient = 13;
	st_Client1.enStatus = ENUM_RFCCOMPONENTS_PROXY_STATUS_CREATE;
	strcpy(st_Client1.tszIPAddr, "dwaodnwaodn");
	
	st_Client2.bClose = TRUE;
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