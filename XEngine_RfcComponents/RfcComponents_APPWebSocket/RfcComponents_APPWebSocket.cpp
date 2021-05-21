#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Core.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_Socket.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_WSProtocol.lib")
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
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_WSProtocol/WSProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_WSProtocol/WSProtocol_Error.h"

//g++ -std=c++17 -Wall -g RfcComponents_APPWebSocket.cpp -o RfcComponents_APPWebSocket.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents -lXEngine_BaseLib -lXEngine_Core -lXClient_Socket -lRfcComponents_WSProtocol -lpthread -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk,--disable-new-dtags

XNETHANDLE xhToken;
TCHAR tszClientAddr[64];

BOOL CALLBACK NetCore_CB_Login(LPCSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	strcpy(tszClientAddr, lpszClientAddr);
	printf("NetCore_CB_Login:%s\n", lpszClientAddr);
	RfcComponents_WSPacket_Create(lpszClientAddr, 1);
	return TRUE;
}
void CALLBACK NetCore_CB_Recv(LPCSTR lpszClientAddr, SOCKET hSocket, LPCSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	printf("NetCore_CB_Recv:%s-%d\n", lpszClientAddr, nMsgLen);
	BOOL bLogin = FALSE;
	RfcComponents_WSPacket_GetLogin(lpszClientAddr, &bLogin);
	if (bLogin)
	{
		if (!RfcComponents_WSPacket_Post(lpszClientAddr, lpszRecvMsg, nMsgLen))
		{
			printf("RfcComponents_WSPacket_Post:%lX\n", WSFrame_GetLastError());
		}
	}
	else
	{
		int nSDLen = nMsgLen;
		TCHAR tszHandsBuffer[1024];
		memset(tszHandsBuffer, '\0', sizeof(tszHandsBuffer));

		RfcComponents_WSConnector_HandShake(lpszRecvMsg, &nSDLen, tszHandsBuffer);
		NetCore_TCPXCore_SendEx(xhToken, lpszClientAddr, tszHandsBuffer, nSDLen);
		RfcComponents_WSPacket_SetLogin(lpszClientAddr);
	}
}
void CALLBACK NetCore_CB_Close(LPCSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	printf("NetCore_CB_Close:%s\n", lpszClientAddr);
	RfcComponents_WSPacket_Delete(lpszClientAddr);
}

XHTHREAD CALLBACK NetCore_Thread()
{
	int i = 0;
	while (TRUE)
	{
		if (RfcComponents_WSPacket_WaitEvent(1))
		{
			int nMsgLen = 20480;
			int nListCount = 0;
			TCHAR tszMsgBuffer[20480];
			RFCCOMPONENTS_WSPKT_CLIENT** ppSt_ListClient;
			ENUM_XENGINE_RFCOMPONENTS_WEBSOCKET_OPCODE enOPCode;

			memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

			RfcComponents_WSPacket_GetPool(1, &ppSt_ListClient, &nListCount);
			for (int i = 0; i < nListCount; i++)
			{
				for (int j = 0; j < ppSt_ListClient[i]->nPktCount; j++)
				{
					if (RfcComponents_WSPacket_Get(ppSt_ListClient[i]->tszClientAddr, tszMsgBuffer, &nMsgLen, &enOPCode))
					{
						printf("%s %d:%s\n", ppSt_ListClient[i]->tszClientAddr, nMsgLen, tszMsgBuffer);

						int nSDLen = 0;
						TCHAR tszSendBuffer[2048];
						memset(tszSendBuffer, '\0', sizeof(tszSendBuffer));

						nSDLen = strlen(tszMsgBuffer);
						RfcComponents_WSCodec_EncodeMsg(tszMsgBuffer, tszSendBuffer, &nSDLen, ENUM_XENGINE_RFCOMPONENTS_WEBSOCKET_OPCODE_TEXT);
						NetCore_TCPXCore_SendEx(xhToken, ppSt_ListClient[i]->tszClientAddr, tszSendBuffer, nSDLen);
					}
				}
			}
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClient, nListCount);
			i++;
		}
		if (i > 3)
		{
			break;
		}
	}
	return 0;
}

int main_1()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

	memset(tszClientAddr, '\0', sizeof(tszClientAddr));

	if (!NetCore_TCPXCore_StartEx(&xhToken, 5000, 100, 2))
	{
		printf("%lX\n", NetCore_GetLastError());
		return 0;
	}
	NetCore_TCPXCore_RegisterCallBackEx(xhToken, NetCore_CB_Login, NetCore_CB_Recv, NetCore_CB_Close);
	RfcComponents_WSPacket_Init(10000, FALSE, 4);

	std::thread pSTDThread(NetCore_Thread);
	pSTDThread.join();

#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}
int main()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

	int nLen = 0;
	TCHAR tszKeyBuffer[1024];
	TCHAR tszMsgBuffer[1024];

	memset(tszKeyBuffer, '\0', sizeof(tszKeyBuffer));
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	if (!RfcComponents_WSConnector_Connect(tszKeyBuffer, tszMsgBuffer, &nLen))
	{
		printf("RfcComponents_WSConnector_Connect:%lX", WSFrame_GetLastError());
		return -1;
	}

	SOCKET hSocket = 0;
	if (!XClient_TCPSelect_Create(_T("127.0.0.1"), 5000, &hSocket))
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
	if (!XClient_TCPSelect_RecvMsg(hSocket, tszMsgBuffer, &nLen, FALSE))
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
	TCHAR tszRecvBuffer[2048];

	memset(tszRecvBuffer, '\0', sizeof(tszRecvBuffer));
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	if (RfcComponents_WSCodec_EncodeMsg("123456", tszMsgBuffer, &nRVLen, ENUM_XENGINE_RFCOMPONENTS_WEBSOCKET_OPCODE_TEXT, TRUE))
	{
		if (XClient_TCPSelect_SendMsg(hSocket, tszMsgBuffer, nRVLen))
		{
			printf("NetClient_TCPSelect_SendMsg:%d\n", nRVLen);
		}
	}
	RfcComponents_WSPacket_Init(10000, FALSE, 4);
	RfcComponents_WSPacket_Create("ClientToken", 1);

	std::thread pSTDThread(NetCore_Thread);
	while (1)
	{
		nRVLen = 2048;
		memset(tszRecvBuffer, '\0', sizeof(tszRecvBuffer));
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		if (XClient_TCPSelect_RecvMsg(hSocket, tszRecvBuffer, &nRVLen, FALSE))
		{
			if (!RfcComponents_WSPacket_Post("ClientToken", tszRecvBuffer, nRVLen))
			{
				printf("RfcComponents_WSPacket_Post:%lX\n", WSFrame_GetLastError());
			}
			//你可以直接解码数据,也可以使用wspacket包管理器来获得高性能和自动包管理
			//RfcComponents_WSCodec_DecodeMsg(tszRecvBuffer, &nRVLen, tszMsgBuffer);
			printf("%s\n", tszMsgBuffer);
		}
	}

	pSTDThread.join();

#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}