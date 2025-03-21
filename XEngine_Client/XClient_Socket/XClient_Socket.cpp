#include "../../XEngine_ExampleHdr.h"
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
#include <locale.h>
#include <thread>
#if 1 == _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_Client/XClient_Socket.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Error.h"
#ifdef _MSC_BUILD
#ifdef _WIN64
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XClient_Socket.lib")
#else
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_Socket.lib")
#endif
#endif
#endif

//linux and macos:g++ -std=c++20 -Wall -g XClient_Socket.cpp -o XClient_Socket.exe -lXEngine_BaseLib -lXClient_Socket

int XClient_ProxyClient()
{
	LPCXSTR lpszRequestMsg = _X("CONNECT 42.194.178.57:80 HTTP/1.1\r\n"
		"Proxy-Connection: Keep-Alive\r\n"
		"Content-Length: 0\r\n"
		"Host: 42.194.178.57\r\n"
		"Proxy-Authorization: Basic MTIzMTIzYWE6MTIzMTIz\r\n"
		"User-Agent: XClient V9 - HTTP C Client Module.Windows 11 24H2\r\n\r\n");
	int nMsgLen = strlen(lpszRequestMsg);
	XCHAR tszMsgBuffer[10240];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	XSOCKET m_Socket;
	if (!XClient_TCPSelect_Create(&m_Socket, _X("10.0.4.69"), 7890))
	{
		printf(_X("连接失败！\n"));
		return -1;
	}
	
	if (XClient_TCPSelect_SendMsg(m_Socket, lpszRequestMsg, nMsgLen))
	{
		printf("NetClient_TCPSelect_SendMsg:%d\n", nMsgLen);
	}
	nMsgLen = 10240;
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	if (BaseLib_IO_Select(m_Socket, true))
	{
		if (XClient_TCPSelect_RecvMsg(m_Socket, tszMsgBuffer, &nMsgLen))
		{
			printf("XClient_TCPSelect_RecvMsg:%s\n", tszMsgBuffer);
		}
	}
	
	LPCXSTR lpszRequestMsg2 = _X("GET / HTTP/1.1\r\n"
		"Content-Length: 0\r\n"
		"Host: xyry.org\r\n"
		"User-Agent: XClient V7 - HTTP C Client Module.Windows 10 2004\r\n\r\n");
	nMsgLen = strlen(lpszRequestMsg2);
	if (XClient_TCPSelect_SendMsg(m_Socket, lpszRequestMsg2, nMsgLen))
	{
		printf("NetClient_TCPSelect_SendMsg:%d\n", nMsgLen);
	}

	for (size_t i = 0; i < 1; i++)
	{
		nMsgLen = 10240;
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		if (XClient_TCPSelect_RecvMsg(m_Socket, tszMsgBuffer, &nMsgLen))
		{
			printf("%s", tszMsgBuffer);
		}
	}
	printf("\n");
	XClient_TCPSelect_Close(m_Socket);
	return 1;
}
int TCPTest()
{
	int nMsgLen = 10240;
	XCHAR tszSDBuffer[10000];
	XCHAR tszMsgBuffer[10240];

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	XSOCKET m_Socket;
	if (!XClient_TCPSelect_Create(&m_Socket, _X("127.0.0.1"), 5200, 3))
	{
		printf(_X("连接失败！\n"));
		return -1;
	}
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	XENGINE_PROTOCOLHDR st_ProtocolHdr;
	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

	st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProtocolHdr.xhToken = 0;
	st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_AUTH;
	st_ProtocolHdr.unOperatorCode = 0x2001;
	st_ProtocolHdr.unPacketSize = 0;
	st_ProtocolHdr.byVersion = 1;
	st_ProtocolHdr.byIsReply = true;
	st_ProtocolHdr.wReserve = 0;
	st_ProtocolHdr.wPacketSerial = 0;
	st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;
	if (XClient_TCPSelect_SendMsg(m_Socket, (LPCXSTR)&st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR)))
	{
		printf("NetClient_TCPSelect_SendMsg:%d\n", nMsgLen);
	}
	if (XClient_TCPSelect_SendMsg(m_Socket, "123", 3))
	{
		printf("NetClient_TCPSelect_SendMsg:123\n");
	}
	XClient_TCPSelect_Close(m_Socket);
	return 1;
}

