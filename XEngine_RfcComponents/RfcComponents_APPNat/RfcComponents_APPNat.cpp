#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_Socket.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_NatClient.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_NatClient/NatClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_NatClient/NatClient_Error.h"

//Linux::g++ -std=c++17 -Wall -g RfcComponents_APPNat.cpp -o RfcComponents_APPNat.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents -lXEngine_BaseLib -lXClient_Socket -lRfcComponents_NatClient -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g RfcComponents_APPNat.cpp -o RfcComponents_APPNat.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_Client -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_RfcComponents -lXEngine_BaseLib -lXClient_Socket -lRfcComponents_NatClient


int main()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	SOCKET m_Socket;
	int nMsgLen = 2048;
	TCHAR tszMsgBuffer[2048];
	UINT nToken[3];
	RFCCOMPONENTS_NATSTUN st_NatClient;

	memset(&st_NatClient, '\0', sizeof(RFCCOMPONENTS_NATSTUN));
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&nToken, '\0', sizeof(nToken));

	if (!XClient_UDPSelect_Create(&m_Socket, "159.75.200.173", 3478))
	{
		printf("创建套接字失败!,错误:%lX\n", NatClient_GetLastError());
		return -1;
	}
	XClient_UDPSelect_Bind(m_Socket, 3478);

	if (!RfcComponents_StunNat_Request(tszMsgBuffer, &nMsgLen, nToken, RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_CLASS_REQUEST, RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_ATTR_MAPPED_ADDRESS))
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
	if (!XClient_UDPSelect_RecvMsg(m_Socket, tszMsgBuffer, &nMsgLen, FALSE))
	{
		printf("接受数据失败!,错误:%lX\n", XClient_GetLastError());
		return -1;
	}
	int nListCount = 0;
	RFCCOMPONENTS_NATATTR** ppSt_ListAttr;
	if (!RfcComponents_StunNat_Response(tszMsgBuffer, nMsgLen, &st_NatClient, &ppSt_ListAttr, &nListCount))
	{
		printf("处理数据失败!,错误:%lX\n", NatClient_GetLastError());
		return -1;
	}
	printf("%s %d %d %d\n", st_NatClient.tszServiceName, st_NatClient.xhToken[0], st_NatClient.xhToken[1], st_NatClient.xhToken[2]);

	for (int i = 0; i < nListCount; i++)
	{
		if (RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_ATTR_ERROR_CODE == ppSt_ListAttr[i]->wAttr)
		{
			int nErrorCode = 0;
			int nMsgLen = 0;
			TCHAR tszMsgBuffer[2048];
			memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

			RfcComponents_StunNat_ParseError(ppSt_ListAttr[i], &nErrorCode, tszMsgBuffer, &nMsgLen);
		}
		else if (RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_ATTR_XOR_MAPPED_ADDRESS == ppSt_ListAttr[i]->wAttr)
		{
			int nIPVer = 0;
			WORD wXor = 0;
			TCHAR tszIPAddr[128];
			memset(tszIPAddr, '\0', sizeof(tszIPAddr));

			memcpy(&wXor, &st_NatClient.unMagic, sizeof(WORD));

			RfcComponents_StunNat_ParseAddr(ppSt_ListAttr[i], &nIPVer, tszIPAddr, wXor);
			printf("%d %s\n", nIPVer, tszIPAddr);
		}
		else if (RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_ATTR_MAPPED_ADDRESS == ppSt_ListAttr[i]->wAttr)
		{
			int nIPVer = 0;
			TCHAR tszIPAddr[128];
			memset(tszIPAddr, '\0', sizeof(tszIPAddr));

			RfcComponents_StunNat_ParseAddr(ppSt_ListAttr[i], &nIPVer, tszIPAddr);
			printf("%d %s\n", nIPVer, tszIPAddr);
		}
		else if (RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_ATTR_SERVERADDR == ppSt_ListAttr[i]->wAttr)
		{
			int nIPVer = 0;
			TCHAR tszIPAddr[128];
			memset(tszIPAddr, '\0', sizeof(tszIPAddr));

			RfcComponents_StunNat_ParseAddr(ppSt_ListAttr[i], &nIPVer, tszIPAddr);
			printf("%d %s\n", nIPVer, tszIPAddr);
		}
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListAttr, nListCount);

	int nA1Len = 0;
	int nA2Len = 0;
	int nA3Len = 0;
	TCHAR tszAttrBuffer[MAX_PATH];

	memset(tszAttrBuffer, '\0', MAX_PATH);
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	RfcComponents_StunNat_BuildTransPort(tszAttrBuffer, &nA1Len);
	RfcComponents_StunNat_BuildLeftTime(tszAttrBuffer + nA1Len, &nA2Len);
	RfcComponents_StunNat_BuildAddrFamily(tszAttrBuffer + nA1Len + nA2Len, &nA3Len);
	nMsgLen = nA1Len + nA2Len + nA3Len;

	if (!RfcComponents_StunNat_Request(tszMsgBuffer, &nMsgLen, nToken, RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_CLASS_REQUEST, RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_METHOD_ALLOCATE, tszAttrBuffer))
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
	if (!XClient_UDPSelect_RecvMsg(m_Socket, tszMsgBuffer, &nMsgLen, FALSE))
	{
		printf("接受数据失败!,错误:%lX\n", XClient_GetLastError());
		return -1;
	}

	XClient_UDPSelect_Close(m_Socket);
#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}