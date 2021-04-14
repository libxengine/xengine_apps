#ifdef _WINDOWS
#include <stdio.h>
#include <Windows.h>
#include <tchar.h>
#include <locale.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Core.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_OPenSsl.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#endif
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_Core/NetCore_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_Core/NetCore_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_OPenSsl/OPenSsl_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_OPenSsl/OPenSsl_Error.h"

//g++ -std=gnu++17 -Wall -g XCore_APPSsl.cpp -o XCore_APPSsl.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -lXEngine_BaseLib -lXEngine_Core -lXEngine_OPenSsl -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core,--disable-new-dtags

BOOL CALLBACK TCPSelect_CBLogin(LPCSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	printf("recv_Login:%s\n", lpszClientAddr);
	TCHAR tszSubject[2048];
	TCHAR tszIssus[2048];
	TCHAR tszAlg[2048];

	memset(tszSubject, '\0', sizeof(tszSubject));
	memset(tszIssus, '\0', sizeof(tszIssus));
	memset(tszAlg, '\0', sizeof(tszAlg));

	OPenSsl_Server_Accept(hSocket, lpszClientAddr, tszSubject, tszIssus, tszAlg);
	printf("TCPSelect_CBLogin:%s %s %s\n", tszSubject, tszIssus, tszAlg);
	return TRUE;
}
void CALLBACK TCPSelect_CBRecv(LPCSTR lpszClientAddr, SOCKET hSocket, LPCSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	int nLen = 2048;
	TCHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	if (OPenSsl_Server_Recv(lpszClientAddr, tszMsgBuffer, &nLen))
	{
		printf("TCPSelect_CBRecv:%s\n", tszMsgBuffer);
	}
}
void CALLBACK TCPSelect_CBLeave(LPCSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	printf("TCPSelect_CBLeave:%s\n", lpszClientAddr);
}
int main()
{
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
	//生成方式参考论坛
	LPCTSTR lpszCAFile = _T("H:\\XEngine_Apps\\Debug\\ca.crt");
	LPCTSTR lpszSrvFile = _T("H:\\XEngine_Apps\\Debug\\server.crt");
	LPCTSTR lpszKeyFile = _T("H:\\XEngine_Apps\\Debug\\server.key");

	if (!OPenSsl_Server_Init(lpszCAFile, lpszSrvFile, lpszKeyFile))
	{
		printf("OPenSsl_Server_Init %lX\n", OPenSsl_GetLastError());
		return -1;
	}
	XNETHANDLE xhToken = 0;
	if (!NetCore_TCPSelect_StartEx(&xhToken, 55443))
	{
		printf("NetCore_TCPIocp_StartEx %lX\n", OPenSsl_GetLastError());
		return -1;
	}
	NetCore_TCPSelect_SetCallBackEx(xhToken, TCPSelect_CBLogin, TCPSelect_CBRecv, TCPSelect_CBLeave);

	printf("ok\n");
	while (1)
	{
		Sleep(1000);
	}

	NetCore_TCPSelect_StopEx(xhToken);
	WSACleanup();
	return 0;
}