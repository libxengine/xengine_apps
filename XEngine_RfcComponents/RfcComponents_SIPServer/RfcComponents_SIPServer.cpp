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
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/NetCore_Define.h>
#include <XEngine_Include/XEngine_Core/NetCore_Error.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/HttpProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/HttpProtocol_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/SIPProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/SIPProtocol_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_Core/XEngine_Core.lib")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_HttpProtocol.lib")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_SIPPorotocol.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Core/NetCore_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Core/NetCore_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_ManagePool/ManagePool_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_HttpProtocol/HttpProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_HttpProtocol/HttpProtocol_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_SIPPorotocol/SIPProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_SIPPorotocol/SIPProtocol_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Core.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_HttpProtocol.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_SIPPorotocol.lib")
#endif
#endif

//Linux::g++ -std=c++17 -Wall -g RfcComponents_SIPServer.cpp -o RfcComponents_SIPServer.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents -lXEngine_BaseLib -lXEngine_Core -lRfcComponents_HttpProtocol -lRfcComponents_SIPPorotocol -lpthread -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g RfcComponents_SIPServer.cpp -o RfcComponents_SIPServer.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_Core -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_RfcComponents -lXEngine_BaseLib -lXEngine_Core -lRfcComponents_HttpProtocol -lRfcComponents_SIPPorotocol -lpthread 

bool bIsRun = false;
XHANDLE xhHttp = NULL;
XHANDLE xhToken = NULL;
int Test_SIPRegister()
{
	XCHAR tszMsgBuffer[2048];
	SIPPROTOCOL_HDRINFO st_SIPProtocol;
	LPCXSTR lpszRegBuffer = "REGISTER sip:xyry.org;transport=tls SIP/2.0\r\n"
		"Via: SIP/2.0/TLS 192.168.1.104:52400;branch=z9hG4bKccc1e88d4ab68cf2;rport\r\n"
		"Via: SIP/2.0/UDP 192.168.2.111:52400;branch=z9hG4bKcc35138d4ab68cf2;rport\r\n"
		"Contact: <sip:1001-0x20d1a38@192.168.1.104:52400;transport=tls>;expires=3600\r\n"
		"Max-Forwards: 70\r\n"
		"To: <sip:1001@xyry.org:5061>\r\n"
		"From: <sip:1001@xyry.org:5061>;tag=24174dec803066c5\r\n"
		"Call-ID: 8f8e5fb8ce260308\r\n"
		"CSeq: 62430 REGISTER\r\n"
		"User-Agent: 1.0.2\r\n"
		"Allow: INVITE,ACK,BYE,CANCEL,OPTIONS,REFER,NOTIFY,SUBSCRIBE,INFO,MESSAGE\r\n"
		"Keepalive: 30\r\n"
		"Content-Length: 0\r\n\r\n";

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_SIPProtocol, '\0', sizeof(SIPPROTOCOL_HDRINFO));
	_tcsxcpy(tszMsgBuffer, lpszRegBuffer);
	int nLen = _tcslen(lpszRegBuffer);
	int nMsgLen = 2048;

	RfcComponents_SIPProtocol_Parse(tszMsgBuffer, nLen, &st_SIPProtocol);

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	st_SIPProtocol.st_Response.nCode = 200;
	HttpProtocol_ServerConfig_GetCode(st_SIPProtocol.st_Response.nCode, st_SIPProtocol.st_Response.tszMethod);
	RfcComponents_SIPProtocol_PacketResponse(&st_SIPProtocol, tszMsgBuffer, &nMsgLen);

	return 0;
}

