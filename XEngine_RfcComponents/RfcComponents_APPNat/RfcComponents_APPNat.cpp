#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if 1 == _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/NatProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/NatProtocol_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_Client/XClient_Socket.lib")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_NatProtocol.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_NatProtocol/NatProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_NatProtocol/NatProtocol_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_Socket.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_NatProtocol.lib")
#endif
#endif
//Linux::g++ -std=c++17 -Wall -g RfcComponents_APPNat.cpp -o RfcComponents_APPNat.exe -lXEngine_BaseLib -lXClient_Socket -lRfcComponents_NatProtocol


int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	
	XSOCKET m_Socket;
	int nMsgLen = 0;
	XCHAR tszMsgBuffer[2048];
	RFCCOMPONENTS_NATSTUN st_NatClient;

	memset(&st_NatClient, '\0', sizeof(RFCCOMPONENTS_NATSTUN));
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	if (!XClient_UDPSelect_Create(&m_Socket))
	{
		printf("创建套接字失败!,错误:%lX\n", NatProtocol_GetLastError());
		return -1;
	}
	XClient_UDPSelect_Connect(m_Socket, "127.0.0.1", 5604);

	XCHAR tszTmpBuffer[128] = {};
	XCHAR tszRandomStr[10] = {};
	BaseLib_OperatorHandle_CreateStr(tszRandomStr, 8, 0, 2);
	NatProtocol_StunNat_BuildAttr(tszTmpBuffer, &nMsgLen, RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_ATTR_USERNAME, "j107le40:qhto", 13);

	//NatProtocol_StunNat_BuildMapAddress(tszTmpBuffer + nMsgLen, &nMsgLen, "127.0.0.1", 5501);
	//NatProtocol_StunNat_BuildMapAddress(tszTmpBuffer + nMsgLen, &nMsgLen, "192.168.1.180", 5501, true);
	
	NatProtocol_StunNat_BuildAttr(tszTmpBuffer + nMsgLen, &nMsgLen, RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_ATTR_ICE_CONTROLLING, tszRandomStr, 8);
	NatProtocol_StunNat_BuildPriority(tszTmpBuffer + nMsgLen, &nMsgLen);
	NatProtocol_StunNat_BuildAttr(tszTmpBuffer + nMsgLen, &nMsgLen, RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_ATTR_USER_CANDIDATE);

	XCHAR tszTokenStr[64] = {};
	BaseLib_OperatorHandle_CreateStr(tszTokenStr, 12, 0, 2);
	if (!NatProtocol_StunNat_Packet(tszMsgBuffer, &nMsgLen, tszTokenStr, RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_CLASS_REQUEST, RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_ATTR_MAPPED_ADDRESS, tszTmpBuffer))
	{
		printf("构建请求包缓冲区错误!\n");
		return -1;
	}
	/*
	int nFLen = 0;
	XCHAR tszFinBuffer[128] = {};
	NatProtocol_StunNat_BuildFinger(tszMsgBuffer + nMsgLen - 8, &nFLen, tszMsgBuffer, nMsgLen - 8);
	*/
	if (!XClient_UDPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf("发送数据失败!,错误:%lX\n", XClient_GetLastError());
		return -1;
	}
	//如果服务器正常,但是收不到包,说明你的路由不支持NAT
	nMsgLen = 2048;
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	if (!XClient_UDPSelect_RecvMsg(m_Socket, tszMsgBuffer, &nMsgLen))
	{
		printf("接受数据失败!,错误:%lX\n", XClient_GetLastError());
		return -1;
	}
	int nListCount = 0;
	RFCCOMPONENTS_NATATTR** ppSt_ListAttr;
	if (!NatProtocol_StunNat_Parse(tszMsgBuffer, nMsgLen, &st_NatClient, &ppSt_ListAttr, &nListCount))
	{
		printf("处理数据失败!,错误:%lX\n", NatProtocol_GetLastError());
		return -1;
	}
	printf("%s %s\n", st_NatClient.tszServiceName, st_NatClient.byTokenStr);

	for (int i = 0; i < nListCount; i++)
	{
		if (RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_ATTR_ERROR_CODE == ppSt_ListAttr[i]->wAttr)
		{
			int nErrorCode = 0;
			int nMsgLen = 0;
			XCHAR tszMsgBuffer[2048];
			memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

			NatProtocol_StunNat_ParseError(ppSt_ListAttr[i], &nErrorCode, tszMsgBuffer, &nMsgLen);
		}
		else if (RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_ATTR_XOR_MAPPED_ADDRESS == ppSt_ListAttr[i]->wAttr)
		{
			int nIPVer = 0;
			XBYTE wXor = 0;
			XCHAR tszIPAddr[128];
			memset(tszIPAddr, '\0', sizeof(tszIPAddr));

			memcpy(&wXor, &st_NatClient.unMagic, sizeof(XBYTE));

			NatProtocol_StunNat_ParseAddr(ppSt_ListAttr[i], &nIPVer, tszIPAddr, wXor);
			printf("%d %s\n", nIPVer, tszIPAddr);
		}
		else if (RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_ATTR_MAPPED_ADDRESS == ppSt_ListAttr[i]->wAttr)
		{
			int nIPVer = 0;
			XCHAR tszIPAddr[128];
			memset(tszIPAddr, '\0', sizeof(tszIPAddr));

			NatProtocol_StunNat_ParseAddr(ppSt_ListAttr[i], &nIPVer, tszIPAddr);
			printf("%d %s\n", nIPVer, tszIPAddr);
		}
		else if (RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_ATTR_SERVERADDR == ppSt_ListAttr[i]->wAttr)
		{
			int nIPVer = 0;
			XCHAR tszIPAddr[128];
			memset(tszIPAddr, '\0', sizeof(tszIPAddr));

			NatProtocol_StunNat_ParseAddr(ppSt_ListAttr[i], &nIPVer, tszIPAddr);
			printf("%d %s\n", nIPVer, tszIPAddr);
		}
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListAttr, nListCount);

	int nA1Len = 0;
	int nA2Len = 0;
	int nA3Len = 0;
	XCHAR tszAttrBuffer[MAX_PATH];

	memset(tszAttrBuffer, '\0', MAX_PATH);
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	NatProtocol_StunNat_BuildTransPort(tszAttrBuffer, &nA1Len);
	NatProtocol_StunNat_BuildLeftTime(tszAttrBuffer + nA1Len, &nA2Len);
	NatProtocol_StunNat_BuildAddrFamily(tszAttrBuffer + nA1Len + nA2Len, &nA3Len);
	nMsgLen = nA1Len + nA2Len + nA3Len;

	if (!NatProtocol_StunNat_Packet(tszMsgBuffer, &nMsgLen, tszTokenStr, RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_CLASS_REQUEST, RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_METHOD_ALLOCATE, tszAttrBuffer))
	{
		printf("构建请求包缓冲区错误!\n");
		return -1;
	}
	if (!XClient_UDPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf("发送数据失败!,错误:%lX\n", XClient_GetLastError());
		return -1;
	}
	//如果服务器正常,但是收不到包,说明你的路由不支持NAT
	nMsgLen = 2048;
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	if (!XClient_UDPSelect_RecvMsg(m_Socket, tszMsgBuffer, &nMsgLen))
	{
		printf("接受数据失败!,错误:%lX\n", XClient_GetLastError());
		return -1;
	}

	XClient_UDPSelect_Close(m_Socket);
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}