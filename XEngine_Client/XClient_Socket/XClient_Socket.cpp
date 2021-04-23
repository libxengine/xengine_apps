#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#include <locale.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_Socket.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#endif
#include <thread>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Error.h"

//g++ -std=gnu++17 -Wall -g XClient_Socket.cpp -o XClient_Socket.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client -lXEngine_BaseLib -lXClient_Socket -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client,--disable-new-dtags

int TCPTest()
{
	int nMsgLen = 10240;
	TCHAR tszSDBuffer[10000];
	TCHAR tszMsgBuffer[10240];

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	SOCKET m_Socket;
	if (!XClient_TCPSelect_Create(_T("127.0.0.1"), 5000, &m_Socket))
	{
		printf(_T("连接失败！\n"));
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
	st_ProtocolHdr.byIsReply = TRUE;
	st_ProtocolHdr.wReserve = 0;
	st_ProtocolHdr.wPacketSerial = 0;
	st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;
	if (XClient_TCPSelect_SendMsg(m_Socket, (LPCTSTR)&st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR)))
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
int TCPTestEx()
{
	int nMsgLen = 10240;
	TCHAR tszSDBuffer[10000];
	TCHAR tszMsgBuffer[10240];

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	XNETHANDLE xhToken;
	if (!XClient_TCPSelect_StartEx(&xhToken, _T("127.0.0.1"), 5000))
	{
		printf(_T("连接失败！\n"));
		return -1;
	}
	for (int i = 0; i < 3; i++)
	{
		int nLen = 6;
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		if (XClient_TCPSelect_SendEx(xhToken, "123456", &nLen))
		{
			printf("NetClient_TCPSelect_SendEx:%d\n", nMsgLen);
		}
		if (XClient_TCPSelect_RecvEx(xhToken, tszMsgBuffer, &nMsgLen, 10))
		{
			printf("NetClient_TCPSelect_RecvEx:%d = %s\n", nMsgLen, tszMsgBuffer);
		}
	}
	XClient_TCPSelect_StopEx(xhToken);
	return 1;
}
int Test_UDPClient()
{
	SOCKET hSocket;
	TCHAR tszMsgBuffer[1024];
	XClient_UDPSelect_Create(&hSocket, "127.0.0.1", 5000);

	for (int i = 0; i < 1024; i++)
	{
		tszMsgBuffer[i] = i;
	}
	if (!XClient_UDPSelect_SendMsg(hSocket, tszMsgBuffer, 1024))
	{
		printf("errno!\n");
	}
	XClient_UDPSelect_Close(hSocket);
	return 0;
}
int Test_Unix()
{
	int nMsgLen = 1024;
#ifdef _WINDOWS
	LPCTSTR lpszUnixName = _T("H:\\XEngine_Apps\\Debug\\unix.socket");
#else
	LPCTSTR lpszUnixName = _T("/tmp/unix.socket");
#endif

	SOCKET m_Socket;
	if (!XClient_UnixDomain_Connect(lpszUnixName, &m_Socket))
	{
		printf(_T("连接失败！\n"));
		return -1;
	}
	LPCTSTR lpszMsgBuffer = _T("hello");
	if (XClient_UnixDomain_SendMsg(m_Socket, lpszMsgBuffer, strlen(lpszMsgBuffer)))
	{
		printf("NetClient_UnixDomain_SendMsg:%d\n", nMsgLen);
	}
	XClient_UnixDomain_Close(m_Socket);
	return 1;
}

int udx_test()
{
#ifdef _WINDOWS
	LPCTSTR lpszFile = _T("H:\\XEngine_Apps\\Debug\\Lib_APPBaselib.exe");
#else
	LPCTSTR lpszFile = _T("Lib_APPBaselib.exe");
#endif
	XNETHANDLE xhNet;

	FILE* pSt_File = fopen(lpszFile, "rb");
	if (NULL == pSt_File)
	{
		printf(_T("打开文件失败\n"));
		return -1;
	}
	XCLIENT_UDXCONFIG st_UDXConfig;
	memset(&st_UDXConfig, '\0', sizeof(XCLIENT_UDXCONFIG));

	st_UDXConfig.bEnableLogin = TRUE;
	st_UDXConfig.bEnableReorder = TRUE;
	st_UDXConfig.bEnableRryTime = TRUE;
	st_UDXConfig.nWindowSize = 1000;

	if (!XClient_UDXSocket_InitEx(&xhNet, &st_UDXConfig, "127.0.0.1", 11339))
	{
		printf(_T("初始化失败,启动错误:%lX\n"), XClient_GetLastError());
		return -1;
	}
	printf(_T("登录成功\n"));

	while (1)
	{
		TCHAR tszMsgBuffer[1024];
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
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	TCPTest();
	TCPTestEx();
	Test_UDPClient();
	Test_Unix();
	udx_test();
	getchar();
#ifdef _WINDOWS
	WSACleanup();
#endif
	return 1;
}