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
#include <thread>
using namespace std;
#if 1 == _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/NetCore_Define.h>
#include <XEngine_Include/XEngine_Core/NetCore_Error.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/HttpProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/HttpProtocol_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_Core/XEngine_Core.lib")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_HttpProtocol.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Core/NetCore_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Core/NetCore_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_ManagePool/ManagePool_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_HttpProtocol/HttpProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_HttpProtocol/HttpProtocol_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Core.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_HttpProtocol.lib")
#endif
#endif

//Linux::g++ -std=c++20 -Wall -g RfcComponents_APPHttp.cpp -o RfcComponents_APPHttp.exe -lXEngine_BaseLib -lXEngine_Core -lRfcComponents_HttpProtocol -lpthread

bool bIsRun = false;
int nRVMode = 0;
XHANDLE xhToken = NULL;
XHANDLE xhHttp = NULL;
FILE* pSt_File = NULL;

bool CALLBACK NetCore_CB_Login(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	printf("NetCore_CB_Login:%s\n", lpszClientAddr);
	HttpProtocol_Server_CreateClientEx(xhHttp, lpszClientAddr, 0);
	HttpProtocol_Server_SetRecvModeEx(xhHttp, lpszClientAddr, nRVMode);
	return true;
}
void CALLBACK NetCore_CB_Recv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	printf("NetCore_CB_Recv:%s-%d\n", lpszClientAddr, nMsgLen);
	if (!HttpProtocol_Server_InserQueueEx(xhHttp, lpszClientAddr, lpszRecvMsg, nMsgLen))
	{
		printf("RfcComponents_WSPacket_Post:%lX\n", HttpProtocol_GetLastError());
	}
}
void CALLBACK NetCore_CB_Close(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	printf("NetCore_CB_Close:%s\n", lpszClientAddr);
	HttpProtocol_Server_CloseClinetEx(xhHttp, lpszClientAddr);
}

XHTHREAD CALLBACK NetCore_Thread()
{
	while (bIsRun)
	{
		if (HttpProtocol_Server_EventWaitEx(xhHttp, 1))
		{
			int nListCount = 0;
			XENGINE_MANAGEPOOL_TASKEVENT** ppSt_ListClient;
			if (HttpProtocol_Server_GetPoolEx(xhHttp, 1, &ppSt_ListClient, &nListCount))
			{
				for (int i = 0; i < nListCount; i++)
				{
					int nMsgLen = 0;
					XCHAR* ptszMsgBuffer = NULL;
					RFCCOMPONENTS_HTTP_REQPARAM st_ReqParam;

					memset(&st_ReqParam, '\0', sizeof(RFCCOMPONENTS_HTTP_REQPARAM));
					if (HttpProtocol_Server_GetMemoryEx(xhHttp, ppSt_ListClient[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_ReqParam))
					{
						int nRVMode = 0;
						int nRVCount = 0;
						int nHDSize = 0;
						HttpProtocol_Server_GetRecvModeEx(xhHttp, ppSt_ListClient[i]->tszClientAddr, &nRVMode, &nRVCount, &nHDSize);
						if (1 == nRVMode)
						{
							printf("count:%d,recv:%d\n", nRVCount, nHDSize);
							fwrite(ptszMsgBuffer, 1, nMsgLen, pSt_File);
							if (nHDSize >= nRVCount)
							{
								RFCCOMPONENTS_HTTP_HDRPARAM st_HdrParam;
								memset(&st_HdrParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

								st_HdrParam.bIsClose = true;
								st_HdrParam.nHttpCode = 200;
								nMsgLen = 2048;
								HttpProtocol_Server_SendMsgEx(xhHttp, ptszMsgBuffer, &nMsgLen, &st_HdrParam);
								NetCore_TCPXCore_SendEx(xhToken, ppSt_ListClient[i]->tszClientAddr, ptszMsgBuffer, nMsgLen);

								fclose(pSt_File);
								break;
							}
						}
						else
						{
							//127.0.0.1:8080
							printf("%s %d:%s\n", ppSt_ListClient[i]->tszClientAddr, nMsgLen, ptszMsgBuffer);
							XCHAR tszMsgBuffer[1024];
							RFCCOMPONENTS_HTTP_HDRPARAM st_HdrParam;

							memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
							memset(&st_HdrParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

							st_HdrParam.bIsClose = true;
							st_HdrParam.nHttpCode = 200;
							nMsgLen = 2048;
							HttpProtocol_Server_SendMsgEx(xhHttp, tszMsgBuffer, &nMsgLen, &st_HdrParam, "123456789", 9);
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
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);

	LPCXSTR lpszWFile = _X("D:\\xengine_apps\\Debug\\2.png");
	LPCXSTR lpszMiniFile = _X("D:\\xengine_apps\\Debug\\HttpMime.types");
	LPCXSTR lpszCodeFile = _X("D:\\xengine_apps\\Debug\\HttpCode.types");
#else
	LPCXSTR lpszWFile = _X("2.png");
	LPCXSTR lpszMiniFile = _X("HttpMime.types");
	LPCXSTR lpszCodeFile = _X("HttpCode.types");
#endif
	LPCXSTR lpszUrl = _X("http://bbs.xyry.org/forum.php?mod=viewthread&tid=2&extra=page%3D1");
	int nListCount = 0;
	XCHAR** pptszListParam;
	XCHAR tszUrlAddr[MAX_PATH];
	HttpProtocol_ServerHelp_GetParament(lpszUrl, &pptszListParam, &nListCount, tszUrlAddr);
	for (int i = 0; i < nListCount; i++)
	{
		printf("%s\n", pptszListParam[i]);
	}
	BaseLib_Memory_Free((XPPPMEM)&pptszListParam, nListCount);

	for (int i = 0; i < 10; i++)
	{
		XCHAR tszDomainUrl[MAX_PATH];
		XCHAR tszDomainKey[MAX_PATH];

		memset(tszDomainUrl, '\0', MAX_PATH);
		memset(tszDomainKey, '\0', MAX_PATH);
		HttpProtocol_ServerHelp_ShortLink(lpszUrl, tszDomainUrl, tszDomainKey);
		printf("%s %s\n", tszDomainUrl, tszDomainKey);
	}

	bIsRun = true;
	if (1 == nRVMode)
	{
		pSt_File = fopen(lpszWFile, _X("wb"));
		if (NULL == pSt_File)
		{
			printf("errno open file\n");
			return 0;
		}
	}
	xhHttp = HttpProtocol_Server_InitEx(lpszCodeFile, lpszMiniFile, 1);
	if (NULL == xhHttp)
	{
		printf("HttpProtocol_Server_InitEx:%lX\n", HttpProtocol_GetLastError());
		return 0;
	}
	xhToken = NetCore_TCPXCore_StartEx(8080);
	if (NULL == xhToken)
	{
		printf("NetCore_TCPXCore_StartEx:%lX\n", NetCore_GetLastError());
		return 0;
	}

	NetCore_TCPXCore_RegisterCallBackEx(xhToken, NetCore_CB_Login, NetCore_CB_Recv, NetCore_CB_Close);
	std::thread pSTDThread(NetCore_Thread);

	std::this_thread::sleep_for(std::chrono::seconds(600));
	bIsRun = false;
	HttpProtocol_Server_DestroyEx(xhHttp);
	pSTDThread.join();

	std::this_thread::sleep_for(std::chrono::seconds(1));
	NetCore_TCPXCore_DestroyEx(xhToken);
	

#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}