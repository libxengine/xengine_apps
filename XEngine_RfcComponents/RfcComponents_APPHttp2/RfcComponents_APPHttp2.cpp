#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Core.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_OPenSsl.lib")
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
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_OPenSsl/OPenSsl_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_OPenSsl/OPenSsl_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_HttpServer/HttpServer_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_HttpServer/HttpServer_Error.h"

//g++ -std=c++17 -Wall -g RfcComponents_APPHttp2.cpp -o RfcComponents_APPHttp2.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents -lXEngine_BaseLib -lXEngine_Core -lRfcComponents_HttpServer -lpthread -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk,--disable-new-dtags

BOOL bSsl = FALSE;
BOOL bIsRun = FALSE;
XNETHANDLE xhToken;
XHANDLE xhHttp = NULL;
XHANDLE xhSsl = NULL;
FILE* pSt_File = NULL;

BOOL CALLBACK NetCore_CB_Login(LPCSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	TCHAR tszSubject[MAX_PATH];
	TCHAR tszIssuer[MAX_PATH];
	TCHAR tszAlgorithm[MAX_PATH];

	memset(tszSubject, '\0', MAX_PATH);
	memset(tszIssuer, '\0', MAX_PATH);
	memset(tszAlgorithm, '\0', MAX_PATH);

	if (bSsl)
	{
		OPenSsl_Server_AcceptEx(xhSsl, hSocket, lpszClientAddr, tszSubject, tszIssuer, tszAlgorithm);
	}
	printf("NetCore_CB_Login:%s\n", lpszClientAddr);
	RfcComponents_Http2Server_CreateClientEx(xhHttp, lpszClientAddr, 1);
	return TRUE;
}
void CALLBACK NetCore_CB_Recv(LPCSTR lpszClientAddr, SOCKET hSocket, LPCSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	int nRVLen = 2048;
	TCHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	if (bSsl)
	{
		OPenSsl_Server_RecvMsgEx(xhSsl, lpszClientAddr, tszMsgBuffer, &nRVLen, lpszRecvMsg, nMsgLen);
		if (!RfcComponents_Http2Server_InsertQueueEx(xhHttp, lpszClientAddr, tszMsgBuffer, nRVLen))
		{
			printf("RfcComponents_Http2Server_InserQueueEx:%lX\n", HttpServer_GetLastError());
		}
	}
	else
	{
		if (!RfcComponents_Http2Server_InsertQueueEx(xhHttp, lpszClientAddr, lpszRecvMsg, nMsgLen))
		{
			printf("RfcComponents_Http2Server_InserQueueEx:%lX\n", HttpServer_GetLastError());
		}
	}
}
void CALLBACK NetCore_CB_Close(LPCSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	printf("NetCore_CB_Close:%s\n", lpszClientAddr);
	if (bSsl)
	{
		OPenSsl_Server_CloseClientEx(xhSsl, lpszClientAddr);
	}
	RfcComponents_Http2Server_CloseClientEx(xhHttp, lpszClientAddr);
}

