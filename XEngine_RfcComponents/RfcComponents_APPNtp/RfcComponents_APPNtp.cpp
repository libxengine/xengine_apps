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
#include <XEngine_Include/XEngine_RfcComponents/NTPProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/NTPProtocol_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_Client/XClient_Socket.lib")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_NTPProtocol.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_NTPProtocol/NTPProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_NTPProtocol/NTPProtocol_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_Socket.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_NTPProtocol.lib")
#endif
#endif
//Linux::g++ -std=c++20 -Wall -g RfcComponents_APPNtp.cpp -o RfcComponents_APPNtp.exe -lXEngine_BaseLib -lXClient_Socket -lRfcComponents_NTPProtocol


int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
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
	XClient_UDPSelect_Connect(m_Socket, "20.189.79.72", 123);

	NTPProtocol_Packet_REQHeader(tszMsgBuffer, &nMsgLen);
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
	__int64x nTimeoffset = 0;
	__int64x nTimeDelay = 0;
	int nTimePoll = 0;
	int dlTimePrecision = 0;

	NTPPROTOCOL_TIMEINFO st_ORGTime = {};
	NTPPROTOCOL_TIMEINFO st_REVTime = {};
	NTPPROTOCOL_TIMEINFO st_REFTime = {};
	NTPPROTOCOL_TIMEINFO st_TRSTime = {};
	XCHAR tszIPAddr[128] = {};

	NTPProtocol_Parse_Header(tszMsgBuffer, nMsgLen, &st_REFTime, &st_REVTime, &st_ORGTime, &st_TRSTime, &nTimePoll, &dlTimePrecision, tszIPAddr);
	NTPProtocol_Parse_TimeInfo(&st_REVTime, &st_ORGTime, &st_TRSTime, &nTimeoffset, &nTimeDelay);
	printf("nTimeoffset:%lld,nTimeDelay:%lld\n", nTimeoffset, nTimeDelay);
	XClient_UDPSelect_Close(m_Socket);
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}