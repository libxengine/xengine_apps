#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
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
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_Core/NetCore_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_Core/NetCore_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_HttpServer/HttpServer_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_HttpServer/HttpServer_Error.h"

//g++ -std=c++17 -Wall -g RfcComponents_APPHttp2.cpp -o RfcComponents_APPHttp2.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents -lXEngine_BaseLib -lXEngine_Core -lRfcComponents_HttpServer -lpthread -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk,--disable-new-dtags


BOOL bIsRun = FALSE;
XNETHANDLE xhToken;
XHANDLE xhHttp = NULL;
FILE* pSt_File = NULL;

BOOL CALLBACK NetCore_CB_Login(LPCSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	printf("NetCore_CB_Login:%s\n", lpszClientAddr);
	RfcComponents_Http2Server_CreateClientEx(xhHttp, lpszClientAddr);
	return TRUE;
}
void CALLBACK NetCore_CB_Recv(LPCSTR lpszClientAddr, SOCKET hSocket, LPCSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	printf("NetCore_CB_Recv:%s-%d\n", lpszClientAddr, nMsgLen);
	if (!RfcComponents_Http2Server_InsertQueueEx(xhHttp, lpszClientAddr, lpszRecvMsg, nMsgLen))
	{
		printf("RfcComponents_Http2Server_InserQueueEx:%lX\n", HttpServer_GetLastError());
	}
}
void CALLBACK NetCore_CB_Close(LPCSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	printf("NetCore_CB_Close:%s\n", lpszClientAddr);
	RfcComponents_Http2Server_CloseClientEx(xhHttp, lpszClientAddr);
}