bool CALLBACK NetCore_CB_Login(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	HttpProtocol_Server_CreateClientEx(xhHttp, lpszClientAddr, 0);
	if (!RfcComponents_SIPServer_Exist(lpszClientAddr))
	{
		RfcComponents_SIPServer_Create(lpszClientAddr);
		printf("用户进入:%s\n", lpszClientAddr);
	}
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
	HttpProtocol_Server_CloseClinetEx(xhHttp, lpszClientAddr);
	RfcComponents_SIPServer_Delete(lpszClientAddr);
	printf("用户离开:%s\n", lpszClientAddr);
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
					int nListCount = 0;
					XCHAR** ppszListHdr;
					XCHAR* ptszMsgBuffer = NULL;
					RFCCOMPONENTS_HTTP_REQPARAM st_ReqParam;

					memset(&st_ReqParam, '\0', sizeof(RFCCOMPONENTS_HTTP_REQPARAM));
					if (HttpProtocol_Server_GetMemoryEx(xhHttp, ppSt_ListClient[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_ReqParam, &ppszListHdr, &nListCount))
					{
						XCHAR tszMsgBuffer[2048];
						XCHAR tszToAddr[64];
						SIPPROTOCOL_HDRINFO st_SIPProtocol;

						memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
						memset(tszToAddr, '\0', sizeof(tszToAddr));
						memset(&st_SIPProtocol, '\0', sizeof(SIPPROTOCOL_HDRINFO));

						RfcComponents_SIPProtocol_ParseByHttp(st_ReqParam.tszHttpMethod, st_ReqParam.tszHttpUri, st_ReqParam.tszHttpVer, ptszMsgBuffer, nMsgLen, &ppszListHdr, nListCount, &st_SIPProtocol);
						RfcComponents_SIPServer_Process(ppSt_ListClient[i]->tszClientAddr, &st_SIPProtocol);

						if (st_SIPProtocol.bRequest)
						{
							//是转发还是本地
							if ((st_SIPProtocol.st_Session.nForward == 0) || (0 == _tcsnicmp(st_SIPProtocol.st_To.tszAddr, st_SIPProtocol.st_From.tszAddr, _tcslen(st_SIPProtocol.st_To.tszAddr))))
							{
								if (_tcslen(st_SIPProtocol.st_From.tszName) > 0)
								{
									//在这里处理用户名是否允许使用
								}
								HttpProtocol_ServerConfig_GetCode(st_SIPProtocol.st_Response.nCode, st_SIPProtocol.st_Response.tszMethod);
								RfcComponents_SIPProtocol_PacketResponse(&st_SIPProtocol, tszMsgBuffer, &nMsgLen);
								NetCore_TCPXCore_SendEx(xhToken, ppSt_ListClient[i]->tszClientAddr, tszMsgBuffer, nMsgLen);
							}
							else
							{
								//呼叫请求
								if (RfcComponents_SIPServer_FindAddr(st_SIPProtocol.st_To.tszName, tszToAddr))
								{
									if (100 == st_SIPProtocol.st_Response.nCode)
									{
										//如果是呼叫
										HttpProtocol_ServerConfig_GetCode(st_SIPProtocol.st_Response.nCode, st_SIPProtocol.st_Response.tszMethod);
										RfcComponents_SIPProtocol_PacketResponse(&st_SIPProtocol, tszMsgBuffer, &nMsgLen);
										NetCore_TCPXCore_SendEx(xhToken, ppSt_ListClient[i]->tszClientAddr, tszMsgBuffer, nMsgLen);
										NetCore_TCPXCore_SendEx(xhToken, tszToAddr, ptszMsgBuffer, nMsgLen);
									}
									else
									{
										//其他情况
										NetCore_UDPXCore_SendEx(xhToken, tszToAddr, ptszMsgBuffer, nMsgLen);
									}
								}
								else
								{
									st_SIPProtocol.st_Response.nCode = 404;
									HttpProtocol_ServerConfig_GetCode(st_SIPProtocol.st_Response.nCode, st_SIPProtocol.st_Response.tszMethod);

									RfcComponents_SIPServer_SetStatus(ppSt_ListClient[i]->tszClientAddr);//还原状态
									RfcComponents_SIPProtocol_PacketResponse(&st_SIPProtocol, tszMsgBuffer, &nMsgLen);
									NetCore_UDPXCore_SendEx(xhToken, ppSt_ListClient[i]->tszClientAddr, tszMsgBuffer, nMsgLen);
								}
							}
						}
						else
						{
							//需要转发
							if (RfcComponents_SIPServer_FindAddr(st_SIPProtocol.st_To.tszName, tszToAddr))
							{
								NetCore_UDPXCore_SendEx(xhToken, tszToAddr, ptszMsgBuffer, nMsgLen);
								printf("%s\n", ptszMsgBuffer);
							}
						}
						printf("用户发送数据:%s\n", ppSt_ListClient[i]->tszClientAddr);
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
	LPCXSTR lpszCodeFile = _X("D:\\xengine_apps\\Debug\\SIPCode.types");
#else
	LPCXSTR lpszWFile = _X("2.png");
	LPCXSTR lpszMiniFile = _X("HttpMime.types");
	LPCXSTR lpszCodeFile = _X("SIPCode.types");
#endif
	Test_SIPRegister();
	bIsRun = true;
	xhHttp = HttpProtocol_Server_InitEx(lpszCodeFile, lpszMiniFile, 1);
	if (NULL == xhHttp)
	{
		printf("HttpProtocol_Server_InitEx:%lX\n", HttpProtocol_GetLastError());
		return 0;
	}
	xhToken = NetCore_TCPXCore_StartEx(5000);
	if (NULL == xhToken)
	{
		printf("NetCore_TCPXCore_StartEx:%lX\n", NetCore_GetLastError());
		return 0;
	}

	NetCore_TCPXCore_RegisterCallBackEx(xhToken, NetCore_CB_Login, NetCore_CB_Recv, NetCore_CB_Close);

	RfcComponents_SIPServer_Init("xyry.org", true);
	printf("启动服务成功\n");
	std::thread pSTDThread(NetCore_Thread);

	std::this_thread::sleep_for(std::chrono::seconds(100000));

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
