#ifdef _MSC_BUILD
#include <stdio.h>
#include <Windows.h>
#include <tchar.h>
#include <locale.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Core.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#endif
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Core/NetCore_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Core/NetCore_Error.h"

//Linux:g++ -std=gnu++17 -Wall -g XCore_APPService.cpp -o XCore_APPService.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -lXEngine_BaseLib -lXEngine_Algorithm -lXEngine_ManagePool -lXEngine_Core -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core,--disable-new-dtags
//Macos:g++ -std=gnu++17 -Wall -g XCore_APPService.cpp -o XCore_APPService.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_Core -lXEngine_BaseLib -lXEngine_Algorithm -lXEngine_ManagePool -lXEngine_Core

XHANDLE xhTCPCore = NULL;
XHANDLE xhUDPCore = NULL;
XHANDLE xhUDX = NULL;
TCHAR tszClientAddr[64];

BOOL CALLBACK TCPOverlapped_Login(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	strcpy(tszClientAddr, lpszClientAddr);
	printf("TCPOverlapped_Login:%s = %d\n", lpszClientAddr, hSocket);
	return TRUE;
}
void CALLBACK TCPOverlapped_Recv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	printf("TCPOverlapped_Recv:%s = %s = %d\n", lpszClientAddr, lpszRecvMsg, nMsgLen);
	_tcscpy(tszClientAddr, lpszClientAddr);
}
void CALLBACK TCPOverlapped_Leave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	printf("TCPOverlapped_Leave:%s\n", lpszClientAddr);
}
void CALLBACK Callback_UDPRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	printf("TCPOverlapped_Recv:%s = %s = %d\n", lpszClientAddr, lpszRecvMsg, nMsgLen);
	NetCore_UDPXCore_SendEx(xhUDPCore, lpszClientAddr, lpszRecvMsg, nMsgLen);
}

int test_tcpxpoll()
{
	if (NetCore_TCPXPoll_Start(5001))
	{
		printf(_T("test_tcpxpoll Start Is Ok!\n"));
	}
	else
	{
		printf(_T("test_tcpxpoll Start Is Failed!\n"));
	}
	NetCore_TCPXPoll_RegisterCallBack(TCPOverlapped_Login, TCPOverlapped_Recv, TCPOverlapped_Leave);
	return 0;
}
int test_tcpxcore()
{
	xhTCPCore = NetCore_TCPXCore_StartEx(5002);
	if (NULL != xhTCPCore)
	{
		printf(_T("NetCore_TCPXCore_StartEx Start Is Ok!\n"));
	}
	else
	{
		printf(_T("NetCore_TCPXCore_StartEx Start Is Failed!\n"));
	}
	NetCore_TCPXCore_RegisterCallBackEx(xhTCPCore,TCPOverlapped_Login, TCPOverlapped_Recv, TCPOverlapped_Leave);
	std::this_thread::sleep_for(std::chrono::milliseconds(10000));
	NetCore_TCPXCore_CloseForClientEx(xhTCPCore, tszClientAddr);
	return 0;
}
int test_udpxcore()
{
	xhUDPCore = NetCore_UDPXCore_StartEx(5002);
	if (NULL != xhUDPCore)
	{
		printf(_T("NetCore_UDPXCore_StartEx Start Is Ok!\n"));
	}
	else
	{
		printf(_T("NetCore_UDPXCore_StartEx Start Is Failed!\n"));
	}
	NetCore_UDPXCore_RegisterCallBackEx(xhUDPCore, Callback_UDPRecv);
	return 0;
}
int test_unixdomain()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszUnixName = _T("H:\\XEngine_Apps\\Debug\\unix.socket");
#else
	LPCXSTR lpszUnixName = "/tmp/unix.socket";
#endif
	if (NetCore_UnixDomain_Start(lpszUnixName))
	{
		printf(_T("NetCore_UnixDomain_Start Start Is Ok!\n"));
	}
	else
	{
		printf(_T("NetCore_UnixDomain_Start Start Is Failed!\n"));
	}
	NetCore_UnixDomain_SetCallback(TCPOverlapped_Login, TCPOverlapped_Recv, TCPOverlapped_Leave);

	return 1;
}
int test_udx()
{
#ifdef _MSC_BUILD
	LPCTSTR lpszFile = _T("H:\\XEngine_Apps\\Debug\\2.exe");
#else
	LPCTSTR lpszFile = _T("2.exe");
#endif
	NETCORE_UDXCONFIG st_UDXConfig;
	FILE* pSt_File = fopen(lpszFile, "wb");
	if (NULL == pSt_File)
	{
		return 0;
	}

	st_UDXConfig.bEnableLogin = TRUE;
	st_UDXConfig.bEnableReorder = TRUE;
	st_UDXConfig.bEnableRryTime = TRUE;
	st_UDXConfig.nWindowSize = 1000000;

	memset(tszClientAddr, '\0', sizeof(tszClientAddr));

	xhUDX = NetCore_UDXSocket_InitEx(&st_UDXConfig, 11339);
	if (NULL == xhUDX)
	{
		printf("启动UDX服务失败");
		return -1;
	}
	printf(_T("NetCore_UDXSocket_InitEx Start Is Ok!\n"));

	NetCore_UDXSocket_CBSetEx(xhUDX, TCPOverlapped_Login, TCPOverlapped_Leave);

	int nMsgLen = 2048;
	TCHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	if (NetCore_UDXSocket_RecvEx(xhUDX, tszClientAddr, tszMsgBuffer, &nMsgLen))
	{
		fwrite(tszMsgBuffer, 1, nMsgLen, pSt_File);
		//NetCore_UDXSocket_SendEx(xhNet, tszClientAddr, "abc", 3);
		//printf("%d=%d\n", i, nMsgLen);
	}
	return 0;
}

int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

	//test_tcpxpoll();
	test_tcpxcore();
	//test_udpxcore();
	//test_unixdomain();
	//test_udx();
	
	std::this_thread::sleep_for(std::chrono::seconds(50000));
	NetCore_TCPXPoll_Stop();
	NetCore_TCPXCore_DestroyEx(xhTCPCore);
	NetCore_UDPXCore_DestroyEx(xhUDPCore);
	NetCore_UnixDomain_Stop();
	NetCore_UDXSocket_DestroyEx(xhUDX);
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 1;
}