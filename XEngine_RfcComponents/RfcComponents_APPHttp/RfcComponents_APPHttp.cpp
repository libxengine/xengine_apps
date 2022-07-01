#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Core.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_HttpServer.lib")
#else
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_Core/NetCore_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_Core/NetCore_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_HttpServer/HttpServer_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_HttpServer/HttpServer_Error.h"

//Linux::g++ -std=c++17 -Wall -g RfcComponents_APPHttp.cpp -o RfcComponents_APPHttp.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents -lXEngine_BaseLib -lXEngine_Core -lXEngine_OPenSsl -lRfcComponents_HttpServer -lpthread -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g RfcComponents_APPHttp.cpp -o RfcComponents_APPHttp.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_Core -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_RfcComponents -lXEngine_BaseLib -lXEngine_Core -lXEngine_OPenSsl -lRfcComponents_HttpServer -lpthread

BOOL bIsRun = FALSE;
XNETHANDLE xhToken;
int nRVMode = 0;
XHANDLE xhHttp = NULL;
FILE* pSt_File = NULL;

BOOL CALLBACK NetCore_CB_Login(LPCSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	printf("NetCore_CB_Login:%s\n", lpszClientAddr);
	RfcComponents_HttpServer_CreateClientEx(xhHttp, lpszClientAddr, 0);
	RfcComponents_HttpServer_SetRecvModeEx(xhHttp, lpszClientAddr, nRVMode);
	return TRUE;
}
void CALLBACK NetCore_CB_Recv(LPCSTR lpszClientAddr, SOCKET hSocket, LPCSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	printf("NetCore_CB_Recv:%s-%d\n", lpszClientAddr, nMsgLen);
	if (!RfcComponents_HttpServer_InserQueueEx(xhHttp, lpszClientAddr, lpszRecvMsg, nMsgLen))
	{
		printf("RfcComponents_WSPacket_Post:%lX\n", HttpServer_GetLastError());
	}
}
void CALLBACK NetCore_CB_Close(LPCSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	printf("NetCore_CB_Close:%s\n", lpszClientAddr);
	RfcComponents_HttpServer_CloseClinetEx(xhHttp, lpszClientAddr);
}

