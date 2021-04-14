#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/NetHelp_XRpc.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_XRpc/XRPC_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_XRpc/XRPC_Error.h"

//g++ -std=c++17 -Wall -g NetHelp_APPRPCClient.cpp -o NetHelp_APPRPCClient.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_HelpComponents -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp -lXEngine_BaseLib -lXEngine_Core -lXClient_Socket -lHelpComponents_Packets -lNetHelp_XRpc -ljsoncpp -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_HelpComponents:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp,--disable-new-dtags


XNETHANDLE xhToken;
int XEngine_AddMethod(int a, int b)
{
	NETHELP_XRPCPARAMETE st_XRPCParamete;
	NETHELP_XRPCPARAMETE** ppSt_XRpcParament;

	memset(&st_XRPCParamete, '\0', sizeof(NETHELP_XRPCPARAMETE));

	st_XRPCParamete.lParameteValue = malloc(100);
	memset(st_XRPCParamete.lParameteValue, '\0', 100);

	NetHelp_XRPCHelp_Malloc(&ppSt_XRpcParament, 2);

	ppSt_XRpcParament[0]->enXRPC_ParameteType = ENUM_NETHELP_XRPC_PARAMETE_TYPE_INT;
	ppSt_XRpcParament[0]->nParameteLen = sizeof(int);
	memcpy(ppSt_XRpcParament[0]->lParameteValue, &a, sizeof(int));

	ppSt_XRpcParament[1]->enXRPC_ParameteType = ENUM_NETHELP_XRPC_PARAMETE_TYPE_INT;
	ppSt_XRpcParament[1]->nParameteLen = sizeof(int);
	memcpy(ppSt_XRpcParament[1]->lParameteValue, &b, sizeof(int));

	LPCTSTR lpszFuncName = _T("XEngine_AddMethod");
	if (!NetHelp_XRPCClient_ReqFuncCall(xhToken, lpszFuncName, ENUM_NETHELP_XRPC_PARAMETE_TYPE_INT, &ppSt_XRpcParament, 2, &st_XRPCParamete))
	{
		printf("NetHelp_XRPCClient_ReqFuncCall1:%lX\n", XRpc_GetLastError());
		return 0;
	}
	NetHelp_XRPCHelp_Free(&ppSt_XRpcParament, 2);

	int nRet = 0;
	memcpy(&nRet, st_XRPCParamete.lParameteValue, sizeof(int));
	printf("%d\n", nRet);
	return nRet;
}
void XEngine_Print(LPCTSTR lpszString)
{
	NETHELP_XRPCPARAMETE** ppSt_XRpcParament;
	NetHelp_XRPCHelp_Malloc(&ppSt_XRpcParament, 1);

	ppSt_XRpcParament[0]->enXRPC_ParameteType = ENUM_NETHELP_XRPC_PARAMETE_TYPE_STRING;
	ppSt_XRpcParament[0]->nParameteLen = strlen(lpszString);
	memcpy(ppSt_XRpcParament[0]->lParameteValue, lpszString, ppSt_XRpcParament[0]->nParameteLen);

	LPCTSTR lpszFuncName = _T("XEngine_Print");
	if (!NetHelp_XRPCClient_ReqFuncCall(xhToken, lpszFuncName, ENUM_NETHELP_XRPC_PARAMETE_TYPE_VOID, &ppSt_XRpcParament, 1))
	{
		printf("NetHelp_XRPCClient_ReqFuncCall2:%lX\n", XRpc_GetLastError());
		return;
	}
	NetHelp_XRPCHelp_Free(&ppSt_XRpcParament, 1);
}
int XEngine_HttpAdd(int a, int b)
{
	int nCode = 0;
	int nLen = 1024;
	TCHAR tszMsgBuffer[1024];
	TCHAR tszFileBuffer[1024];
	LPCTSTR lpszHttpAddr = _T("http://127.0.0.1:50002/XRPCApi/FuncCall");
	LPCTSTR lpszJsonFile = _T("G:\\netengineapp\\NetEngine_WINApps\\Debug\\1.txt");

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(tszFileBuffer, '\0', sizeof(tszFileBuffer));

	FILE* pSt_File = fopen(lpszJsonFile, "rb");
	int nRet = fread(tszFileBuffer, 1, sizeof(tszFileBuffer), pSt_File);
	fclose(pSt_File);

	//APIHelp_HttpRequest_Post(lpszHttpAddr, &nCode, tszFileBuffer, tszMsgBuffer, &nLen);
	printf("%s\n", tszMsgBuffer);
	return nRet;
}

int main()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

	LPCTSTR lpszAddr = _T("192.168.1.4");
	if (!NetHelp_XRPCClient_Connect(&xhToken, lpszAddr, 50001))
	{
		printf("NetHelp_XRPCClient_Connect\n");
		return -1;
	}

	printf("XEngine_AddMethod(1, 2) = %d\n", XEngine_AddMethod(1, 2));
	XEngine_Print("asdasdasd");
	printf("XEngine_Print\n");

	//XEngine_HttpAdd(1, 2);
	printf("wandan\n");

#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}