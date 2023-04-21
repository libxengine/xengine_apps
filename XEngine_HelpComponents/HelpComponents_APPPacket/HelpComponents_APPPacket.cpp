#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/HelpComponents_Packets.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_HelpComponents/HelpComponents_Packets/Packets_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_HelpComponents/HelpComponents_Packets/Packets_Error.h"

//Linux::g++ -std=gnu++17 -Wall -g HelpComponents_APPPacket.cpp -o HelpComponents_APPPacket.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_HelpComponents -lXEngine_BaseLib -lHelpComponents_Packets -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_HelpComponents,--disable-new-dtags
//Macos::g++ -std=gnu++17 -Wall -g HelpComponents_APPPacket.cpp -o HelpComponents_APPPacket.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_HelpComponents -lXEngine_BaseLib -lHelpComponents_Packets


int Test_Cache()
{
	XHANDLE xhPacket = HelpComponents_Cache_InitEx();
	if (NULL == xhPacket)
	{
		printf("HelpComponents_Packets_Init:%lX\n", Packets_GetLastError());
		return -1;
	}
	LPCXSTR lpszMsgBuffer = _X("hello");
	int nLen = strlen(lpszMsgBuffer);

	HelpComponents_Cache_PostEx(xhPacket, lpszMsgBuffer, nLen);
	HelpComponents_Cache_PostEx(xhPacket, lpszMsgBuffer, nLen);
	printf("count:%llu\n", HelpComponents_Cache_GetCountEx(xhPacket));

	HelpComponents_Cache_WaitEventEx(xhPacket);

	int nMsgLen = 0;
	XCHAR* ptszMsgBuffer = NULL;
	HelpComponents_Cache_GetMemoryEx(xhPacket, &ptszMsgBuffer, &nMsgLen);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	HelpComponents_Cache_GetMemoryEx(xhPacket, &ptszMsgBuffer, &nMsgLen);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);

	HelpComponents_Cache_WaitEventEx(xhPacket, 2000);
	HelpComponents_Cache_DestoryEx(xhPacket);
	return 0;
}
int Test_Packets()
{
	XSOCKET hSocket = 100;
	XHANDLE xhPacket = HelpComponents_Packets_Init();
	if (NULL == xhPacket)
	{
		printf("HelpComponents_Packets_Init:%lX\n", Packets_GetLastError());
		return -1;
	}
	if (!HelpComponents_Packets_CreateEx(xhPacket, hSocket))
	{
		printf("HelpComponents_Packets_CreateEx:%lX\n", Packets_GetLastError());
		return -1;
	}

	XCHAR tszMsgBuffer[2048];
	LPCXSTR lpszMsgBuffer = _X("hello");
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
	st_ProtocolHdr.wIsReply = false;

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
	LPCXSTR lpszClientAddr = _X("127.0.0.1");
	XHANDLE xhPacket = HelpComponents_Datas_Init(10000, 4);
	
	if (NULL == xhPacket)
	{
		printf("HelpComponents_Datas_Init:%lX\n", Packets_GetLastError());
		return -1;
	}
	if (!HelpComponents_Datas_CreateEx(xhPacket, lpszClientAddr, 1))
	{
		printf("HelpComponents_Datas_CreateEx:%lX\n", Packets_GetLastError());
		return -1;
	}

	XCHAR tszMsgBuffer[2048];
	LPCXSTR lpszMsgBuffer = _X("hello");
	XENGINE_PROTOCOLHDR st_ProtocolHdr;
	int nLen = strlen(lpszMsgBuffer);

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

	int nMsgLen = sizeof(XENGINE_PROTOCOLHDR) + nLen;

	HelpComponents_Datas_PostEx(xhPacket, lpszClientAddr, tszMsgBuffer, 10);
	HelpComponents_Datas_PostEx(xhPacket, lpszClientAddr, tszMsgBuffer + 10, 15);
	HelpComponents_Datas_PostEx(xhPacket, lpszClientAddr, tszMsgBuffer + 25, 7);
	HelpComponents_Datas_PostEx(xhPacket, lpszClientAddr, tszMsgBuffer + 32, 3);

	HelpComponents_Datas_PostEx(xhPacket, lpszClientAddr, tszMsgBuffer, 35);

	HelpComponents_Datas_WaitEventEx(xhPacket, 1);
	int nListCount = 0;
	HELPCOMPONENT_PACKET_CLIENT** ppSst_ListAddr;
	HelpComponents_Datas_GetPoolEx(xhPacket, 1, &ppSst_ListAddr, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		//再循环客户端拥有的任务个数
		for (int j = 0; j < ppSst_ListAddr[i]->nPktCount; j++)
		{
			int nMsgLen = 0;                             //客户端发送的数据大小,不包括头
			XCHAR* ptszMsgBuffer = NULL;                 //客户端发送的数据
			XENGINE_PROTOCOLHDR st_ProtocolHdr;          //客户端发送的数据的协议头
			//得到一个指定客户端的完整数据包
			if (HelpComponents_Datas_GetMemoryEx(xhPacket, ppSst_ListAddr[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_ProtocolHdr))
			{
				printf("Test_Datas:%d=%s\n", nMsgLen, ptszMsgBuffer);
				//释放内存
				BaseLib_OperatorMemory_FreeCStyle((VOID**)&ptszMsgBuffer);
			}
		}
	}
	HelpComponents_Datas_WaitEventEx(xhPacket, 1);
	HelpComponents_Datas_WaitEventEx(xhPacket, 1, 5000);
	HelpComponents_Datas_DeleteEx(xhPacket, lpszClientAddr);
	HelpComponents_Datas_Destory(xhPacket);
	return 0;
}
int Test_PacketPool()
{
	LPCXSTR lpszClientAddr = _X("127.0.0.1");
	XHANDLE xhPacket = HelpComponents_Datas_Init(10000, 4);
	if (NULL == xhPacket)
	{
		printf("HelpComponents_Datas_Init:%lX\n", Packets_GetLastError());
		return -1;
	}
	if (!HelpComponents_Datas_CreateEx(xhPacket, lpszClientAddr, 1))
	{
		printf("HelpComponents_Datas_CreateEx:%lX\n", Packets_GetLastError());
		return -1;
	}

	XCHAR tszMsgBuffer[2048];
	LPCXSTR lpszMsgBuffer = _X("hello");
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

	XCHAR tszClientAddr[64];
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
	XBYTE byStart;
	XBYTE byCode;
	XBYTE wLen;
}HELPCOMONENTS_CUSTOMHDR;
typedef struct
{
	XBYTE byCheck;
	XBYTE byEnd;
}HELPCOMONENTS_CUSTOMTAIL;

int Test_PacketCustom()
{
	XSOCKET hSocket = 1000;
	XHANDLE xhPacket = HelpComponents_PKTCustom_Init();
	if (NULL == xhPacket)
	{
		printf("HelpComponents_PKTCustom_Init:%lX\n", Packets_GetLastError());
		return -1;
	}
	HelpComponents_PKTCustom_SetHdrEx(xhPacket, 2, 4, sizeof(HELPCOMONENTS_CUSTOMHDR), false);
	HelpComponents_PKTCustom_SetTailEx(xhPacket, sizeof(HELPCOMONENTS_CUSTOMTAIL));

	if (!HelpComponents_PKTCustom_CreateEx(xhPacket, hSocket, 1))
	{
		printf("HelpComponents_PKTCustom_CreateEx:%lX\n", Packets_GetLastError());
		return -1;
	}

	XCHAR tszMsgBuffer[2048];
	LPCXSTR lpszMsgBuffer = _X("hello");
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
	HelpComponents_PKTCustom_GetPoolEx(xhPacket, 1, &ppSt_ListAddr, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		XCHAR* ptszMsgBuffer;
		int nHDRLen = 0;
		HelpComponents_PKTCustom_GetMemoryEx(xhPacket, ppSt_ListAddr[i]->hSocket, &ptszMsgBuffer, &nMsgLen, &st_ProtocolHdr, &nHDRLen, &st_ProtocolTail);
		printf("Test_PacketCustom:%d=%s\n", nMsgLen, tszMsgBuffer);
		BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	}
	HelpComponents_PKTCustom_WaitEventEx(xhPacket, 1, 5000);
	HelpComponents_PKTCustom_DeleteEx(xhPacket, hSocket);
	HelpComponents_PKTCustom_Destory(xhPacket);
	return 0;
}
typedef struct
{
	XBYTE byFlags[4];

	XBYTE byCC : 4;
	XBYTE byX : 1;
	XBYTE byP : 1;
	XBYTE byV : 2;

	XBYTE byPT : 7;
	XBYTE byM : 1;

	XBYTE wSerial;
	XBYTE bySIMNumber[6];
	XBYTE byChannel;

	XBYTE byPacket : 4;
	XBYTE byType : 4;

	XBYTE byTime[8];
	XBYTE wLen;
}XENGINE_RTPPACKETHDR;
int Test_PacketCustom2()
{
	XSOCKET hSocket = 1000;
	XHANDLE xhPacket = HelpComponents_PKTCustom_Init();
	if (NULL == xhPacket)
	{
		printf("HelpComponents_PKTCustom_Init:%lX\n", Packets_GetLastError());
		return -1;
	}
	HelpComponents_PKTCustom_SetHdrEx(xhPacket, 24, 26, sizeof(XENGINE_RTPPACKETHDR));
	
	HelpComponents_PKTCustom_SetConditionsEx(xhPacket, 15, 4, 4, -8, true, true);
	HelpComponents_PKTCustom_SetConditionsEx(xhPacket, 15, 4, 0, 4, true, true);
	HelpComponents_PKTCustom_SetConditionsEx(xhPacket, 15, 4, 1, 4, true, true);
	HelpComponents_PKTCustom_SetConditionsEx(xhPacket, 15, 4, 2, 4, true, true);

	if (!HelpComponents_PKTCustom_CreateEx(xhPacket, hSocket))
	{
		printf("HelpComponents_PKTCustom_CreateEx:%lX\n", Packets_GetLastError());
		return -1;
	}

#ifdef _MSC_BUILD
	LPCXSTR lpszFile = _X("D:\\XEngine_JT1078\\XEngine_APPClient\\Debug\\1.rtp");
#else
	LPCXSTR lpszFile = _X("1.rtp");
#endif
	FILE* pSt_File = fopen(lpszFile, _X("rb"));
	if (NULL == pSt_File)
	{
		return -1;
	}
	while (true)
	{
		XCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		int nRet = fread(tszMsgBuffer, 1, sizeof(tszMsgBuffer), pSt_File);
		if (nRet <= 0)
		{
			break;
		}
		HelpComponents_PKTCustom_PostEx(xhPacket, hSocket, tszMsgBuffer, nRet);

		while (true)
		{
			int nHDRLen = 0;
			int nMsgLen = 0;
			XCHAR* ptszMsgBuffer;
			XCHAR tszHDRBuffer[MAX_PATH];
			XENGINE_RTPPACKETHDR st_RTPPacket;

			memset(&st_RTPPacket, '\0', sizeof(XENGINE_RTPPACKETHDR));
			memset(tszHDRBuffer, '\0', MAX_PATH);

			if (!HelpComponents_PKTCustom_GetMemoryEx(xhPacket, hSocket, &ptszMsgBuffer, &nMsgLen, tszHDRBuffer, &nHDRLen))
			{
				break;
			}
			memcpy(&st_RTPPacket, tszHDRBuffer, sizeof(XENGINE_RTPPACKETHDR));
			printf("Test_PacketCustom:%d T:%d P:%d %d\n", nMsgLen, st_RTPPacket.byType, st_RTPPacket.byPacket, nHDRLen);
			BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
		}
	}
	HelpComponents_PKTCustom_DeleteEx(xhPacket, hSocket);
	HelpComponents_PKTCustom_Destory(xhPacket);
	return 0;
}

void CALLBACK Packet_CBChunk(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, int nChunkCode, XPVOID lParam)
{
	printf("%s:%d-%d\n", lpszClientAddr, nChunkCode, nMsgLen);
}
int Test_DataChunk()
{
	LPCXSTR lpszFile = _X("D:\\xmaster\\Debug\\XEngine_RecordMaster.exe");
	LPCXSTR lpszClientID = _X("192.168.1.5:3301");
	XENGINE_PROTOCOLHDR st_ProtocolHdr;
	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

	st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_CHUNKED;
	st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_CHUNKED_START;
	st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

	//XHANDLE xhPacket = HelpComponents_Datas_Init(10000, 0, 1, false, false, false, Packet_CBChunk);
	XHANDLE xhPacket = HelpComponents_Datas_Init();
	HelpComponents_Datas_CreateEx(xhPacket, lpszClientID, 0);
	HelpComponents_Datas_PostEx(xhPacket, lpszClientID, (LPCXSTR)&st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
	FILE* pSt_File = fopen(lpszFile, _X("rb"));
	if (NULL == pSt_File)
	{
		return -1;
	}
	while (1)
	{
		XCHAR tszMsgBuffer[4096];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		int nRet = fread(tszMsgBuffer, 1, sizeof(tszMsgBuffer), pSt_File);
		if (nRet <= 0)
		{
			break;
		}
		HelpComponents_Datas_PostEx(xhPacket, lpszClientID, tszMsgBuffer, nRet);
	}
	st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_CHUNKED_END;

	XCHAR tszMsgBuffer[4096];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memcpy(tszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));

	st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_LEAVE;
	st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_LEAVE;
	memcpy(tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));

	HelpComponents_Datas_PostEx(xhPacket, lpszClientID, tszMsgBuffer, sizeof(XENGINE_PROTOCOLHDR) * 2);
	fclose(pSt_File);

	pSt_File = fopen("D:\\xmaster\\Debug\\XEngine_RecordMaster1.exe", _X("wb"));
	while (1)
	{
		int nMsgLen = 4096;
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

		if (!HelpComponents_Datas_GetEx(xhPacket, lpszClientID, tszMsgBuffer, &nMsgLen, &st_ProtocolHdr))
		{
			break;
		}
		fwrite(tszMsgBuffer, 1, nMsgLen, pSt_File);
	}
	fclose(pSt_File);
	return 0;
}

int main()
{
	Test_Cache();
	Test_Datas();
	Test_DataChunk();

	Test_PacketCustom2();
	Test_PacketCustom();

	Test_Packets();
	Test_PacketPool();
	return 0;
}