XHTHREAD NetCore_Thread()
{
	while (bIsRun)
	{
		if (RfcComponents_Http2Server_EventWaitEx(xhHttp, 1))
		{
			int nListCount = 0;
			RFCCOMPONENTS_HTTP_PKTCLIENT** ppSt_ListClient;
			if (RfcComponents_Http2Server_GetPoolEx(xhHttp, 1, &ppSt_ListClient, &nListCount))
			{
				for (int i = 0; i < nListCount; i++)
				{
					int nStreamCount = 0;
					RFCCOMPONENTS_HTTP2_PKTSTREAM** ppSt_PKTStream;
					RfcComponents_Http2Server_GetStreamEx(xhHttp, ppSt_ListClient[i]->tszClientAddr, &ppSt_PKTStream, &nStreamCount);
					for (int j = 0; j < nStreamCount; j++)
					{
						for (int k = 0; k < ppSt_PKTStream[j]->nPktCount; k++)
						{
							int nMsgLen = 4096;
							int nHdrCount = 0;
							TCHAR* ptszMsgBuffer;
							XENGINE_RFCCOMPONENTS_HTTP2_FRAME_TYPE enFrameType;
							RFCCOMPONENTS_HTTP2_HPACK** ppSt_ListHdr;
							if (RfcComponents_Http2Server_GetClientEx(xhHttp, ppSt_ListClient[i]->tszClientAddr, ppSt_PKTStream[j]->nStreamID, &enFrameType, &ptszMsgBuffer, &nMsgLen, &ppSt_ListHdr, &nHdrCount))
							{
								if (XENGINE_RFCCOMPONENTS_HTTP2_FRAME_TYPE_HEADERS == enFrameType)
								{
									//header 如果是POST,都跟上数据,get可能不带数据
									RFCCOMPONENTS_HTTP_REQPARAM st_HTTPRequest;
									memset(&st_HTTPRequest, '\0', sizeof(RFCCOMPONENTS_HTTP_REQPARAM));

									RfcComponents_HttpHelp_HTTP2HdrConvert(&st_HTTPRequest, &ppSt_ListHdr, nHdrCount);
									if (0 == _tcsnicmp("POST", st_HTTPRequest.tszHttpMethod, 4))
									{
										//只有POST才有后续数据
										RfcComponents_Http2Server_GetClientEx(xhHttp, ppSt_ListClient[i]->tszClientAddr, ppSt_PKTStream[j]->nStreamID, &enFrameType, &ptszMsgBuffer, &nMsgLen);
										ppSt_PKTStream[j]->nPktCount--;
									}
								}
								int nSSLLen = 2048;
								int nSDLen = 2048;
								TCHAR tszSSLBuffer[2048];
								TCHAR tszSDBuffer[2048];
								RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;

								memset(tszSSLBuffer, '\0', sizeof(tszSSLBuffer));
								memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
								memset(&st_HDRParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

								if (1)
								{
									//第一次必须打包setting,否则无法继续通信.后面不用在发送
									RfcComponents_Http2Server_PKTSettingEx(xhHttp, tszSDBuffer, &nSDLen, 100, 1024000, 1024000, 8196);
									if (bSsl)
									{
										OPenSsl_Server_SendMsgEx(xhSsl, ppSt_ListClient[i]->tszClientAddr, tszSDBuffer, nSDLen, tszSSLBuffer, &nSSLLen);
										NetCore_TCPXCore_SendEx(xhToken, ppSt_ListClient[i]->tszClientAddr, tszSSLBuffer, nSSLLen);
									}
									else
									{
										NetCore_TCPXCore_SendEx(xhToken, ppSt_ListClient[i]->tszClientAddr, tszSDBuffer, nSDLen);
									}
								}
								st_HDRParam.nHttpCode = 200;
								st_HDRParam.nStreamID = ppSt_PKTStream[j]->nStreamID;

								RfcComponents_Http2Server_PKTHeaderEx(xhHttp, tszSDBuffer, &nSDLen, &st_HDRParam, 6);
								if (bSsl)
								{
									OPenSsl_Server_SendMsgEx(xhSsl, ppSt_ListClient[i]->tszClientAddr, tszSDBuffer, nSDLen, tszSSLBuffer, &nSSLLen);
									NetCore_TCPXCore_SendEx(xhToken, ppSt_ListClient[i]->tszClientAddr, tszSSLBuffer, nSSLLen);
								}
								else
								{
									NetCore_TCPXCore_SendEx(xhToken, ppSt_ListClient[i]->tszClientAddr, tszSDBuffer, nSDLen);
								}
								
								RfcComponents_Http2Server_PKTDataEx(xhHttp, tszSDBuffer, &nSDLen, st_HDRParam.nStreamID, "123456", 6);
								if (bSsl)
								{
									OPenSsl_Server_SendMsgEx(xhSsl, ppSt_ListClient[i]->tszClientAddr, tszSDBuffer, nSDLen, tszSSLBuffer, &nSSLLen);
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
								BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
							}
							if (nHdrCount > 0)
							{
								BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListHdr, nHdrCount);
							}
						}
					}
					BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_PKTStream, nStreamCount);
				}
			}
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClient, nListCount);
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

	LPCTSTR lpszCodeFile = _T("D:\\xengine_apps\\Debug\\HttpCode.types");
	LPCTSTR lpszMimeFile = _T("D:\\xengine_apps\\Debug\\HttpMime.types");
#else
	LPCTSTR lpszCodeFile = _T("HttpCode.types");
	LPCTSTR lpszMimeFile = _T("HttpMime.types");
#endif
	bIsRun = TRUE;

	RfcComponents_HttpConfig_InitCode(lpszCodeFile);
	RfcComponents_HttpConfig_InitMime(lpszMimeFile, FALSE, FALSE);
	xhHttp = RfcComponents_Http2Server_InitEx(1);
	if (NULL == xhHttp)
	{
		printf("%lX\n", HttpServer_GetLastError());
		return 0;
	}

	if (!NetCore_TCPXCore_StartEx(&xhToken, 80))
	{
		printf("%lX\n", NetCore_GetLastError());
		return 0;
	}
	NetCore_TCPXCore_RegisterCallBackEx(xhToken, NetCore_CB_Login, NetCore_CB_Recv, NetCore_CB_Close);

#ifdef _MSC_BUILD
	LPCTSTR lpszCertChain = _T("D:\\xengine_apps\\Debug\\test.xyry.org_bundle.crt");
	LPCTSTR lpszCertKey = _T("D:\\xengine_apps\\Debug\\test.xyry.org.key");
#else
	LPCTSTR lpszCertChain = _T("1_www.xyry.org_bundle.crt");
	LPCTSTR lpszCertKey = _T("2_www.xyry.org.key");
#endif
	xhSsl = OPenSsl_Server_InitEx(lpszCertChain, NULL, lpszCertKey, FALSE, FALSE);
	if (NULL == xhSsl)
	{
		printf("OPenSsl_Server_InitEx:%lX\n", OPenSsl_GetLastError());
		return 0;
	}
	NetCore_Thread();
	std::this_thread::sleep_for(std::chrono::seconds(50000));
	bIsRun = FALSE;
	NetCore_TCPXCore_DestroyEx(xhToken);
	RfcComponents_HttpServer_DestroyEx(xhHttp);
	OPenSsl_Server_StopEx(xhSsl);

#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}