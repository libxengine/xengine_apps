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
#include <string>
#include <locale.h>
#if 1 == _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#include <XEngine_Include/XEngine_Core/Cryption_Define.h>
#include <XEngine_Include/XEngine_Core/Cryption_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_Client/XClient_Socket.lib")
#pragma comment(lib,"XEngine_Client/XEngine_Cryption.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Cryption/Cryption_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Cryption/Cryption_Error.h"
#ifdef _MSC_BUILD
#ifdef _WIN64
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XClient_Socket.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_Cryption.lib")
#else
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_Socket.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Cryption.lib")
#endif
#endif
#endif

//Linux macos:g++ -std=gnu++20 -Wall -g XCore_SslClient.cpp -o XCore_SslClient.exe -lXEngine_BaseLib -lXClient_Socket -lXEngine_Cryption

int XClient_TSLTest()
{
	XHANDLE xhNet;
	XSOCKET m_Socket;
	XCHAR tszSslSubJect[MAX_PATH] = {};
	XCHAR tszSslIssuer[MAX_PATH] = {};
	XCHAR tszSslAlgorithm[MAX_PATH] = {};

	xhNet = Cryption_Client_InitEx(XENGINE_CRYPTION_PROTOCOL_TLS);
	if (NULL == xhNet)
	{
		printf("NetClient_OpenSsl_Init:%lX\n", Cryption_GetLastError());
		return -1;
	}
	if (!XClient_TCPSelect_Create(&m_Socket, "127.0.0.1", 5604))
	{
		printf("NetClient_TCPSelect_Create:%lX\n", XClient_GetLastError());
		return -1;
	}
	if (!Cryption_Client_ConnectEx(xhNet, m_Socket, tszSslSubJect, tszSslIssuer, tszSslAlgorithm))
	{
		printf("NetClient_OPenSsl_ConnectEx:%lX\n", Cryption_GetLastError());
		return -1;
	}
	std::string strWrite =
		"GET https://www.baidu.com/ HTTP/1.1\r\n"
		"Host: www.baidu.com\r\n"
		"Connection: close\r\n\r\n";

	if (!Cryption_Client_SendMsgEx(xhNet, strWrite.c_str(), strWrite.length()))
	{
		printf("NetClient_OpenSsl_SendMsg:%lX\n", Cryption_GetLastError());
		return -1;
	}
	while (1)
	{
		int nLen = 2048;
		char tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		if (!Cryption_Client_RecvMsgEx(xhNet, tszMsgBuffer, &nLen))
		{
			break;
		}
		printf("%s\n", tszMsgBuffer);
	}
	Cryption_Client_CloseEx(xhNet);
	XClient_TCPSelect_Close(m_Socket);
	return 0;
}
int XClient_DTLTest()
{
	XHANDLE xhNet;
	XSOCKET m_Socket;
	XCHAR tszSslSubJect[MAX_PATH] = {};
	XCHAR tszSslIssuer[MAX_PATH] = {};
	XCHAR tszSslAlgorithm[MAX_PATH] = {};

	xhNet = Cryption_Client_InitEx(XENGINE_CRYPTION_PROTOCOL_DTL);
	if (NULL == xhNet)
	{
		printf("NetClient_OpenSsl_Init:%lX\n", Cryption_GetLastError());
		return -1;
	}
	Cryption_Client_ConfigEx(xhNet);
	if (!XClient_UDPSelect_Create(&m_Socket))
	{
		printf("NetClient_TCPSelect_Create:%lX\n", XClient_GetLastError());
		return -1;
	}
	XClient_UDPSelect_Connect(m_Socket, "127.0.0.1", 5604);
	if (!Cryption_Client_ConnectEx(xhNet, m_Socket, tszSslSubJect, tszSslIssuer, tszSslAlgorithm))
	{
		printf("NetClient_OPenSsl_ConnectEx:%lX\n", Cryption_GetLastError());
		return -1;
	}
	XBYTE byKEYBuffer[128] = {};
	Cryption_Client_GetKeyEx(xhNet, byKEYBuffer);

	std::string strWrite =
		"GET https://www.baidu.com/ HTTP/1.1\r\n"
		"Host: www.baidu.com\r\n"
		"Connection: close\r\n\r\n";

	if (!Cryption_Client_SendMsgEx(xhNet, strWrite.c_str(), strWrite.length()))
	{
		printf("NetClient_OpenSsl_SendMsg:%lX\n", Cryption_GetLastError());
		return -1;
	}
	int nLen = 2048;
	char tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	if (!Cryption_Client_RecvMsgEx(xhNet, tszMsgBuffer, &nLen))
	{

	}
	printf("%s\n", tszMsgBuffer);

	XClient_UDPSelect_Close(m_Socket);
	Cryption_Client_CloseEx(xhNet);
	
	return 0;
}

int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

	//XClient_DTLTest();
	XClient_TSLTest();
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}
