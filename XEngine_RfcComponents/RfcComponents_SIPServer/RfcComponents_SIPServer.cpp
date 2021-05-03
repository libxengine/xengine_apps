#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Core.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_HttpServer.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_SIPPorotocol.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_Core/NetCore_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_Core/NetCore_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_HttpServer/HttpServer_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_HttpServer/HttpServer_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_SIPPorotocol/SIPProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_SIPPorotocol/SIPProtocol_Error.h"


//g++ -std=c++17 -Wall -g RfcComponents_SIPServer.cpp -o RfcComponents_SIPServer.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents -lXEngine_BaseLib -lXEngine_Core -lRfcComponents_HttpServer -lRfcComponents_SIPPorotocol -lpthread -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk,--disable-new-dtags


XNETHANDLE xhToken = 0;
int Test_SIPRegister()
{
	TCHAR tszMsgBuffer[2048];
	SIPPROTOCOL_HDRINFO st_SIPProtocol;
	LPCTSTR lpszRegBuffer = "REGISTER sip:xyry.org;transport=tls SIP/2.0\r\n"
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
	_tcscpy(tszMsgBuffer, lpszRegBuffer);
	int nLen = _tcslen(lpszRegBuffer);
	int nMsgLen = 2048;

	RfcComponents_SIPProtocol_Parse(tszMsgBuffer, nLen, &st_SIPProtocol);

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	st_SIPProtocol.st_Response.nCode = 200;
	RfcComponents_HttpConfig_GetCode(st_SIPProtocol.st_Response.nCode, st_SIPProtocol.st_Response.tszMethod);
	RfcComponents_SIPProtocol_PacketResponse(&st_SIPProtocol, tszMsgBuffer, &nMsgLen);

	return 0;
}
void CALLBACK NetCore_CBRecv(LPCSTR lpszClientAddr, SOCKET hSocket, LPCSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	int nRecvLen = nMsgLen;
	TCHAR tszMsgBuffer[2048];
	TCHAR tszToAddr[64];
	SIPPROTOCOL_HDRINFO st_SIPProtocol;

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(tszToAddr, '\0', sizeof(tszToAddr));
	memset(&st_SIPProtocol, '\0', sizeof(SIPPROTOCOL_HDRINFO));

	if (!RfcComponents_SIPServer_Exist(lpszClientAddr))
	{
		RfcComponents_SIPServer_Create(lpszClientAddr);
		printf("用户进入:%s\n", lpszClientAddr);
	}
	RfcComponents_SIPProtocol_Parse(lpszRecvMsg, nRecvLen, &st_SIPProtocol);
	RfcComponents_SIPServer_Process(lpszClientAddr, &st_SIPProtocol);

	if (st_SIPProtocol.bRequest)
	{
		//是转发还是本地
		if ((st_SIPProtocol.st_Session.nForward == 0) || (0 == _tcsnicmp(st_SIPProtocol.st_To.tszAddr, st_SIPProtocol.st_From.tszAddr, _tcslen(st_SIPProtocol.st_To.tszAddr))))
		{
			RfcComponents_HttpConfig_GetCode(st_SIPProtocol.st_Response.nCode, st_SIPProtocol.st_Response.tszMethod);
			RfcComponents_SIPProtocol_PacketResponse(&st_SIPProtocol, tszMsgBuffer, &nRecvLen);
			NetCore_UDPXCore_SendMsgEx(xhToken, lpszClientAddr, tszMsgBuffer, &nRecvLen);
		}
		else
		{
			//呼叫请求
			if (RfcComponents_SIPServer_FindAddr(st_SIPProtocol.st_To.tszName, tszToAddr))
			{
				if (100 == st_SIPProtocol.st_Response.nCode)
				{
					//如果是呼叫
					RfcComponents_HttpConfig_GetCode(st_SIPProtocol.st_Response.nCode, st_SIPProtocol.st_Response.tszMethod);
					RfcComponents_SIPProtocol_PacketResponse(&st_SIPProtocol, tszMsgBuffer, &nRecvLen);
					NetCore_UDPXCore_SendMsgEx(xhToken, lpszClientAddr, tszMsgBuffer, &nRecvLen);

					NetCore_UDPXCore_SendMsgEx(xhToken, tszToAddr, lpszRecvMsg, &nMsgLen);
				}
				else
				{
					//其他情况
					NetCore_UDPXCore_SendMsgEx(xhToken, tszToAddr, lpszRecvMsg, &nMsgLen);
				}
			}
			else
			{
				st_SIPProtocol.st_Response.nCode = 404;
				RfcComponents_HttpConfig_GetCode(st_SIPProtocol.st_Response.nCode, st_SIPProtocol.st_Response.tszMethod);

				RfcComponents_SIPServer_SetStatus(lpszClientAddr);//还原状态
				RfcComponents_SIPProtocol_PacketResponse(&st_SIPProtocol, tszMsgBuffer, &nRecvLen);
				NetCore_UDPXCore_SendMsgEx(xhToken, lpszClientAddr, tszMsgBuffer, &nRecvLen);
			}
		}
	}
	else
	{
		//需要转发
		if (RfcComponents_SIPServer_FindAddr(st_SIPProtocol.st_To.tszName, tszToAddr))
		{
			NetCore_UDPXCore_SendMsgEx(xhToken, tszToAddr, lpszRecvMsg, &nRecvLen);
			printf("%s\n", lpszRecvMsg);
		}
	}
	printf("用户发送数据:%s\n", lpszClientAddr);
}

int main()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

	LPCTSTR lpszCodeFile = _T("SipCode.types");
	RfcComponents_HttpConfig_InitCode(lpszCodeFile, FALSE);
 	Test_SIPRegister();

	if (!NetCore_UDPXCore_StartEx(&xhToken, 5000, 2))
	{
		printf("错误\n");
		return -1;
	}
	NetCore_UDPXCore_RegisterCallBackEx(xhToken, NetCore_CBRecv);

	RfcComponents_SIPServer_Init("xyry.org", TRUE);
	RfcComponents_SIPServer_UserInsert("bob", "123123");
	RfcComponents_SIPServer_UserInsert("alice", "123123");
	printf("启动服务成功\n");
	while (1)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}
