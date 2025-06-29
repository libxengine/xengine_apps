﻿#include "../../XEngine_ExampleHdr.h"
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
#include <XEngine_Include/XEngine_Core/Cryption_Define.h>
#include <XEngine_Include/XEngine_Core/Cryption_Error.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/HttpProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/HttpProtocol_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_Core/XEngine_Core.lib")
#pragma comment(lib,"XEngine_Core/XEngine_Cryption.lib")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_HttpProtocol.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Core/NetCore_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Core/NetCore_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Cryption/Cryption_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Cryption/Cryption_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_ManagePool/ManagePool_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_HttpProtocol/HttpProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_HttpProtocol/HttpProtocol_Error.h"
#ifdef _MSC_BUILD
#ifdef _WIN64
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_Core.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_Cryption.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/RfcComponents_HttpProtocol.lib")
#else
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Core.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Cryption.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_HttpProtocol.lib")
#endif
#endif
#endif
//Linux::g++ -std=c++20 -Wall -g RfcComponents_APPHttp2.cpp -o RfcComponents_APPHttp2.exe -lXEngine_BaseLib -lXEngine_Core -lXEngine_Cryption -lRfcComponents_HttpProtocol -lpthread

bool bSsl = false;
bool bIsRun = false;
XHANDLE xhToken = NULL;
XHANDLE xhHttp = NULL;
XHANDLE xhSsl = NULL;
FILE* pSt_File = NULL;

bool XCALLBACK NetCore_CB_Login(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	XCHAR tszSubject[XPATH_MAX];
	XCHAR tszIssuer[XPATH_MAX];
	XCHAR tszAlgorithm[XPATH_MAX];

	memset(tszSubject, '\0', XPATH_MAX);
	memset(tszIssuer, '\0', XPATH_MAX);
	memset(tszAlgorithm, '\0', XPATH_MAX);

	if (bSsl)
	{
		Cryption_Server_AcceptEx(xhSsl, hSocket, lpszClientAddr);
	}
	printf("NetCore_CB_Login:%s\n", lpszClientAddr);
	HttpProtocol_Server2_CreateClientEx(xhHttp, lpszClientAddr, 1);
	return true;
}
void XCALLBACK NetCore_CB_Recv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	int nRVLen = 2048;
	XCHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	if (bSsl)
	{
		Cryption_Server_RecvMsgEx(xhSsl, lpszClientAddr, tszMsgBuffer, &nRVLen, lpszRecvMsg, nMsgLen);
		if (!HttpProtocol_Server2_InsertQueueEx(xhHttp, lpszClientAddr, tszMsgBuffer, nRVLen))
		{
			printf("HttpProtocol_Server2_InserQueueEx:%lX\n", HttpProtocol_GetLastError());
		}
	}
	else
	{
		if (!HttpProtocol_Server2_InsertQueueEx(xhHttp, lpszClientAddr, lpszRecvMsg, nMsgLen))
		{
			printf("HttpProtocol_Server2_InserQueueEx:%lX\n", HttpProtocol_GetLastError());
		}
	}
}
void XCALLBACK NetCore_CB_Close(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	printf("NetCore_CB_Close:%s\n", lpszClientAddr);
	if (bSsl)
	{
		Cryption_Server_CloseClientEx(xhSsl, lpszClientAddr);
	}
	HttpProtocol_Server2_CloseClientEx(xhHttp, lpszClientAddr);
}