XHTHREAD CALLBACK NetCore_Thread()
{
	while (bIsRun)
	{
		if (RfcComponents_HttpServer_EventWaitEx(xhHttp, 1))
		{
			int nListCount = 0;
			RFCCOMPONENTS_HTTP_PKTCLIENT** ppSt_ListClient;
			if (RfcComponents_HttpServer_GetPoolEx(xhHttp, 1, &ppSt_ListClient, &nListCount))
			{
				for (int i = 0; i < nListCount; i++)
				{
					int nMsgLen = 0;
					TCHAR* ptszMsgBuffer = NULL;
					RFCCOMPONENTS_HTTP_REQPARAM st_ReqParam;

					memset(&st_ReqParam, '\0', sizeof(RFCCOMPONENTS_HTTP_REQPARAM));
					if (RfcComponents_HttpServer_GetMemoryEx(xhHttp, ppSt_ListClient[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_ReqParam))
					{
						int nRVMode = 0;
						int nRVCount = 0;
						int nHDSize = 0;
						RfcComponents_HttpServer_GetRecvModeEx(xhHttp, ppSt_ListClient[i]->tszClientAddr, &nRVMode, &nRVCount, &nHDSize);
						if (1 == nRVMode)
						{
							printf("count:%d,recv:%d\n", nRVCount, nHDSize);
							fwrite(ptszMsgBuffer, 1, nMsgLen, pSt_File);
							if (nHDSize >= nRVCount)
							{
								RFCCOMPONENTS_HTTP_HDRPARAM st_HdrParam;
								memset(&st_HdrParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

								st_HdrParam.bIsClose = TRUE;
								st_HdrParam.nHttpCode = 200;
								nMsgLen = 2048;
								RfcComponents_HttpServer_SendMsgEx(xhHttp, ptszMsgBuffer, &nMsgLen, &st_HdrParam);
								NetCore_TCPXCore_SendEx(xhToken, ppSt_ListClient[i]->tszClientAddr, ptszMsgBuffer, nMsgLen);

								fclose(pSt_File);
								break;
							}
						}
						else
						{
							printf("%s %d:%s\n", ppSt_ListClient[i]->tszClientAddr, nMsgLen, ptszMsgBuffer);
							TCHAR tszMsgBuffer[1024];
							RFCCOMPONENTS_HTTP_HDRPARAM st_HdrParam;

							memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
							memset(&st_HdrParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

							st_HdrParam.bIsClose = TRUE;
							st_HdrParam.nHttpCode = 200;
							nMsgLen = 2048;
							RfcComponents_HttpServer_SendMsgEx(xhHttp, tszMsgBuffer, &nMsgLen, &st_HdrParam, "123456789", 9);
							NetCore_TCPXCore_SendEx(xhToken, ppSt_ListClient[i]->tszClientAddr, tszMsgBuffer, nMsgLen);
						}
					}
				}
			}
		}
	}
	printf("exit\n");
	return 0;
}

int main()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);

	LPCTSTR lpszWFile = _T("D:\\xengine_apps\\Debug\\2.png");
	LPCTSTR lpszMiniFile = _T("D:\\xengine_apps\\Debug\\HttpMime.types");
	LPCTSTR lpszCodeFile = _T("D:\\xengine_apps\\Debug\\HttpCode.types");
#else
	LPCTSTR lpszWFile = _T("2.png");
	LPCTSTR lpszMiniFile = _T("HttpMime.types");
	LPCTSTR lpszCodeFile = _T("HttpCode.types");
#endif
	LPCTSTR lpszUrl = _T("http://bbs.xyry.org/forum.php?mod=viewthread&tid=2&extra=page%3D1");
	int nListCount = 0;
	TCHAR** pptszListParam;
	TCHAR tszUrlAddr[MAX_PATH];
	RfcComponents_HttpHelp_GetParament(lpszUrl, &pptszListParam, &nListCount, tszUrlAddr);
	for (int i = 0; i < nListCount; i++)
	{
		printf("%s\n", pptszListParam[i]);
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&pptszListParam, nListCount);

	bIsRun = TRUE;
	if (1 == nRVMode)
	{
		pSt_File = fopen(lpszWFile, _T("wb"));
		if (NULL == pSt_File)
		{
			printf("errno open file\n");
			return 0;
		}
	}
	xhHttp = RfcComponents_HttpServer_InitEx(lpszCodeFile, lpszMiniFile, 1);
	if (NULL == xhHttp)
	{
		printf("RfcComponents_HttpServer_InitEx:%lX\n", HttpServer_GetLastError());
		return 0;
	}
	if (!NetCore_TCPXCore_StartEx(&xhToken, 8080))
	{
		printf("NetCore_TCPXCore_StartEx:%lX\n", NetCore_GetLastError());
		return 0;
	}
	RFCCOMPONENTS_HTTP_REGPROCESS st_Process;
	memset(&st_Process, '\0', sizeof(RFCCOMPONENTS_HTTP_REGPROCESS));

	st_Process.bAddr = TRUE;
	st_Process.bBody = TRUE;
	st_Process.bHdr = TRUE;
	st_Process.bMethod = TRUE;
	st_Process.bUrl = TRUE;
	//注册名称api的内容到程序中
#ifdef _WINDOWS
	RfcComponents_HttpExec_RegisterProcess(_T("api"), _T("D:\\xengine_apps\\Debug\\RfcComponents_APPHttpModule.dll"), &st_Process, TRUE);
#else
	RfcComponents_HttpExec_RegisterProcess(_T("api"), _T("../RfcComponents_APPHttpExec/RfcComponents_APPHttpExec.exe"), &st_Process, FALSE);
#endif

	NetCore_TCPXCore_RegisterCallBackEx(xhToken, NetCore_CB_Login, NetCore_CB_Recv, NetCore_CB_Close);
	std::thread pSTDThread(NetCore_Thread);

	std::this_thread::sleep_for(std::chrono::seconds(10));
	bIsRun = FALSE;
	RfcComponents_HttpServer_DestroyEx(xhHttp);
	pSTDThread.join();

	std::this_thread::sleep_for(std::chrono::seconds(1));
	NetCore_TCPXCore_DestroyEx(xhToken);
	

#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}