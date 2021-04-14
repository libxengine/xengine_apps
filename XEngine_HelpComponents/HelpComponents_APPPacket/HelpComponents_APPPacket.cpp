#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/HelpComponents_Packets.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_HelpComponents/HelpComponents_Packets/Packets_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_HelpComponents/HelpComponents_Packets/Packets_Error.h"

//g++ -std=gnu++17 -Wall -g HelpComponents_APPPacket.cpp -o HelpComponents_APPPacket.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_HelpComponents -lXEngine_BaseLib -lHelpComponents_Packets -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_HelpComponents,--disable-new-dtags

int Test_Packets()
{
	SOCKET hSocket = 100;
	XNETHANDLE xhPacket;
	if (!HelpComponents_Packets_Init(&xhPacket))
	{
		printf("HelpComponents_Packets_Init:%lX\n", Packets_GetLastError());
		return -1;
	}
	if (!HelpComponents_Packets_CreateEx(xhPacket, hSocket))
	{
		printf("HelpComponents_Packets_CreateEx:%lX\n", Packets_GetLastError());
		return -1;
	}

	TCHAR tszMsgBuffer[2048];
	LPCTSTR lpszMsgBuffer = _T("hello");
	XENGINE_PROTOCOLHDREX st_ProtocolHdr;
	XENGINE_PROTOCOLTAILEX st_ProtocolTail;
	int nLen = strlen(lpszMsgBuffer);
	int nMsgLen = 0;

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDREX));
	memset(&st_ProtocolTail, '\0', sizeof(XENGINE_PROTOCOLTAILEX));

	st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProtocolHdr.wVersion = 1;
	st_ProtocolHdr.wPayload = 2;
	st_ProtocolHdr.xhToken = hSocket;
	st_ProtocolHdr.xhXTPTime = 1;
	st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_NORMAL;
	st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_ISOK;
	st_ProtocolHdr.unPacketSize = 5;
	st_ProtocolHdr.wIsReply = FALSE;

	st_ProtocolTail.wCheckSum = HelpComponents_Help_CheckSum(lpszMsgBuffer, nLen);
	st_ProtocolTail.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

	memcpy(tszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDREX));
	memcpy(tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDREX), lpszMsgBuffer, nLen);
	memcpy(tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDREX) + nLen, &st_ProtocolTail, sizeof(XENGINE_PROTOCOLTAILEX));

	nMsgLen = sizeof(XENGINE_PROTOCOLHDREX) + sizeof(XENGINE_PROTOCOLTAILEX) + nLen;

	HelpComponents_Packets_PostEx(xhPacket, hSocket, tszMsgBuffer, 10);
	HelpComponents_Packets_PostEx(xhPacket, hSocket, tszMsgBuffer + 10, 15);
	HelpComponents_Packets_PostEx(xhPacket, hSocket, tszMsgBuffer + 25, 10);

	nMsgLen = 2048;
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDREX));

	HelpComponents_Packets_WaitEventEx(xhPacket);
	HelpComponents_Packets_GetEx(xhPacket, hSocket, tszMsgBuffer, &nMsgLen, &st_ProtocolHdr);

	printf("Test_Packets:%d=%s\n", nMsgLen, tszMsgBuffer);
	HelpComponents_Packets_DeleteEx(xhPacket, hSocket);
	HelpComponents_Packets_Destory(xhPacket);
	return 0;
}
int Test_Datas()
{
	XNETHANDLE xhPacket;
	LPCTSTR lpszClientAddr = _T("127.0.0.1");
	if (!HelpComponents_Datas_Init(&xhPacket))
	{
		printf("HelpComponents_Datas_Init:%lX\n", Packets_GetLastError());
		return -1;
	}
	if (!HelpComponents_Datas_CreateEx(xhPacket, lpszClientAddr))
	{
		printf("HelpComponents_Datas_CreateEx:%lX\n", Packets_GetLastError());
		return -1;
	}

	TCHAR tszMsgBuffer[2048];
	LPCTSTR lpszMsgBuffer = _T("hello");
	XENGINE_PROTOCOLHDR st_ProtocolHdr;
	int nLen = strlen(lpszMsgBuffer);
	int nMsgLen = 0;

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

	st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProtocolHdr.byIsReply = 1;
	st_ProtocolHdr.byVersion = 2;
	st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_ISOK;
	st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_NORMAL;
	st_ProtocolHdr.unPacketSize = nLen;
	st_ProtocolHdr.wPacketSerial = 1;
	st_ProtocolHdr.wReserve = 0;
	st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

	memcpy(tszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
	memcpy(tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), lpszMsgBuffer, nLen);

	nMsgLen = sizeof(XENGINE_PROTOCOLHDR) + nLen;

	HelpComponents_Datas_PostEx(xhPacket, lpszClientAddr, tszMsgBuffer, 10);
	HelpComponents_Datas_PostEx(xhPacket, lpszClientAddr, tszMsgBuffer + 10, 15);
	HelpComponents_Datas_PostEx(xhPacket, lpszClientAddr, tszMsgBuffer + 25, 10);

	HelpComponents_Datas_PostEx(xhPacket, lpszClientAddr, tszMsgBuffer, 35);
	nMsgLen = 2048;
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

	TCHAR tszClientAddr[64];
	memset(tszClientAddr, '\0', sizeof(tszClientAddr));
	HelpComponents_Datas_WaitEventEx(xhPacket);
	HelpComponents_Datas_GetRandomEx(xhPacket, tszClientAddr, tszMsgBuffer, &nMsgLen, &st_ProtocolHdr);
	printf("Test_Datas:%d=%s\n", nMsgLen, tszMsgBuffer);

	nMsgLen = 2048;
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));
	HelpComponents_Datas_WaitEventEx(xhPacket);
	HelpComponents_Datas_GetRandomEx(xhPacket, tszClientAddr, tszMsgBuffer, &nMsgLen, &st_ProtocolHdr);
	printf("Test_Datas:%d=%s\n", nMsgLen, tszMsgBuffer);

	HelpComponents_Datas_WaitEventEx(xhPacket, 5000);
	HelpComponents_Datas_DeleteEx(xhPacket, lpszClientAddr);
	HelpComponents_Datas_Destory(xhPacket);
	return 0;
}
int Test_PacketPool()
{
	XNETHANDLE xhPacket;
	LPCTSTR lpszClientAddr = _T("127.0.0.1");
	if (!HelpComponents_Datas_Init(&xhPacket, 10000, 0, 4))
	{
		printf("HelpComponents_Datas_Init:%lX\n", Packets_GetLastError());
		return -1;
	}
	if (!HelpComponents_Datas_CreateEx(xhPacket, lpszClientAddr, 1))
	{
		printf("HelpComponents_Datas_CreateEx:%lX\n", Packets_GetLastError());
		return -1;
	}

	TCHAR tszMsgBuffer[2048];
	LPCTSTR lpszMsgBuffer = _T("hello");
	XENGINE_PROTOCOLHDR st_ProtocolHdr;
	int nLen = strlen(lpszMsgBuffer);
	int nMsgLen = 0;

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

	st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProtocolHdr.byIsReply = 1;
	st_ProtocolHdr.byVersion = 2;
	st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_ISOK;
	st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_NORMAL;
	st_ProtocolHdr.unPacketSize = nLen;
	st_ProtocolHdr.wPacketSerial = 1;
	st_ProtocolHdr.wReserve = 0;
	st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

	memcpy(tszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
	memcpy(tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), lpszMsgBuffer, nLen);

	nMsgLen = sizeof(XENGINE_PROTOCOLHDR) + nLen;

	HelpComponents_Datas_PostEx(xhPacket, lpszClientAddr, tszMsgBuffer, 10);
	HelpComponents_Datas_PostEx(xhPacket, lpszClientAddr, tszMsgBuffer + 10, 15);
	HelpComponents_Datas_PostEx(xhPacket, lpszClientAddr, tszMsgBuffer + 25, 10);

	HelpComponents_Datas_PostEx(xhPacket, lpszClientAddr, tszMsgBuffer, sizeof(XENGINE_PROTOCOLHDR) + nLen);

	nMsgLen = 2048;
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

	TCHAR tszClientAddr[64];
	memset(tszClientAddr, '\0', sizeof(tszClientAddr));
	HelpComponents_Datas_WaitEventEx(xhPacket, 1);

	int nListCount = 0;
	HELPCOMPONENT_PACKET_CLIENT** ppSt_ListAddr;
	HelpComponents_Datas_GetPoolEx(xhPacket, 1, &ppSt_ListAddr, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		for (int j = 0; j < ppSt_ListAddr[i]->nPktCount; j++)
		{
			HelpComponents_Datas_GetEx(xhPacket, ppSt_ListAddr[i]->tszClientAddr, tszMsgBuffer, &nMsgLen, &st_ProtocolHdr);
			printf("Test_PacketPool:%d=%s\n", nMsgLen, tszMsgBuffer);
		}
	}
	HelpComponents_Datas_WaitEventEx(xhPacket, 1, 5000);
	HelpComponents_Datas_DeleteEx(xhPacket, lpszClientAddr);
	HelpComponents_Datas_Destory(xhPacket);
	return 0;
}

