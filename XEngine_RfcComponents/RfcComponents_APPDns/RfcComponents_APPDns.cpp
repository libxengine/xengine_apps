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
#include <XEngine_Include/XEngine_RfcComponents/DNSProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/DNSProtocol_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_Client/XClient_Socket.lib")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_DNSProtocol.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_DNSProtocol/DNSProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_DNSProtocol/DNSProtocol_Error.h"
#ifdef _MSC_BUILD
#ifdef _WIN64
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XClient_Socket.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/RfcComponents_DNSProtocol.lib")
#else
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_Socket.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_DNSProtocol.lib")
#endif
#endif
#endif
//Linux::g++ -std=c++20 -Wall -g RfcComponents_APPDns.cpp -o RfcComponents_APPDns.exe -lXEngine_BaseLib -lXClient_Socket -lRfcComponents_DNSProtocol

int RfcComponents_APPDns_Domain()
{
	XSOCKET m_Socket;
	int nMsgLen = 0;
	XCHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	if (!XClient_UDPSelect_Create(&m_Socket))
	{
		printf("创建套接字失败!,错误:%lX\n", XClient_GetLastError());
		return -1;
	}
	XClient_UDPSelect_Bind(m_Socket, 12333);
	XClient_UDPSelect_Connect(m_Socket, "127.0.0.1", 53);

	DNSProtocol_Packet_REQHeader(tszMsgBuffer, &nMsgLen, 123, "www.baidu.com", XENGINE_DNSPROTOCOL_QUERY_QUESTION_TYPE_A);

	if (!XClient_UDPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf("发送数据失败!,错误:%lX\n", XClient_GetLastError());
		return -1;
	}

	nMsgLen = 2048;
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	if (!XClient_UDPSelect_RecvMsg(m_Socket, tszMsgBuffer, &nMsgLen))
	{
		printf("接受数据失败!,错误:%lX\n", XClient_GetLastError());
		return -1;
	}

	XENGINE_DNSPROTOCOL st_DNSRequest = {};

	int nRequestID = 0;
	int nListCount = 0;
	XENGINE_DNSPROTOCOL** ppSt_DNSAnswer;
	DNSProtocol_Parse_Header(tszMsgBuffer, nMsgLen, &nRequestID, &st_DNSRequest, &ppSt_DNSAnswer, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		printf("name:%s addr:%s %d %d\n", ppSt_DNSAnswer[i]->tszNameStr, ppSt_DNSAnswer[i]->tszAddrStr, ppSt_DNSAnswer[i]->st_QueryInfo.nNameType, ppSt_DNSAnswer[i]->nTTL);
	}
	BaseLib_Memory_Free((XPPPMEM)&ppSt_DNSAnswer, nListCount);

	XClient_UDPSelect_Close(m_Socket);
	return 0;
}
int RfcComponents_APPDns_IPAddr()
{
	XSOCKET m_Socket;
	int nMsgLen = 0;
	XCHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	if (!XClient_UDPSelect_Create(&m_Socket))
	{
		printf("创建套接字失败!,错误:%lX\n", XClient_GetLastError());
		return -1;
	}
	XClient_UDPSelect_Bind(m_Socket, 12333);
	XClient_UDPSelect_Connect(m_Socket, "127.0.0.1", 53);

	DNSProtocol_Packet_REQHeader(tszMsgBuffer, &nMsgLen, 123, "104.21.4.128", XENGINE_DNSPROTOCOL_QUERY_QUESTION_TYPE_PTR);

	if (!XClient_UDPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf("发送数据失败!,错误:%lX\n", XClient_GetLastError());
		return -1;
	}

	nMsgLen = 2048;
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	if (!XClient_UDPSelect_RecvMsg(m_Socket, tszMsgBuffer, &nMsgLen))
	{
		printf("接受数据失败!,错误:%lX\n", XClient_GetLastError());
		return -1;
	}

	XENGINE_DNSPROTOCOL st_DNSRequest = {};

	int nRequestID = 0;
	int nListCount = 0;
	XENGINE_DNSPROTOCOL** ppSt_DNSAnswer;
	DNSProtocol_Parse_Header(tszMsgBuffer, nMsgLen, &nRequestID, &st_DNSRequest, &ppSt_DNSAnswer, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		printf("name:%s addr:%s %d %d\n", ppSt_DNSAnswer[i]->tszNameStr, ppSt_DNSAnswer[i]->tszAddrStr, ppSt_DNSAnswer[i]->st_QueryInfo.nNameType, ppSt_DNSAnswer[i]->nTTL);
	}
	BaseLib_Memory_Free((XPPPMEM)&ppSt_DNSAnswer, nListCount);

	XClient_UDPSelect_Close(m_Socket);
	return 0;
}
int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

	int nIPVer = 0;
	XCHAR tszIPStr[128] = {};
	LPCXSTR lpszIPV4 = "4.3.2.1.in-addr.arpa";
	DNSProtocol_Help_PTRStr(lpszIPV4, tszIPStr, &nIPVer);
	
	LPCXSTR lpszIPV6 = "b.a.9.8.7.6.5.4.3.2.1.0.f.e.d.c.b.a.9.8.7.6.5.4.3.2.1.0.f.e.d.c.b.a.ip6.arpa";
	DNSProtocol_Help_PTRStr(lpszIPV6, tszIPStr, &nIPVer);

	RfcComponents_APPDns_Domain();
	RfcComponents_APPDns_IPAddr();
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}