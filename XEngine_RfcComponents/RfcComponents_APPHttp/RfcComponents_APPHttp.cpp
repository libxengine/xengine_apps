#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Core.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_HttpServer.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_Core/NetCore_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_Core/NetCore_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_HttpServer/HttpServer_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_HttpServer/HttpServer_Error.h"

//g++ -std=c++17 -Wall -g RfcComponents_APPHttp.cpp -o RfcComponents_APPHttp.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents -lXEngine_BaseLib -lXEngine_Core -lRfcComponents_HttpServer -lpthread -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk,--disable-new-dtags


XNETHANDLE xhToken;
BOOL CALLBACK NetCore_CB_Login(LPCSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	printf("NetCore_CB_Login:%s\n", lpszClientAddr);
	RfcComponents_HttpServer_CreateClient(lpszClientAddr);
	return TRUE;
}
void CALLBACK NetCore_CB_Recv(LPCSTR lpszClientAddr, SOCKET hSocket, LPCSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	printf("NetCore_CB_Recv:%s-%d\n", lpszClientAddr, nMsgLen);
	if (!RfcComponents_HttpServer_InserQueue(lpszClientAddr, lpszRecvMsg, nMsgLen))
	{
		printf("RfcComponents_WSPacket_Post:%lX\n", HttpServer_GetLastError());
	}
}
void CALLBACK NetCore_CB_Close(LPCSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	printf("NetCore_CB_Close:%s\n", lpszClientAddr);
	RfcComponents_HttpServer_CloseClinet(lpszClientAddr);
}

XHTHREAD CALLBACK NetCore_Thread()
{
	while (TRUE)
	{
		if (RfcComponents_HttpServer_EventWait())
		{
			int nMsgLen = 20480;
			int nListCount = 0;
			TCHAR tszMsgBuffer[20480];
			TCHAR tszClientAddr[64];
			CHAR** ppszListHdr;
			RFCCOMPONENTS_HTTP_REQPARAM st_ReqParam;

			memset(&st_ReqParam, '\0', sizeof(RFCCOMPONENTS_HTTP_REQPARAM));
			memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
			memset(tszClientAddr, '\0', sizeof(tszClientAddr));

			if (RfcComponents_HttpServer_GetRandom(tszClientAddr, &ppszListHdr, &nListCount, &st_ReqParam, tszMsgBuffer, &nMsgLen))
			{
				printf("%s %d:%s\n", tszClientAddr, nMsgLen, tszMsgBuffer);
				RFCCOMPONENTS_HTTP_HDRPARAM st_HdrParam;
				memset(&st_HdrParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

				st_HdrParam.bIsClose = TRUE;
				st_HdrParam.nHttpCode = 200;
				nMsgLen = 2048;
				RfcComponents_HttpServer_SendMsg(tszMsgBuffer, &nMsgLen, &st_HdrParam);
				NetCore_TCPXCore_SendEx(xhToken, tszClientAddr, tszMsgBuffer, nMsgLen);
				break;
			}
		}
	}
	return 0;
}

int main()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

	LPCTSTR lpszMiniFile = _T("HttpMime.types");
	LPCTSTR lpszCodeFile = _T("HttpCode.types");

	if (!RfcComponents_HttpServer_Init(lpszCodeFile, lpszMiniFile))
	{
		printf("%lX\n", HttpServer_GetLastError());
		return 0;
	}
	if (!NetCore_TCPXCore_StartEx(&xhToken))
	{
		printf("%lX\n", NetCore_GetLastError());
		return 0;
	}

	NetCore_TCPXCore_RegisterCallBackEx(xhToken, NetCore_CB_Login, NetCore_CB_Recv, NetCore_CB_Close);

	std::thread pSTDThread(NetCore_Thread);
	pSTDThread.join();

	std::this_thread::sleep_for(std::chrono::seconds(1));
	NetCore_TCPXCore_DestroyEx(xhToken);
	RfcComponents_HttpServer_Destroy();

#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}