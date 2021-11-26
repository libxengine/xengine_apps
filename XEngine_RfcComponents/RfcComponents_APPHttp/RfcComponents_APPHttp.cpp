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

//g++ -std=c++17 -Wall -g RfcComponents_APPHttp.cpp -o RfcComponents_APPHttp.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents -lXEngine_BaseLib -lXEngine_Core -lXEngine_OPenSsl -lRfcComponents_HttpServer -lpthread -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk,--disable-new-dtags

BOOL bSSL = TRUE;
BOOL bIsRun = FALSE;
XHANDLE xhSsl = NULL;
XNETHANDLE xhToken;
int nRVMode = 0;
XHANDLE xhHttp = NULL;
FILE* pSt_File = NULL;

BOOL CALLBACK NetCore_CB_Login(LPCSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	if (bSSL)
	{
		TCHAR tszSubject[MAX_PATH];
		TCHAR tszIssuer[MAX_PATH];
		TCHAR tszAlgorithm[MAX_PATH];

		memset(tszSubject, '\0', MAX_PATH);
		memset(tszIssuer, '\0', MAX_PATH);
		memset(tszAlgorithm, '\0', MAX_PATH);

		OPenSsl_Server_AcceptEx(xhSsl, hSocket, lpszClientAddr, tszSubject, tszIssuer, tszAlgorithm);
	}
	printf("NetCore_CB_Login:%s\n", lpszClientAddr);
	RfcComponents_HttpServer_CreateClientEx(xhHttp, lpszClientAddr, 0);
	RfcComponents_HttpServer_SetRecvModeEx(xhHttp, lpszClientAddr, nRVMode);
	return TRUE;
}
void CALLBACK NetCore_CB_Recv(LPCSTR lpszClientAddr, SOCKET hSocket, LPCSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	printf("NetCore_CB_Recv:%s-%d\n", lpszClientAddr, nMsgLen);
	if (bSSL)
	{
		int nRVLen = 2048;
		TCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		OPenSsl_Server_RecvMsgEx(xhSsl, lpszClientAddr, tszMsgBuffer, &nRVLen, lpszRecvMsg, nMsgLen);
		if (!RfcComponents_HttpServer_InserQueueEx(xhHttp, lpszClientAddr, tszMsgBuffer, nRVLen))
		{
			printf("RfcComponents_WSPacket_Post:%lX\n", HttpServer_GetLastError());
		}
	}
	else
	{
		if (!RfcComponents_HttpServer_InserQueueEx(xhHttp, lpszClientAddr, lpszRecvMsg, nMsgLen))
		{
			printf("RfcComponents_WSPacket_Post:%lX\n", HttpServer_GetLastError());
		}
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
			int nMsgLen = 20480;
			int nListCount = 0;
			TCHAR tszMsgBuffer[20480];
			TCHAR tszClientAddr[64];
			CHAR** ppszListHdr;
			RFCCOMPONENTS_HTTP_REQPARAM st_ReqParam;

			memset(&st_ReqParam, '\0', sizeof(RFCCOMPONENTS_HTTP_REQPARAM));
			memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
			memset(tszClientAddr, '\0', sizeof(tszClientAddr));

			if (RfcComponents_HttpServer_GetRandomEx(xhHttp, tszClientAddr, tszMsgBuffer, &nMsgLen, &st_ReqParam, &ppszListHdr, &nListCount))
			{
				int nRVMode = 0;
				int nRVCount = 0;
				int nHDSize = 0;
				RfcComponents_HttpServer_GetRecvModeEx(xhHttp, tszClientAddr, &nRVMode, &nRVCount, &nHDSize);
				if (1 == nRVMode)
				{
					printf("count:%d,recv:%d\n", nRVCount, nHDSize);
					fwrite(tszMsgBuffer, 1, nMsgLen, pSt_File);
					if (nHDSize >= nRVCount)
					{
						RFCCOMPONENTS_HTTP_HDRPARAM st_HdrParam;
						memset(&st_HdrParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

						st_HdrParam.bIsClose = TRUE;
						st_HdrParam.nHttpCode = 200;
						nMsgLen = 2048;
						RfcComponents_HttpServer_SendMsgEx(xhHttp, tszMsgBuffer, &nMsgLen, &st_HdrParam);
						NetCore_TCPXCore_SendEx(xhToken, tszClientAddr, tszMsgBuffer, nMsgLen);

						fclose(pSt_File);
						break;
					}
				}
				else
				{
					printf("%s %d:%s\n", tszClientAddr, nMsgLen, tszMsgBuffer);
					RFCCOMPONENTS_HTTP_HDRPARAM st_HdrParam;
					memset(&st_HdrParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

					st_HdrParam.bIsClose = TRUE;
					st_HdrParam.nHttpCode = 200;
					nMsgLen = 2048;
					RfcComponents_HttpServer_SendMsgEx(xhHttp, tszMsgBuffer, &nMsgLen, &st_HdrParam, "123456789", 9);
					if (bSSL)
					{
						int nSSLLen = 2048;
						TCHAR tszSSLBuffer[2048];
						memset(tszSSLBuffer, '\0', sizeof(tszSSLBuffer));

						OPenSsl_Server_SendMsgEx(xhSsl, tszClientAddr, tszMsgBuffer, nMsgLen, tszSSLBuffer, &nSSLLen);
						NetCore_TCPXCore_SendEx(xhToken, tszClientAddr, tszSSLBuffer, nSSLLen);
					}
					else
					{
						NetCore_TCPXCore_SendEx(xhToken, tszClientAddr, tszMsgBuffer, nMsgLen);
					}
				}
				BaseLib_OperatorMemory_Free((XPPPMEM)&ppszListHdr, nListCount);
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
		printf("%lX\n", HttpServer_GetLastError());
		return 0;
	}
	if (bSSL)
	{
#ifdef _WINDOWS
		LPCTSTR lpszCertChain = _T("D:\\xengine_apps\\Debug\\1_www.xyry.org_bundle.crt");
		LPCTSTR lpszCertKey = _T("D:\\xengine_apps\\Debug\\2_www.xyry.org.key");
#else
		LPCTSTR lpszCertChain = _T("1_www.xyry.org_bundle.crt");
		LPCTSTR lpszCertKey = _T("2_www.xyry.org.key");
#endif
		xhSsl = OPenSsl_Server_InitEx(lpszCertChain, NULL, lpszCertKey, FALSE, FALSE);
		if (NULL == xhSsl)
		{
			printf("%lX\n", OPenSsl_GetLastError());
			return 0;
		}
		if (!NetCore_TCPXCore_StartEx(&xhToken, 443))
		{
			printf("%lX\n", NetCore_GetLastError());
			return 0;
		}
	}
	else
	{
		if (!NetCore_TCPXCore_StartEx(&xhToken, 80))
		{
			printf("%lX\n", NetCore_GetLastError());
			return 0;
		}
	}
	
	NetCore_TCPXCore_RegisterCallBackEx(xhToken, NetCore_CB_Login, NetCore_CB_Recv, NetCore_CB_Close);
	std::thread pSTDThread(NetCore_Thread);

	std::this_thread::sleep_for(std::chrono::seconds(2000));
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