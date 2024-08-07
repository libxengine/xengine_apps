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
#include <string>
#include <locale.h>
#if 1 == _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#include <XEngine_Include/XEngine_Client/SslClient_Define.h>
#include <XEngine_Include/XEngine_Client/SslClient_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_Client/XClient_Socket.lib")
#pragma comment(lib,"XEngine_Client/XClient_OPenSsl.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_OPenSsl/SslClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_OPenSsl/SslClient_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_Socket.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_OPenSsl.lib")
#endif
#endif

//Linux macos:g++ -std=gnu++17 -Wall -g XClient_APPSSLClient.cpp -o XClient_APPSSLClient.exe -lXEngine_BaseLib -lXClient_Socket -lXClient_OPenSsl 

int XClient_TSLTest()
{
	XHANDLE xhNet;
	XSOCKET m_Socket;
	XCLIENT_SSLCERT_SRVINFO st_SrvInfo;
	memset(&st_SrvInfo, '\0', sizeof(XCLIENT_SSLCERT_SRVINFO));

	xhNet = XClient_OPenSsl_InitEx(ENUM_XCLIENT_SSL_TYPE_TLS_VERSION);
	if (NULL == xhNet)
	{
		printf("NetClient_OpenSsl_Init:%lX\n", XClientSsl_GetLastError());
		return -1;
	}
	if (!XClient_TCPSelect_Create(&m_Socket, "127.0.0.1", 5604))
	{
		printf("NetClient_TCPSelect_Create:%lX\n", XClient_GetLastError());
		return -1;
	}
	if (!XClient_OPenSsl_ConnectEx(xhNet, m_Socket, &st_SrvInfo))
	{
		printf("NetClient_OPenSsl_ConnectEx:%lX\n", XClientSsl_GetLastError());
		return -1;
	}
	std::string strWrite =
		"GET https://www.baidu.com/ HTTP/1.1\r\n"
		"Host: www.baidu.com\r\n"
		"Connection: close\r\n\r\n";

	if (!XClient_OPenSsl_SendMsgEx(xhNet, strWrite.c_str(), strWrite.length()))
	{
		printf("NetClient_OpenSsl_SendMsg:%lX\n", XClientSsl_GetLastError());
		return -1;
	}
	while (1)
	{
		int nLen = 2048;
		char tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		if (!XClient_OPenSsl_RecvMsgEx(xhNet, tszMsgBuffer, &nLen))
		{
			break;
		}
		printf("%s\n", tszMsgBuffer);
	}
	XClient_OPenSsl_CloseEx(xhNet);
	XClient_TCPSelect_Close(m_Socket);
	return 0;
}
int XClient_DTLTest()
{
	XHANDLE xhNet;
	XSOCKET m_Socket;
	XCLIENT_SSLCERT_SRVINFO st_SrvInfo;
	memset(&st_SrvInfo, '\0', sizeof(XCLIENT_SSLCERT_SRVINFO));

	xhNet = XClient_OPenSsl_InitEx(ENUM_XCLIENT_SSL_TYPE_DTL_VERSION);
	if (NULL == xhNet)
	{
		printf("NetClient_OpenSsl_Init:%lX\n", XClientSsl_GetLastError());
		return -1;
	}
	XClient_OPenSsl_ConfigEx(xhNet);
	if (!XClient_UDPSelect_Create(&m_Socket))
	{
		printf("NetClient_TCPSelect_Create:%lX\n", XClient_GetLastError());
		return -1;
	}
	XClient_UDPSelect_Connect(m_Socket, "127.0.0.1", 5604);
	if (!XClient_OPenSsl_ConnectEx(xhNet, m_Socket, &st_SrvInfo))
	{
		printf("NetClient_OPenSsl_ConnectEx:%lX\n", XClientSsl_GetLastError());
		return -1;
	}
	XBYTE byKEYBuffer[128] = {};
	XClient_OPenSsl_GetKeyEx(xhNet, byKEYBuffer);

	std::string strWrite =
		"GET https://www.baidu.com/ HTTP/1.1\r\n"
		"Host: www.baidu.com\r\n"
		"Connection: close\r\n\r\n";

	if (!XClient_OPenSsl_SendMsgEx(xhNet, strWrite.c_str(), strWrite.length()))
	{
		printf("NetClient_OpenSsl_SendMsg:%lX\n", XClientSsl_GetLastError());
		return -1;
	}
	while (1)
	{
		int nLen = 2048;
		char tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		if (!XClient_OPenSsl_RecvMsgEx(xhNet, tszMsgBuffer, &nLen))
		{
			break;
		}
		printf("%s\n", tszMsgBuffer);
	}
	XClient_OPenSsl_CloseEx(xhNet);
	XClient_UDPSelect_Close(m_Socket);
	return 0;
}

int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	
	XClient_DTLTest();
	//XClient_TSLTest();
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}
