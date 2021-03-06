#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#include <locale.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_Socket.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_OPenSsl.lib")
#else
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_OPenSsl/XClientSsl_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_OPenSsl/XClientSsl_Error.h"

//Linux::g++ -std=gnu++17 -Wall -g XClient_APPSSLClient.cpp -o XClient_APPSSLClient.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client -lXEngine_BaseLib -lXClient_Socket -lXClient_OPenSsl -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client,--disable-new-dtags
//Macos::g++ -std=gnu++17 -Wall -g XClient_APPSSLClient.cpp -o XClient_APPSSLClient.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_Client -lXEngine_BaseLib -lXClient_Socket -lXClient_OPenSsl

int main()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	XHANDLE xhNet;
	SOCKET m_Socket;
	XCLIENT_SSLCERT_SRVINFO st_SrvInfo;
	memset(&st_SrvInfo, '\0', sizeof(XCLIENT_SSLCERT_SRVINFO));

	xhNet = XClient_OPenSsl_InitEx(ENUM_XCLIENT_SSL_TYPE_SSL_VERSION);
	if (NULL == xhNet)
	{
		printf("NetClient_OpenSsl_Init:%lX\n", XClientSsl_GetLastError());
		return -1;
	}
	if (!XClient_TCPSelect_Create(&m_Socket, "14.215.177.39", 443))
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
		TCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		if (!XClient_OPenSsl_RecvMsgEx(xhNet, tszMsgBuffer, &nLen))
		{
			break;
		}
		printf("%s\n", tszMsgBuffer);
	}
	XClient_OPenSsl_CloseEx(xhNet);
	XClient_TCPSelect_Close(m_Socket);
#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}
