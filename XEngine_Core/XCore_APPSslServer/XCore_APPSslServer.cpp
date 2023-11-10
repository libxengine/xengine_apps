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

//Linux::g++ -std=gnu++17 -Wall -g XCore_APPSslServer.cpp -o XCore_APPSslServer.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -lXEngine_BaseLib -lXEngine_Core -lXEngine_OPenSsl -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core,--disable-new-dtags
//Macos::g++ -std=gnu++17 -Wall -g XCore_APPSslServer.cpp -o XCore_APPSslServer.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_Core -lXEngine_BaseLib -lXEngine_Core -lXEngine_OPenSsl

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

	OPenSsl_Server_AcceptEx(xhSSL, hSocket, lpszClientAddr, tszSubject, tszIssus, tszAlg);
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
}
void CALLBACK TCPSelect_CBLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	printf("TCPSelect_CBLeave:%s\n", lpszClientAddr);
}
int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
	LPCXSTR lpszCAFile = _X("d:\\xengine_apps\\Debug\\root_bundle.crt");
	LPCXSTR lpszSrvFile = _X("d:\\xengine_apps\\Debug\\test.xyry.org.crt");
	LPCXSTR lpszKeyFile = _X("d:\\xengine_apps\\Debug\\test.xyry.org.key");
#else
	LPCXSTR lpszCAFile = _X("root_bundle.crt");
	LPCXSTR lpszSrvFile = _X("test.xyry.org.crt");
	LPCXSTR lpszKeyFile = _X("test.xyry.org.key");
#endif

	xhSSL = OPenSsl_Server_InitEx(lpszCAFile, lpszSrvFile, lpszKeyFile, false, false);
	if (NULL == xhSSL)
	{
		printf("OPenSsl_Server_Init %lX\n", OPenSsl_GetLastError());
		return -1;
	}
	if (!NetCore_TCPSelect_Start(55443))
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
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}