typedef struct
{
	BYTE byStart;
	BYTE byCode;
	WORD wLen;
}HELPCOMONENTS_CUSTOMHDR;
typedef struct
{
	BYTE byCheck;
	BYTE byEnd;
}HELPCOMONENTS_CUSTOMTAIL;

int Test_PacketCustom()
{
	XNETHANDLE xhPacket;
	SOCKET hSocket = 1000;
	if (!HelpComponents_PKTCustom_Init(&xhPacket))
	{
		printf("HelpComponents_PKTCustom_Init:%lX\n", Packets_GetLastError());
		return -1;
	}
	HelpComponents_PKTCustom_SetHdrEx(xhPacket, 2, 4, sizeof(HELPCOMONENTS_CUSTOMHDR), FALSE);
	HelpComponents_PKTCustom_SetTailEx(xhPacket, sizeof(HELPCOMONENTS_CUSTOMTAIL));

	if (!HelpComponents_PKTCustom_CreateEx(xhPacket, hSocket))
	{
		printf("HelpComponents_PKTCustom_CreateEx:%lX\n", Packets_GetLastError());
		return -1;
	}

	TCHAR tszMsgBuffer[2048];
	LPCTSTR lpszMsgBuffer = _T("hello");
	HELPCOMONENTS_CUSTOMHDR st_ProtocolHdr;
	HELPCOMONENTS_CUSTOMTAIL st_ProtocolTail;

	int nLen = strlen(lpszMsgBuffer);
	int nMsgLen = 0;

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_ProtocolHdr, '\0', sizeof(HELPCOMONENTS_CUSTOMHDR));
	memset(&st_ProtocolTail, '\0', sizeof(HELPCOMONENTS_CUSTOMTAIL));

	st_ProtocolHdr.byStart = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProtocolHdr.byCode = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_NORMAL;
	st_ProtocolHdr.wLen = nLen;

	st_ProtocolTail.byCheck = 'E';
	st_ProtocolTail.byEnd = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

	memcpy(tszMsgBuffer, &st_ProtocolHdr, sizeof(HELPCOMONENTS_CUSTOMHDR));
	memcpy(tszMsgBuffer + sizeof(HELPCOMONENTS_CUSTOMHDR), lpszMsgBuffer, nLen);
	memcpy(tszMsgBuffer + sizeof(HELPCOMONENTS_CUSTOMHDR) + nLen, &st_ProtocolTail, sizeof(HELPCOMONENTS_CUSTOMTAIL));

	nMsgLen = sizeof(XENGINE_PROTOCOLHDR) + sizeof(HELPCOMONENTS_CUSTOMTAIL) + nLen;

	HelpComponents_PKTCustom_PostEx(xhPacket, hSocket, tszMsgBuffer, 2);
	HelpComponents_PKTCustom_PostEx(xhPacket, hSocket, tszMsgBuffer + 2, 2);
	HelpComponents_PKTCustom_PostEx(xhPacket, hSocket, tszMsgBuffer + 4, 5);
	HelpComponents_PKTCustom_PostEx(xhPacket, hSocket, tszMsgBuffer + 9, 2);

	nMsgLen = 2048;
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_ProtocolHdr, '\0', sizeof(HELPCOMONENTS_CUSTOMHDR));
	memset(&st_ProtocolTail, '\0', sizeof(HELPCOMONENTS_CUSTOMTAIL));

	int nListCount = 0;
	HELPCOMPONENT_PACKET_CLIENT** ppSt_ListAddr;
	HelpComponents_PKTCustom_GetListEx(xhPacket, &ppSt_ListAddr, &nListCount, 4, 4);
	for (int i = 0; i < nListCount; i++)
	{
		HelpComponents_PKTCustom_GetEx(xhPacket, ppSt_ListAddr[i]->hSocket, tszMsgBuffer, &nMsgLen, &st_ProtocolHdr, &st_ProtocolTail);
		printf("Test_PacketCustom:%d=%s\n", nMsgLen, tszMsgBuffer);
	}
	HelpComponents_PKTCustom_WaitEventEx(xhPacket, 1, 5000);
	HelpComponents_PKTCustom_DeleteEx(xhPacket, hSocket);
	HelpComponents_PKTCustom_Destory(xhPacket);
	return 0;
}

int main()
{
	Test_Packets();
	Test_Datas();
	Test_PacketPool();
	Test_PacketCustom();
	return 0;
}