XHTHREAD NetCore_Thread()
{
	while (bIsRun)
	{
		if (HttpProtocol_Server2_EventWaitEx(xhHttp, 1))
		{
			int nListCount = 0;
			XENGINE_MANAGEPOOL_TASKEVENT** ppSt_ListClient;
			if (HttpProtocol_Server2_GetPoolEx(xhHttp, 1, &ppSt_ListClient, &nListCount))
			{
				for (int i = 0; i < nListCount; i++)
				{
					int nStreamCount = 0;
					RFCCOMPONENTS_HTTP2_PKTSTREAM** ppSt_PKTStream;
					HttpProtocol_Server2_GetStreamEx(xhHttp, ppSt_ListClient[i]->tszClientAddr, &ppSt_PKTStream, &nStreamCount);
					for (int j = 0; j < nStreamCount; j++)
					{
						for (int k = 0; k < ppSt_PKTStream[j]->nPktCount; k++)
						{
							int nMsgLen = 4096;
							int nHdrCount = 0;
							XCHAR* ptszMsgBuffer;
							XENGINE_RFCCOMPONENTS_HTTP2_FRAME_TYPE enFrameType;
							RFCCOMPONENTS_HTTP2_HPACK** ppSt_ListHdr;
							if (HttpProtocol_Server2_GetClientEx(xhHttp, ppSt_ListClient[i]->tszClientAddr, ppSt_PKTStream[j]->nStreamID, &enFrameType, &ptszMsgBuffer, &nMsgLen, &ppSt_ListHdr, &nHdrCount))
							{
								if (XENGINE_RFCCOMPONENTS_HTTP2_FRAME_TYPE_HEADERS == enFrameType)
								{
									//header 如果是POST,都跟上数据,get可能不带数据
									RFCCOMPONENTS_HTTP_REQPARAM st_HTTPRequest;
									memset(&st_HTTPRequest, '\0', sizeof(RFCCOMPONENTS_HTTP_REQPARAM));

									HttpProtocol_ServerHelp_HTTP2HdrConvert(&st_HTTPRequest, &ppSt_ListHdr, nHdrCount);
									if (0 == _tcsnicmp("POST", st_HTTPRequest.tszHttpMethod, 4))
									{
										//只有POST才有后续数据
										HttpProtocol_Server2_GetClientEx(xhHttp, ppSt_ListClient[i]->tszClientAddr, ppSt_PKTStream[j]->nStreamID, &enFrameType, &ptszMsgBuffer, &nMsgLen);
										ppSt_PKTStream[j]->nPktCount--;
									}
								}
								int nSSLLen = 2048;
								int nSDLen = 2048;
								XCHAR tszSSLBuffer[2048];
								XCHAR tszSDBuffer[2048];
								RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;

								memset(tszSSLBuffer, '\0', sizeof(tszSSLBuffer));
								memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
								memset(&st_HDRParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

								if (1)
								{
									//第一次必须打包setting,否则无法继续通信.后面不用在发送
									HttpProtocol_Server2_PKTSettingEx(xhHttp, tszSDBuffer, &nSDLen, 100, 1024000, 1024000, 8196);
									if (bSsl)
									{
										Cryption_Server_SendMsgEx(xhSsl, ppSt_ListClient[i]->tszClientAddr, tszSDBuffer, nSDLen, tszSSLBuffer, &nSSLLen);
										NetCore_TCPXCore_SendEx(xhToken, ppSt_ListClient[i]->tszClientAddr, tszSSLBuffer, nSSLLen);
									}
									else
									{
										NetCore_TCPXCore_SendEx(xhToken, ppSt_ListClient[i]->tszClientAddr, tszSDBuffer, nSDLen);
									}
								}
								st_HDRParam.nHttpCode = 200;
								st_HDRParam.nStreamID = ppSt_PKTStream[j]->nStreamID;

								HttpProtocol_Server2_PKTHeaderEx(xhHttp, tszSDBuffer, &nSDLen, &st_HDRParam, 6);
								if (bSsl)
								{
									Cryption_Server_SendMsgEx(xhSsl, ppSt_ListClient[i]->tszClientAddr, tszSDBuffer, nSDLen, tszSSLBuffer, &nSSLLen);
									NetCore_TCPXCore_SendEx(xhToken, ppSt_ListClient[i]->tszClientAddr, tszSSLBuffer, nSSLLen);
								}
								else
								{
									NetCore_TCPXCore_SendEx(xhToken, ppSt_ListClient[i]->tszClientAddr, tszSDBuffer, nSDLen);
								}
								
								HttpProtocol_Server2_PKTDataEx(xhHttp, tszSDBuffer, &nSDLen, st_HDRParam.nStreamID, "123456", 6);
								if (bSsl)
								{
									Cryption_Server_SendMsgEx(xhSsl, ppSt_ListClient[i]->tszClientAddr, tszSDBuffer, nSDLen, tszSSLBuffer, &nSSLLen);
									NetCore_TCPXCore_SendEx(xhToken, ppSt_ListClient[i]->tszClientAddr, tszSSLBuffer, nSSLLen);
								}
								else
								{
									NetCore_TCPXCore_SendEx(xhToken, ppSt_ListClient[i]->tszClientAddr, tszSDBuffer, nSDLen);
								}
							}
							if (nMsgLen > 0)
							{
								printf("%s\n", ptszMsgBuffer);
								BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
							}
							if (nHdrCount > 0)
							{
								BaseLib_Memory_Free((XPPPMEM)&ppSt_ListHdr, nHdrCount);
							}
						}
					}
					BaseLib_Memory_Free((XPPPMEM)&ppSt_PKTStream, nStreamCount);
				}
			}
			BaseLib_Memory_Free((XPPPMEM)&ppSt_ListClient, nListCount);
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

	LPCXSTR lpszCodeFile = _X("D:\\xengine_apps\\Debug\\HttpCode.types");
	LPCXSTR lpszMimeFile = _X("D:\\xengine_apps\\Debug\\HttpMime.types");
#else
	LPCXSTR lpszCodeFile = _X("HttpCode.types");
	LPCXSTR lpszMimeFile = _X("HttpMime.types");
#endif
	bIsRun = true;

	HttpProtocol_ServerConfig_InitCode(lpszCodeFile);
	HttpProtocol_ServerConfig_InitMime(lpszMimeFile, false, false);
	xhHttp = HttpProtocol_Server2_InitEx(1);
	if (NULL == xhHttp)
	{
		printf("%lX\n", HttpProtocol_GetLastError());
		return 0;
	}
	xhToken = NetCore_TCPXCore_StartEx(80);
	if (NULL == xhToken)
	{
		printf("%lX\n", NetCore_GetLastError());
		return 0;
	}
	NetCore_TCPXCore_RegisterCallBackEx(xhToken, NetCore_CB_Login, NetCore_CB_Recv, NetCore_CB_Close);

#ifdef _MSC_BUILD
	LPCXSTR lpszCAFile = _X("d:\\xengine_apps\\Debug\\root_bundle.crt");
	LPCXSTR lpszSrvFile = _X("d:\\xengine_apps\\Debug\\test.xyry.org.crt");
	LPCXSTR lpszKeyFile = _X("d:\\xengine_apps\\Debug\\test.xyry.org.key");
#else
	LPCXSTR lpszCAFile = _X("root_bundle.crt");
	LPCXSTR lpszSrvFile = _X("test.xyry.org.crt");
	LPCXSTR lpszKeyFile = _X("test.xyry.org.key");
#endif
	xhSsl = Cryption_Server_InitEx(lpszCAFile, lpszSrvFile, lpszKeyFile, false, false);
	if (NULL == xhSsl)
	{
		printf("Cryption_Server_InitEx:%lX\n", Cryption_GetLastError());
		return 0;
	}
	NetCore_Thread();
	std::this_thread::sleep_for(std::chrono::seconds(50000));
	bIsRun = false;
	NetCore_TCPXCore_DestroyEx(xhToken);
	HttpProtocol_Server2_DestroyEx(xhHttp);
	Cryption_Server_StopEx(xhSsl);

#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}