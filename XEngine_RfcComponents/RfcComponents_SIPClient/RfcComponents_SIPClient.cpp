﻿#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")

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
#include <XEngine_Include/XEngine_Core/ManagePool_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/HttpProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/HttpProtocol_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/SIPProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/SIPProtocol_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_Client/XClient_Socket.lib")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_HttpProtocol.lib")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_SIPPorotocol.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_ManagePool/ManagePool_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_HttpProtocol/HttpProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_HttpProtocol/HttpProtocol_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_SIPPorotocol/SIPProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_SIPPorotocol/SIPProtocol_Error.h"
#ifdef _MSC_BUILD
#ifdef _WIN64
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XClient_Socket.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/RfcComponents_HttpProtocol.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/RfcComponents_SIPPorotocol.lib")
#else
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_Socket.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_HttpProtocol.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_SIPPorotocol.lib")
#endif
#endif
#endif
//Linux::g++ -std=c++20 -Wall -g RfcComponents_SIPClient.cpp -o RfcComponents_SIPClient.exe -lXEngine_BaseLib -lXClient_Socket -lRfcComponents_HttpProtocol -lRfcComponents_SIPPorotocol -lpthread 


XSOCKET hTCPSocket = 0;
XNETHANDLE xhToken = 0;

XHTHREAD XCALLBACK XClient_TCPSelect_Thread()
{
	XCHAR tszMsgBuffer[2048];

	while (1)
	{
		int nMsgLen = 2048;
		SIPPROTOCOL_HDRINFO st_SIPProtocol;
		memset(&st_SIPProtocol, '\0', sizeof(SIPPROTOCOL_HDRINFO));
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		if (XClient_TCPSelect_RecvMsg(hTCPSocket, tszMsgBuffer, &nMsgLen))
		{
			RfcComponents_SIPProtocol_Parse(tszMsgBuffer, nMsgLen, &st_SIPProtocol);
			RfcComponents_SIPClient_Process(xhToken, &st_SIPProtocol);

			//如果是呼叫,我们需要返回一个响铃给对方
			if (0 == _tcsnicmp(st_SIPProtocol.st_Request.tszMethod, XENGINE_RFCCOMPONENTS_SIP_PROTOCOL_STR_TYPE_INVITE, strlen(XENGINE_RFCCOMPONENTS_SIP_PROTOCOL_STR_TYPE_INVITE)))
			{
				memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
				HttpProtocol_ServerConfig_GetCode(st_SIPProtocol.st_Response.nCode, st_SIPProtocol.st_Response.tszMethod);
				RfcComponents_SIPProtocol_PacketResponse(&st_SIPProtocol, tszMsgBuffer, &nMsgLen, true);
				XClient_TCPSelect_SendMsg(hTCPSocket, tszMsgBuffer, nMsgLen);
				printf("Recv:%d\n,%s\n", nMsgLen, tszMsgBuffer);
				//处理后的值是180,表示成功,才可以返回
				if (st_SIPProtocol.st_Response.nCode == 180)
				{
					std::this_thread::sleep_for(std::chrono::seconds(2));
					st_SIPProtocol.st_Response.nCode = 200;       //等待两秒我们接起
					RfcComponents_SIPClient_RepInvite(xhToken, &st_SIPProtocol);
					HttpProtocol_ServerConfig_GetCode(st_SIPProtocol.st_Response.nCode, st_SIPProtocol.st_Response.tszMethod);
					memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
					RfcComponents_SIPProtocol_PacketResponse(&st_SIPProtocol, tszMsgBuffer, &nMsgLen);
					XClient_TCPSelect_SendMsg(hTCPSocket, tszMsgBuffer, nMsgLen);
				}
			}
			else if (0 == _tcsnicmp(st_SIPProtocol.st_Request.tszMethod, XENGINE_RFCCOMPONENTS_SIP_PROTOCOL_STR_TYPE_ACK, strlen(XENGINE_RFCCOMPONENTS_SIP_PROTOCOL_STR_TYPE_ACK)))
			{
				//呼叫请求响应
				printf("通话中\n");
			}
			else if (0 == _tcsnicmp(st_SIPProtocol.st_Request.tszMethod, XENGINE_RFCCOMPONENTS_SIP_PROTOCOL_STR_TYPE_BYE, strlen(XENGINE_RFCCOMPONENTS_SIP_PROTOCOL_STR_TYPE_BYE)))
			{
				//呼叫请求响应
				printf("挂断\n");
				memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
				HttpProtocol_ServerConfig_GetCode(st_SIPProtocol.st_Response.nCode, st_SIPProtocol.st_Response.tszMethod);
				RfcComponents_SIPProtocol_PacketResponse(&st_SIPProtocol, tszMsgBuffer, &nMsgLen, true);
				XClient_TCPSelect_SendMsg(hTCPSocket, tszMsgBuffer, nMsgLen);
			}
			else if (0 == _tcsnicmp(st_SIPProtocol.st_Request.tszMethod, XENGINE_RFCCOMPONENTS_SIP_PROTOCOL_STR_TYPE_MESSAGE, strlen(XENGINE_RFCCOMPONENTS_SIP_PROTOCOL_STR_TYPE_MESSAGE)))
			{
				printf("NEW MESSAGE %s:%s\n", st_SIPProtocol.st_From.tszName, st_SIPProtocol.st_Context.ptszBodyBuffer);
				BaseLib_Memory_FreeCStyle((XPPMEM)&st_SIPProtocol.st_Context.ptszBodyBuffer);
				//清理SDP
				memset(&st_SIPProtocol.st_Context, '\0', sizeof(st_SIPProtocol.st_Context));
				RfcComponents_SIPProtocol_PacketResponse(&st_SIPProtocol, tszMsgBuffer, &nMsgLen, true);
				HttpProtocol_ServerConfig_GetCode(st_SIPProtocol.st_Response.nCode, st_SIPProtocol.st_Response.tszMethod);
				XClient_TCPSelect_SendMsg(hTCPSocket, tszMsgBuffer, nMsgLen);
			}
			else if (0 == _tcsnicmp(st_SIPProtocol.st_Request.tszMethod, XENGINE_RFCCOMPONENTS_SIP_PROTOCOL_STR_TYPE_CANCEL, strlen(XENGINE_RFCCOMPONENTS_SIP_PROTOCOL_STR_TYPE_CANCEL)))
			{
				RfcComponents_SIPProtocol_PacketResponse(&st_SIPProtocol, tszMsgBuffer, &nMsgLen, true);
				HttpProtocol_ServerConfig_GetCode(st_SIPProtocol.st_Response.nCode, st_SIPProtocol.st_Response.tszMethod);
				XClient_TCPSelect_SendMsg(hTCPSocket, tszMsgBuffer, nMsgLen);

				if (200 == st_SIPProtocol.st_Response.nCode)
				{
					//继续返回一个481错误给客户端
					st_SIPProtocol.st_Response.nCode = 481;
					RfcComponents_SIPProtocol_PacketResponse(&st_SIPProtocol, tszMsgBuffer, &nMsgLen);
					HttpProtocol_ServerConfig_GetCode(st_SIPProtocol.st_Response.nCode, st_SIPProtocol.st_Response.tszMethod);
					XClient_TCPSelect_SendMsg(hTCPSocket, tszMsgBuffer, nMsgLen);
				}
			}
			else if (0 == _tcsnicmp(st_SIPProtocol.st_Request.tszMethod, XENGINE_RFCCOMPONENTS_SIP_PROTOCOL_STR_TYPE_OPTIONS, strlen(XENGINE_RFCCOMPONENTS_SIP_PROTOCOL_STR_TYPE_OPTIONS)))
			{
				printf("NEW OPTION %s\n", st_SIPProtocol.st_From.tszName);
				RfcComponents_SIPProtocol_PacketResponse(&st_SIPProtocol, tszMsgBuffer, &nMsgLen, true);
				HttpProtocol_ServerConfig_GetCode(st_SIPProtocol.st_Response.nCode, st_SIPProtocol.st_Response.tszMethod);
				XClient_TCPSelect_SendMsg(hTCPSocket, tszMsgBuffer, nMsgLen);
			}
			//如果是响应并且为成功
			if (!st_SIPProtocol.bRequest)
			{
				if (200 == st_SIPProtocol.st_Response.nCode)
				{
					//我们看看是应该处理什么
					ENUM_RFCCOMPONENTS_SIP_EVENTS enSIPEvent;
					RfcComponents_SIPClient_GetStatus(xhToken, &enSIPEvent);
					if (ENUM_RFCCOMPONENTS_SIP_EVENTS_CALL_COMMUNICATE == enSIPEvent)
					{
						//printf("push any key continue\n");
						//对话接受呼叫了,需要发送一个ACK给对面
						std::this_thread::sleep_for(std::chrono::seconds(1));
						memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
						RfcComponents_SIPProtocol_PacketRequest(&st_SIPProtocol, tszMsgBuffer, &nMsgLen, true);
						XClient_TCPSelect_SendMsg(hTCPSocket, tszMsgBuffer, nMsgLen);
						//通话5秒后挂断
						std::this_thread::sleep_for(std::chrono::seconds(5));
						memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
						RfcComponents_SIPClient_ReqBye(xhToken, &st_SIPProtocol);
						RfcComponents_SIPProtocol_PacketRequest(&st_SIPProtocol, tszMsgBuffer, &nMsgLen);
						XClient_TCPSelect_SendMsg(hTCPSocket, tszMsgBuffer, nMsgLen);
					}
					else if (ENUM_RFCCOMPONENTS_SIP_EVENTS_MESSAGE_ANSWERED == enSIPEvent)
					{
						printf("ok ENUM_RFCCOMPONENTS_SIP_EVENTS_MESSAGE_ANSWERED\n");
					}
					else if (ENUM_RFCCOMPONENTS_SIP_EVENTS_OPTION_ANSWERED == enSIPEvent)
					{
						printf("ok ENUM_RFCCOMPONENTS_SIP_EVENTS_OPTION_ANSWERED\n");
					}
					else if (ENUM_RFCCOMPONENTS_SIP_EVENTS_REGISTRATION_SUCCESS == enSIPEvent)
					{
						printf("ok ENUM_RFCCOMPONENTS_SIP_EVENTS_REGISTRATION_SUCCESS\n");
					}
					else if (ENUM_RFCCOMPONENTS_SIP_EVENTS_REGISTRATION_SUCCESS == enSIPEvent)
					{
						printf("ok ENUM_RFCCOMPONENTS_SIP_EVENTS_REGISTRATION_SUCCESS\n");
					}
					else if (ENUM_RFCCOMPONENTS_SIP_EVENTS_BYE_REQUEST == enSIPEvent)
					{
						printf("ok ENUM_RFCCOMPONENTS_SIP_EVENTS_BYE_ANSWERED\n");
					}
					else if (ENUM_RFCCOMPONENTS_SIP_EVENTS_BYE_ANSWERED == enSIPEvent)
					{
						printf("ok ENUM_RFCCOMPONENTS_SIP_EVENTS_BYE_ANSWERED\n");
					}
				}
				else
				{
					printf("%d %s\n", st_SIPProtocol.st_Response.nCode, st_SIPProtocol.st_Response.tszMethod);
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}
	return 0;
}
int main(int argc, char* argv[])
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

	bool bInvite = false;
	int nPort = 0;
	int nMsgLen = 0;
	XCHAR tszUser[64];
	XCHAR tszToAddr[64];
	XCHAR tszToUser[64];
	XCHAR tszMsgBuffer[2048];

	memset(tszUser, '\0', 64);
	memset(tszToAddr, '\0', 64);
	memset(tszToUser, '\0', 64);
	memset(tszMsgBuffer, '\0', 2048);
	if (bInvite)
	{
		nPort = 15051;
		strcpy(tszUser, "alice");
		strcpy(tszToAddr, "bob@xyry.org");
		strcpy(tszToUser, "bob");
	}
	else
	{
		nPort = 15052;
		strcpy(tszUser, "bob");
		strcpy(tszToAddr, "bob@xyry.org");
		strcpy(tszToUser, "bob");
	}

	LPCXSTR lpszCodeFile = _X("SIPCode.types");
	HttpProtocol_ServerConfig_InitCode(lpszCodeFile, false);

	SIPPROTOCOL_HDRINFO st_SIPProtocol;
	memset(&st_SIPProtocol, '\0', sizeof(SIPPROTOCOL_HDRINFO));

	if (!XClient_TCPSelect_Create(&hTCPSocket, "10.10.12.114", 5000))
	{
		printf("错误\n");
		return -1;
	}
	std::thread pSTDThread(XClient_TCPSelect_Thread);
	//注册
	RfcComponents_SIPClient_Create(&xhToken, "xyry.org", tszUser, "123123");
	RfcComponents_SIPClient_Register(xhToken, &st_SIPProtocol);
	RfcComponents_SIPProtocol_PacketRequest(&st_SIPProtocol, tszMsgBuffer, &nMsgLen);
	XClient_TCPSelect_SendMsg(hTCPSocket, tszMsgBuffer, nMsgLen);

	if (bInvite)
	{
		std::this_thread::sleep_for(std::chrono::seconds(3));
		memset(&st_SIPProtocol, '\0', sizeof(SIPPROTOCOL_HDRINFO));
		memset(tszMsgBuffer, '\0', 2048);
		//填充发送者
		strcpy(st_SIPProtocol.st_To.tszName, tszToUser);
		strcpy(st_SIPProtocol.st_To.tszAddr, tszToAddr);

		//呼叫
		//填充SDP
		st_SIPProtocol.st_Context.ptszBodyBuffer = (XCHAR*)malloc(10);
		memset(st_SIPProtocol.st_Context.ptszBodyBuffer, '\0', 10);
		st_SIPProtocol.st_Context.nBodyLen = 3;
		strcpy(st_SIPProtocol.st_Context.ptszBodyBuffer, "v=0");
		strcpy(st_SIPProtocol.st_Context.tszContentType, "application/sdp");
		//发送
		RfcComponents_SIPClient_ReqInvite(xhToken, &st_SIPProtocol);
		RfcComponents_SIPProtocol_PacketRequest(&st_SIPProtocol, tszMsgBuffer, &nMsgLen);
		XClient_TCPSelect_SendMsg(hTCPSocket, tszMsgBuffer, nMsgLen);

		free(st_SIPProtocol.st_Context.ptszBodyBuffer);
		st_SIPProtocol.st_Context.ptszBodyBuffer = NULL;
		/*MESSSAGE
		st_SIPProtocol.st_Context.nBodyLen = 11;
		strcpy(st_SIPProtocol.st_Context.tszBodyBuffer, "hello world");
		strcpy(st_SIPProtocol.st_Context.tszContentType, "text/plain");
		//发送
		RfcComponents_SIPClient_ReqMessage(xhToken, &st_SIPProtocol);
		RfcComponents_SIPProtocol_PacketRequest(&st_SIPProtocol, tszMsgBuffer, &nMsgLen);
		XClient_TCPSelect_SendMsg(hTCPSocket, tszMsgBuffer, nMsgLen);
		//OPTIONS
		RfcComponents_SIPClient_ReqOptions(xhToken, &st_SIPProtocol);
		RfcComponents_SIPProtocol_PacketRequest(&st_SIPProtocol, tszMsgBuffer, &nMsgLen);
		XClient_TCPSelect_SendMsg(hTCPSocket, tszMsgBuffer, nMsgLen);
	*/
	}
	while (1)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}