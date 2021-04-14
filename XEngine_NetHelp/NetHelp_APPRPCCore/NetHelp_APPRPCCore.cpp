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
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_XRpc/XRPC_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_XRpc/XRPC_Error.h"

//g++ -std=c++17 -Wall -g NetHelp_APPRPCCore.cpp -o NetHelp_APPRPCCore.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_HelpComponents -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp -lXEngine_BaseLib -lXEngine_Core -lXClient_Socket -lHelpComponents_Packets -lNetHelp_XRpc -ljsoncpp -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_HelpComponents:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp,--disable-new-dtags

bool bRun = false;
void __stdcall XRPCCore_Callback(LPCSTR lpszClientAddr, LPCSTR lpszFuncName, ENUM_NETHELP_XRPC_PARAMETE_TYPE nRtnType, NETHELP_XRPCPARAMETE*** pppSt_ListParamete, int nListCount, LPVOID lParam)
{
	printf("Client:%s FuncName:%s\n", lpszClientAddr, lpszFuncName);

	int nPktLen = 2048;
	int nMsgLen = 2048;
	TCHAR tszMsgBuffer[2048];
	TCHAR tszPKTBuffer[2048];

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(tszPKTBuffer, '\0', sizeof(tszPKTBuffer));

	if (0 == _tcsnicmp(lpszFuncName, _T("XEngine_AddMethod"), strlen(lpszFuncName)))
	{
		int nRet = 0;
		for (int i = 0; i < nListCount; i++)
		{
			int nValue = 0;
			memcpy(&nValue, (*pppSt_ListParamete)[i]->lParameteValue, (*pppSt_ListParamete)[i]->nParameteLen);

			nRet += nValue;
		}
		XRPCProtocol_Core_RepFuncCall(lpszFuncName, (LPCTSTR)&nRet, sizeof(int), nRtnType, tszMsgBuffer, &nMsgLen);
		XRPCProtocol_Core_Packet(tszMsgBuffer, nMsgLen, tszPKTBuffer, &nPktLen, FALSE);
		NetHelp_XRPCCore_SendMsg(lpszClientAddr, tszPKTBuffer, nPktLen);
	}
	if (0 == _tcsnicmp(lpszFuncName, _T("XEngine_Print"), strlen(lpszFuncName)))
	{
		for (int i = 0; i < nListCount; i++)
		{
			printf("XEngine_Print:%s\n", (LPCTSTR)(*pppSt_ListParamete)[i]->lParameteValue);
		}
		XRPCProtocol_Core_RepFuncCall(lpszFuncName, NULL, 0, nRtnType, tszMsgBuffer, &nMsgLen);
		XRPCProtocol_Core_Packet(tszMsgBuffer, nMsgLen, tszPKTBuffer, &nPktLen, FALSE);
		NetHelp_XRPCCore_SendMsg(lpszClientAddr, tszPKTBuffer, nPktLen);
		bRun = false;
	}
}
int main()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

	if (!NetHelp_XRPCCore_Init(50001))
	{
		printf("NetHelp_XRPCCore_Init:%lX", XRpc_GetLastError());
	}
	NetHelp_XRPCCore_SetCallback(XRPCCore_Callback);

	printf("ok\n");
	bRun = true;
	while (bRun)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	NetHelp_XRPCCore_Destory();

#ifdef _WINDOWS
	WSACleanup();
#endif
	printf("exit\n");
	return 0;
}