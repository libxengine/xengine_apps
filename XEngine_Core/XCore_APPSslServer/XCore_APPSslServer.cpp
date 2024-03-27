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
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/NetCore_Define.h>
#include <XEngine_Include/XEngine_Core/NetCore_Error.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Define.h>
#include <XEngine_Include/XEngine_Core/OPenSsl_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_Core/XEngine_Core.lib")
#pragma comment(lib,"XEngine_Core/XEngine_OPenSsl.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Core/NetCore_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Core/NetCore_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_OPenSsl/OPenSsl_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_OPenSsl/OPenSsl_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Core.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_OPenSsl.lib")
#endif
#endif

//Linux::g++ -std=gnu++17 -Wall -g XCore_APPSslServer.cpp -o XCore_APPSslServer.exe -lXEngine_BaseLib -lXEngine_Core -lXEngine_OPenSsl 

XHANDLE xhSSL = NULL;
bool CALLBACK TCPSelect_CBLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	printf("recv_Login:%s\n", lpszClientAddr);
	XCHAR tszSubject[2048];
	XCHAR tszIssus[2048];
	XCHAR tszAlg[2048];

	memset(tszSubject, '\0', sizeof(tszSubject));
	memset(tszIssus, '\0', sizeof(tszIssus));
	memset(tszAlg, '\0', sizeof(tszAlg));

	OPenSsl_Server_AcceptEx(xhSSL, hSocket, lpszClientAddr);
	OPenSsl_Server_GetSSLInfoEx(xhSSL, lpszClientAddr, tszSubject, tszIssus, tszAlg);
	printf("TCPSelect_CBLogin:%s %s %s\n", tszSubject, tszIssus, tszAlg);
	return true;
}
void CALLBACK TCPSelect_CBRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	int nLen = 2048;
	XCHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	if (OPenSsl_Server_RecvMsgEx(xhSSL, lpszClientAddr, tszMsgBuffer, &nLen, lpszRecvMsg, nMsgLen))
	{
		printf("TCPSelect_CBRecv:%s\n", tszMsgBuffer);
	}
	int nSLen = 2048;
	XCHAR tszSDBuffer[2048] = {};
	OPenSsl_Server_SendMsgEx(xhSSL, lpszClientAddr, tszMsgBuffer, nLen, tszSDBuffer, &nSLen);
	NetCore_TCPSelect_Send(lpszClientAddr, tszSDBuffer, nSLen);
}
void CALLBACK TCPSelect_CBLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	printf("TCPSelect_CBLeave:%s\n", lpszClientAddr);
}

int XCore_DTSLTest(LPCXSTR lpszCAFile, LPCXSTR lpszSrvFile, LPCXSTR lpszKeyFile)
{
	xhSSL = OPenSsl_Server_InitEx(lpszCAFile, lpszSrvFile, lpszKeyFile, false, true, XENGINE_OPENSSL_PROTOCOL_DTL_SERVER);
	if (NULL == xhSSL)
	{
		printf("OPenSsl_Server_Init %lX\n", OPenSsl_GetLastError());
		return -1;
	}
	OPenSsl_Server_ConfigEx(xhSSL);
	XHANDLE xhUDP = NetCore_UDPSelect_Start(5604);
	if (NULL == xhUDP)
	{
		printf("NetCore_UDPSelect_Start %lX\n", OPenSsl_GetLastError());
		return -1;
	}
	XCHAR tszSubject[2048];
	XCHAR tszIssus[2048];
	XCHAR tszAlg[2048];

	memset(tszSubject, '\0', sizeof(tszSubject));
	memset(tszIssus, '\0', sizeof(tszIssus));
	memset(tszAlg, '\0', sizeof(tszAlg));
	//UDP需要
	XSOCKET hSocket;
	XCHAR tszIPPort[128] = {};
	XBYTE tszSDKey[128] = {};
	XBYTE tszRVKey[128] = {};
	NetCore_UDPSelect_GetSocket(xhUDP, &hSocket);

	if (1)
	{
		OPenSsl_Server_AcceptEx(xhSSL, hSocket, NULL, tszIPPort);
	}
	else
	{
		while (true)
		{
			int nRVLen = 2048;
			int nSDLen = 2048;
			XCHAR tszRVBuffer[2048] = {};
			XCHAR tszSDBuffer[2048] = {};
			NetCore_UDPSelect_Recv(xhUDP, tszIPPort, tszRVBuffer, &nRVLen);
			printf("%d-%s\n", nRVLen, tszRVBuffer);

			if (OPenSsl_Server_AcceptMemoryEx(xhSSL, hSocket, tszIPPort, tszSDBuffer, &nSDLen, tszRVBuffer, nRVLen))
			{
				OPenSsl_Server_GetKeyEx(xhSSL, tszIPPort, tszSDKey, tszRVKey);
				break;
			}
			int nPort = 0;
			BaseLib_OperatorIPAddr_SegAddr(tszIPPort, &nPort);
			NetCore_UDPSelect_Send(xhUDP, tszSDBuffer, nSDLen, tszIPPort, nPort);
		}
	}
	
	printf("ok\n");
	while (1)
	{
		int nMSGLen = 1024;
		XCHAR tszMSGBuffer[1024] = {};
		OPenSsl_Server_RecvMsgEx(xhSSL, tszIPPort, tszMSGBuffer, &nMSGLen);
		printf("%d-%s\n", nMSGLen, tszMSGBuffer);
		OPenSsl_Server_SendMsgEx(xhSSL, tszIPPort, tszMSGBuffer, nMSGLen);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	NetCore_UDPSelect_Stop(xhSSL);
	return 0;
}
int XCore_TSLTest(LPCXSTR lpszCAFile, LPCXSTR lpszSrvFile, LPCXSTR lpszKeyFile)
{
	xhSSL = OPenSsl_Server_InitEx(lpszCAFile, lpszSrvFile, lpszKeyFile, false, true);
	if (NULL == xhSSL)
	{
		printf("OPenSsl_Server_Init %lX\n", OPenSsl_GetLastError());
		return -1;
	}
	if (!NetCore_TCPSelect_Start(5604))
	{
		printf("NetCore_TCPIocp_StartEx %lX\n", OPenSsl_GetLastError());
		return -1;
	}
	NetCore_TCPSelect_RegisterCallBack(TCPSelect_CBLogin, TCPSelect_CBRecv, TCPSelect_CBLeave);
	printf("ok\n");
	while (1)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	NetCore_TCPSelect_Stop();
	return 0;
}

int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
	LPCXSTR lpszCAFile = _X("d:\\xengine_apps\\Debug\\server.crt");
	LPCXSTR lpszSrvFile = _X("d:\\xengine_apps\\Debug\\test.xyry.org.crt");
	LPCXSTR lpszKeyFile = _X("d:\\xengine_apps\\Debug\\server.key");
#else
	LPCXSTR lpszCAFile = _X("root_bundle.crt");
	LPCXSTR lpszSrvFile = _X("test.xyry.org.crt");
	LPCXSTR lpszKeyFile = _X("test.xyry.org.key");
#endif
	XCore_DTSLTest(lpszCAFile, NULL, lpszKeyFile);
	//XCore_TSLTest(lpszCAFile, NULL, lpszKeyFile);


#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}