int test_localstream()
{
	RfcComponents_Http2Server_CreateClientEx(xhHttp, "123123");

	TCHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	//////////////////////////////////////////////////////////////////////////1
#ifdef _WINDOWS
	pSt_File = fopen("D:\\xengine_apps\\Debug\\1.bin", "rb");
#else
	pSt_File = fopen("1.bin", "rb");
#endif
	int nRet = fread(tszMsgBuffer, 1, sizeof(tszMsgBuffer), pSt_File);
	fclose(pSt_File);
	if (!RfcComponents_Http2Server_InsertQueueEx(xhHttp, "123123", tszMsgBuffer, nRet))
	{
		printf("RfcComponents_Http2Server_InserQueueEx:%lX\n", HttpServer_GetLastError());
	}
	//////////////////////////////////////////////////////////////////////////2
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
#ifdef _WINDOWS
	pSt_File = fopen("D:\\xengine_apps\\Debug\\2.bin", "rb");
#else
	pSt_File = fopen("2.bin", "rb");
#endif
	nRet = fread(tszMsgBuffer, 1, sizeof(tszMsgBuffer), pSt_File);
	fclose(pSt_File);
	if (!RfcComponents_Http2Server_InsertQueueEx(xhHttp, "123123", tszMsgBuffer, nRet))
	{
		printf("RfcComponents_Http2Server_InserQueueEx:%lX\n", HttpServer_GetLastError());
	}
	//////////////////////////////////////////////////////////////////////////3
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
#ifdef _WINDOWS
	pSt_File = fopen("D:\\xengine_apps\\Debug\\3.bin", "rb");
#else
	pSt_File = fopen("3.bin", "rb");
#endif
	nRet = fread(tszMsgBuffer, 1, sizeof(tszMsgBuffer), pSt_File);
	fclose(pSt_File);
	if (!RfcComponents_Http2Server_InsertQueueEx(xhHttp, "123123", tszMsgBuffer, nRet))
	{
		printf("RfcComponents_Http2Server_InserQueueEx:%lX\n", HttpServer_GetLastError());
	}
	//////////////////////////////////////////////////////////////////////////4
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
#ifdef _WINDOWS
	pSt_File = fopen("D:\\xengine_apps\\Debug\\4.bin", "rb");
#else
	pSt_File = fopen("4.bin", "rb");
#endif
	nRet = fread(tszMsgBuffer, 1, sizeof(tszMsgBuffer), pSt_File);
	if (!RfcComponents_Http2Server_InsertQueueEx(xhHttp, "123123", tszMsgBuffer, nRet))
	{
		printf("RfcComponents_Http2Server_InserQueueEx:%lX\n", HttpServer_GetLastError());
	}
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	nRet = fread(tszMsgBuffer, 1, sizeof(tszMsgBuffer), pSt_File);
	fclose(pSt_File);
	if (!RfcComponents_Http2Server_InsertQueueEx(xhHttp, "123123", tszMsgBuffer, nRet))
	{
		printf("RfcComponents_Http2Server_InserQueueEx:%lX\n", HttpServer_GetLastError());
	}
	//////////////////////////////////////////////////////////////////////////5
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
#ifdef _WINDOWS
	pSt_File = fopen("D:\\xengine_apps\\Debug\\5.bin", "rb");
#else
	pSt_File = fopen("5.bin", "rb");
#endif
	nRet = fread(tszMsgBuffer, 1, sizeof(tszMsgBuffer), pSt_File);
	if (!RfcComponents_Http2Server_InsertQueueEx(xhHttp, "123123", tszMsgBuffer, nRet))
	{
		printf("RfcComponents_Http2Server_InserQueueEx:%lX\n", HttpServer_GetLastError());
	}

	while (1)
	{
		if (!RfcComponents_Http2Server_EventWaitEx(xhHttp, 1, 1000))
		{
			break;
		}
		int nClientCount = 0;
		RFCCOMPONENTS_HTTP_PKTCLIENT** ppSt_ListClient;
		RfcComponents_Http2Server_GetPoolEx(xhHttp, 1, &ppSt_ListClient, &nClientCount);
		for (int i = 0; i < nClientCount; i++)
		{
			int nStreamCount = 0;
			RFCCOMPONENTS_HTTP2_PKTSTREAM** ppSt_PKTStream;
			RfcComponents_Http2Server_GetStreamEx(xhHttp, ppSt_ListClient[i]->tszClientAddr, &ppSt_PKTStream, &nStreamCount);
			for (int j = 0; j < nStreamCount; j++)
			{
				for (int k = 0; k < ppSt_PKTStream[j]->nStreamCount; k++)
				{
					int nMsgLen = 4096;
					int nHdrCount = 0;
					TCHAR* ptszMsgBuffer;
					XENGINE_RFCCOMPONENTS_HTTP2_FRAME_TYPE enFrameType;
					RFCCOMPONENTS_HTTP2_HPACK** ppSt_ListHdr;
					if (RfcComponents_Http2Server_GetClientEx(xhHttp, ppSt_ListClient[i]->tszClientAddr, ppSt_PKTStream[j]->nStreamID, &enFrameType, &ptszMsgBuffer, &nMsgLen, &ppSt_ListHdr, &nHdrCount))
					{
						printf("%s\n", ptszMsgBuffer);
						BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
						BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListHdr, nHdrCount);
					}
				}
			}
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_PKTStream, nStreamCount);
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClient, nClientCount);
	}
	RfcComponents_Http2Server_CloseClientEx(xhHttp, "123123");
	RfcComponents_Http2Server_DestroyEx(xhHttp);
	return 0;
}
int main()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);

	LPCTSTR lpszPackFile = _T("D:\\xengine_apps\\Debug\\Http2Pack.types");
#else
	LPCTSTR lpszPackFile = _T("Http2Pack.types");
#endif
	bIsRun = TRUE;

	RfcComponents_HttpConfig_InitPack(lpszPackFile);
	xhHttp = RfcComponents_Http2Server_InitEx(1);
	if (NULL == xhHttp)
	{
		printf("%lX\n", HttpServer_GetLastError());
		return 0;
	}
	return test_localstream();

	if (!NetCore_TCPXCore_StartEx(&xhToken))
	{
		printf("%lX\n", NetCore_GetLastError());
		return 0;
	}
	NetCore_TCPXCore_RegisterCallBackEx(xhToken, NetCore_CB_Login, NetCore_CB_Recv, NetCore_CB_Close);

	std::this_thread::sleep_for(std::chrono::seconds(50000));
	bIsRun = FALSE;
	RfcComponents_HttpServer_DestroyEx(xhHttp);

	std::this_thread::sleep_for(std::chrono::seconds(1));
	NetCore_TCPXCore_DestroyEx(xhToken);


#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}