void CALLBACK XClient_TCPCallback_Recv(XHANDLE xhToken, XNETHANDLE xhClient, XSOCKET hSocket, ENUM_XCLIENT_SOCKET_EVENTS enTCPClientEvents, LPCXSTR lpszMsgBuffer, int nLen, XPVOID lParam)
{
	if (ENUM_XCLIENT_SOCKET_EVENT_RECV == enTCPClientEvents)
	{
		printf("Recv:%d-%d = %d\n", hSocket, enTCPClientEvents, nLen);
		XClient_TCPXCore_SendMsg(xhToken, lpszMsgBuffer, nLen);
	}
	else if (ENUM_XCLIENT_SOCKET_EVENT_CLOSE == enTCPClientEvents)
	{
		printf("Close:%d-%d\n", hSocket, enTCPClientEvents);
	}
	else if (ENUM_XCLIENT_SOCKET_EVENT_CONNECTED == enTCPClientEvents)
	{
		printf("connect:%d-%d\n", hSocket, enTCPClientEvents);
	}
	else if (ENUM_XCLIENT_SOCKET_EVENT_SEND == enTCPClientEvents)
	{
		printf("send:%d-%d\n", hSocket, enTCPClientEvents);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		XClient_TCPXCore_SendMsg(xhToken, "123456", 6);
	}
}
int TCPTestEx()
{
	XNETHANDLE xhClient[4];
	XHANDLE xhToken = XClient_TCPSelect_StartEx(XClient_TCPCallback_Recv);
	XClient_TCPSelect_HBStartEx(xhToken, 2);
	for (int i = 0; i < 4; i++)
	{
		XClient_TCPSelect_InsertEx(xhToken, &xhClient[i], _X("127.0.0.1"), 5000);
		XClient_TCPSelect_SetLimitEx(xhToken, xhClient[i], 5, 5);  //每秒发送5KB
	}
	for (int i = 0; i < 999; i++)
	{
		LPCXSTR lpszMSGBuffer = _X("123456789qwertyuiopasdfghjlzxcvbnm123456789qwertyuiopasdfghjlzxcvbnm123456789qwertyuiopasdfghjlzxcvbnm123456789qwertyuiopasdfghjlzxcvbnm");
		int nMLen = strlen(lpszMSGBuffer);
		if (XClient_TCPSelect_SendEx(xhToken, xhClient[0], lpszMSGBuffer, nMLen))
		{
			printf("ok\n");
		}
		else
		{
			printf("fail:%lX\n", XClient_GetLastError());
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	std::this_thread::sleep_for(std::chrono::seconds(0));
	for (int i = 0; i < 4; i++)
	{
		XClient_TCPSelect_DeleteEx(xhToken, xhClient[i]);
	}
	XClient_TCPSelect_StopEx(xhToken);
	return 1;
}
int TCPTestXCore()
{
	XHANDLE xhToken = XClient_TCPXCore_Connect(_X("10.0.1.88"), 5000, XClient_TCPCallback_Recv);
	if (NULL == xhToken)
	{
		printf("XClient_TCPXCore_Connect:%lX\n", XClient_GetLastError());
		return -1;
	}
	if (!XClient_TCPXCore_CBSend(xhToken, true))
	{
		printf("XClient_TCPXCore_CBSend1:%lX\n", XClient_GetLastError());
		return -1;
	}
	std::this_thread::sleep_for(std::chrono::seconds(10));
	if (!XClient_TCPXCore_CBSend(xhToken, false))
	{
		printf("XClient_TCPXCore_CBSend2:%lX\n", XClient_GetLastError());
		return -1;
	}

	std::this_thread::sleep_for(std::chrono::seconds(5));
	XClient_TCPXCore_Close(xhToken);
	return 1;
}

int Test_UDPClient()
{
	XSOCKET hSocket;
	int nMsgLen = 1024;
	XCHAR tszMsgBuffer[1024];
	XCHAR tszIPAddr[128];

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(tszIPAddr, '\0', sizeof(tszIPAddr));

	XClient_UDPSelect_Create(&hSocket, 5002);
	if (!XClient_UDPSelect_SendMsg(hSocket, "hello", 5))
	{
		printf("errno!\n");
	}
	
	if (XClient_UDPSelect_RecvMsg(hSocket, tszMsgBuffer, &nMsgLen, tszIPAddr))
	{
		printf("%d:%s:%s\n", nMsgLen, tszIPAddr, tszMsgBuffer);
	}
	XClient_UDPSelect_Close(hSocket);
	return 0;
}
int Test_Unix()
{
	int nMsgLen = 1024;
#ifdef _MSC_BUILD
	LPCXSTR lpszUnixName = _X("d:\\xengine_apps\\Debug\\unix.socket");
#else
	LPCXSTR lpszUnixName = _X("/tmp/unix.socket");
#endif

	XSOCKET m_Socket;
	if (!XClient_UnixDomain_Connect(lpszUnixName, &m_Socket))
	{
		printf(_X("连接失败！\n"));
		return -1;
	}
	LPCXSTR lpszMsgBuffer = _X("hello");
	if (XClient_UnixDomain_SendMsg(m_Socket, lpszMsgBuffer, strlen(lpszMsgBuffer)))
	{
		printf("NetClient_UnixDomain_SendMsg:%d\n", nMsgLen);
	}
	XClient_UnixDomain_Close(m_Socket);
	return 1;
}

int udx_test()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszFile = _X("d:\\xengine_apps\\Debug\\Lib_APPBaselib.exe");
#else
	LPCXSTR lpszFile = _X("Lib_APPBaselib.exe");
#endif
	FILE* pSt_File = fopen(lpszFile, "rb");
	if (NULL == pSt_File)
	{
		printf(_X("打开文件失败\n"));
		return -1;
	}
	XCLIENT_UDXCONFIG st_UDXConfig;
	memset(&st_UDXConfig, '\0', sizeof(XCLIENT_UDXCONFIG));

	st_UDXConfig.bEnableLogin = true;
	st_UDXConfig.bEnableReorder = true;
	st_UDXConfig.bEnableRryTime = true;
	st_UDXConfig.nWindowSize = 1000;

	XHANDLE xhNet = XClient_UDXSocket_InitEx(&st_UDXConfig, "10.10.13.53", 11339);
	if (NULL == xhNet)
	{
		printf(_X("初始化失败,启动错误:%lX\n"), XClient_GetLastError());
		return -1;
	}
	printf(_X("登录成功\n"));

	while (1)
	{
		XCHAR tszMsgBuffer[1024];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		int nRet = fread(tszMsgBuffer, 1, sizeof(tszMsgBuffer), pSt_File);
		if (nRet <= 0)
		{
			break;
		}
		if (XClient_UDXSocket_SendEx(xhNet, tszMsgBuffer, nRet))
		{
			printf("XClient_UDXSocket_SendEx1:%d\n", nRet);
		}
		else
		{
			printf("XClient_UDXSocket_SendEx2:%d\n", nRet);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	XClient_UDXSocket_DestroyEx(xhNet);
	return 0;
}
int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

	XClient_ProxyClient();
	//TCPTest();
	//TCPTestEx();
	//TCPTestXCore();
	//Test_UDPClient();
	//Test_Unix();
	//udx_test();
	
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 1;
}