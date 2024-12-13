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
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/WSProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/WSProtocol_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_Core/XEngine_Core.lib")
#pragma comment(lib,"XEngine_Client/XClient_Socket.lib")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_WSProtocol.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Core/NetCore_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Core/NetCore_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_ManagePool/ManagePool_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_WSProtocol/WSProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_WSProtocol/WSProtocol_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Core.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_Socket.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_WSProtocol.lib")
#endif
#endif

//Linux::g++ -std=c++20 -Wall -g RfcComponents_APPWebSocket.cpp -o RfcComponents_APPWebSocket.exe -lXEngine_BaseLib -lXEngine_Core -lXClient_Socket -lRfcComponents_WSProtocol -lpthread

XHANDLE xhToken = NULL;
XHANDLE xhWBPacket = NULL;
XCHAR tszClientAddr[64];

bool CALLBACK NetCore_CB_Login(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	strcpy(tszClientAddr, lpszClientAddr);
	printf("NetCore_CB_Login:%s\n", lpszClientAddr);
	RfcComponents_WSPacket_CreateEx(xhWBPacket, lpszClientAddr, 1);
	return true;
}
void CALLBACK NetCore_CB_Recv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	printf("NetCore_CB_Recv:%s-%d\n", lpszClientAddr, nMsgLen);
	bool bLogin = false;
	RfcComponents_WSPacket_GetLoginEx(xhWBPacket, lpszClientAddr, &bLogin);
	if (bLogin)
	{
		if (!RfcComponents_WSPacket_InsertQueueEx(xhWBPacket, lpszClientAddr, lpszRecvMsg, nMsgLen))
		{
			printf("RfcComponents_WSPacket_Post:%lX\n", WSFrame_GetLastError());
		}
	}
	else
	{
		int nSDLen = nMsgLen;
		XCHAR tszHandsBuffer[1024];
		memset(tszHandsBuffer, '\0', sizeof(tszHandsBuffer));

		RfcComponents_WSConnector_HandShake(lpszRecvMsg, &nSDLen, tszHandsBuffer);
		NetCore_TCPXCore_SendEx(xhToken, lpszClientAddr, tszHandsBuffer, nSDLen);
		RfcComponents_WSPacket_SetLoginEx(xhWBPacket, lpszClientAddr);
		printf("login:%s-%d\n", lpszClientAddr, nMsgLen);
	}
}
void CALLBACK NetCore_CB_Close(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	printf("NetCore_CB_Close:%s\n", lpszClientAddr);
	RfcComponents_WSPacket_DeleteEx(xhWBPacket, lpszClientAddr);
}

XHTHREAD CALLBACK NetCore_Thread()
{
	int i = 0;
	while (true)
	{
		if (RfcComponents_WSPacket_WaitEventEx(xhWBPacket, 1))
		{
			int nListCount = 0;
			XENGINE_MANAGEPOOL_TASKEVENT** ppSt_ListClient;

			RfcComponents_WSPacket_GetPoolEx(xhWBPacket, 1, &ppSt_ListClient, &nListCount);
			for (int i = 0; i < nListCount; i++)
			{
				for (int j = 0; j < ppSt_ListClient[i]->nPktCount; j++)
				{
					int nMsgLen = 8192;
					XCHAR tszMsgBuffer[8192];
					ENUM_XENGINE_RFCOMPONENTS_WEBSOCKET_OPCODE enOPCode;

					memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
					if (RfcComponents_WSPacket_GetEx(xhWBPacket, ppSt_ListClient[i]->tszClientAddr, tszMsgBuffer, &nMsgLen, &enOPCode))
					{
						printf("RfcComponents_WSPacket_GetEx:%s %d:%s\n", ppSt_ListClient[i]->tszClientAddr, nMsgLen, tszMsgBuffer);

						int nSDLen = 0;
						XCHAR tszSendBuffer[2048];
						memset(tszSendBuffer, '\0', sizeof(tszSendBuffer));

						nSDLen = strlen(tszMsgBuffer);
						RfcComponents_WSCodec_EncodeMsg(tszMsgBuffer, tszSendBuffer, &nSDLen, ENUM_XENGINE_RFCOMPONENTS_WEBSOCKET_OPCODE_TEXT);
						NetCore_TCPXCore_SendEx(xhToken, ppSt_ListClient[i]->tszClientAddr, tszSendBuffer, nSDLen);
					}
				}
			}
			BaseLib_Memory_Free((XPPPMEM)&ppSt_ListClient, nListCount);
			i++;
		}
	}
	return 0;
}

int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

	memset(tszClientAddr, '\0', sizeof(tszClientAddr));

	xhToken = NetCore_TCPXCore_StartEx(5000, 100, 2, false, true);
	if (NULL == xhToken)
	{
		printf("%lX\n", NetCore_GetLastError());
		return 0;
	}
	NetCore_TCPXCore_RegisterCallBackEx(xhToken, NetCore_CB_Login, NetCore_CB_Recv, NetCore_CB_Close);
	xhWBPacket = RfcComponents_WSPacket_InitEx(4);

	std::thread pSTDThread(NetCore_Thread);
	pSTDThread.join();

#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}

int mainc()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

	int nLen = 0;
	XCHAR tszKeyBuffer[1024];
	XCHAR tszMsgBuffer[1024];

	memset(tszKeyBuffer, '\0', sizeof(tszKeyBuffer));
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	if (!RfcComponents_WSConnector_Connect(tszKeyBuffer, tszMsgBuffer, &nLen))
	{
		printf("RfcComponents_WSConnector_Connect:%lX", WSFrame_GetLastError());
		return -1;
	}

	XSOCKET hSocket = 0;
	if (!XClient_TCPSelect_Create(&hSocket, _X("10.10.12.114"), 5000))
	{
		printf("NetClient_TCPSelect_Create:%lX", XClient_GetLastError());
		return -1;
	}

	if (!XClient_TCPSelect_SendMsg(hSocket, tszMsgBuffer, nLen))
	{
		printf("NetClient_TCPSelect_SendMsg:%lX", XClient_GetLastError());
		return -1;
	}

	int nPos = 0;
	nLen = 1024;
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	if (!XClient_TCPSelect_RecvMsg(hSocket, tszMsgBuffer, &nLen))
	{
		printf("NetClient_TCPSelect_RecvMsg:%lX", XClient_GetLastError());
		return -1;
	}
	if (!RfcComponents_WSConnector_VerConnect(tszKeyBuffer, tszMsgBuffer, &nPos))
	{
		printf("RfcComponents_WSConnector_VerConnect:%lX", WSFrame_GetLastError());
		return -1;
	}

	if (nPos > 0)
	{
		printf("%s\n", tszMsgBuffer + nPos);
	}
	int nRVLen = 6;
	XCHAR tszRecvBuffer[2048];

	memset(tszRecvBuffer, '\0', sizeof(tszRecvBuffer));
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	if (RfcComponents_WSCodec_EncodeMsg("123456", tszMsgBuffer, &nRVLen, ENUM_XENGINE_RFCOMPONENTS_WEBSOCKET_OPCODE_TEXT, true))
	{
		if (XClient_TCPSelect_SendMsg(hSocket, tszMsgBuffer, nRVLen))
		{
			printf("NetClient_TCPSelect_SendMsg:%d\n", nRVLen);
		}
	}
	xhWBPacket = RfcComponents_WSPacket_InitEx(10000, false, 4);
	RfcComponents_WSPacket_CreateEx(xhWBPacket, "ClientToken", 1);

	std::thread pSTDThread(NetCore_Thread);
	while (1)
	{
		nRVLen = 2048;
		memset(tszRecvBuffer, '\0', sizeof(tszRecvBuffer));
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		if (XClient_TCPSelect_RecvMsg(hSocket, tszRecvBuffer, &nRVLen))
		{
			if (!RfcComponents_WSPacket_InsertQueueEx(xhWBPacket,"ClientToken", tszRecvBuffer, nRVLen))
			{
				printf("RfcComponents_WSPacket_Post:%lX\n", WSFrame_GetLastError());
			}
			//你可以直接解码数据,也可以使用wspacket包管理器来获得高性能和自动包管理
			//RfcComponents_WSCodec_DecodeMsg(tszRecvBuffer, &nRVLen, tszMsgBuffer);
			printf("%s\n", tszMsgBuffer);
		}
	}
	pSTDThread.join();

#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}