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
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/SnmpProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/SnmpProtocol_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_Client/XClient_Socket.lib")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_SnmpProtocol.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_SnmpProtocol/SnmpProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_SnmpProtocol/SnmpProtocol_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_Socket.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_SnmpProtocol.lib")
#endif
#endif
//Linux::g++ -std=c++20 -Wall -g RfcComponents_APPSnmp.cpp -o RfcComponents_APPSnmp.exe -lXEngine_BaseLib -lXClient_Socket -lRfcComponents_SnmpProtocol 

int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	XSOCKET hSocket;
	int nMsgLen = 0;
	int nPort = 161;
	XCHAR tszMsgBuffer[1024];
	LPCXSTR lpszSnmpAddr = "118.25.14.242";
	LPCXSTR lpszOIDStr = "1.3.6.1.2.1.1.1";
	RFCSNMP_PROTOCOL st_SNMPProtocol;

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_SNMPProtocol, '\0', sizeof(RFCSNMP_PROTOCOL));

	st_SNMPProtocol.byVersion = XENGINE_RFCCOMPONENTS_SNMP_PROTOCOL_V2;
	st_SNMPProtocol.byOPCode = XENGINE_SNMP_PROTOCOL_BER_TYPE_GETNEXT;
	strcpy(st_SNMPProtocol.tszCommname, "public");

	st_SNMPProtocol.st_PacketInfo.dwSerial = 698329327;

	RfcComponents_SnmpHelp_StrToOID(lpszOIDStr, st_SNMPProtocol.st_BindVar.tszOIDStr, &st_SNMPProtocol.st_BindVar.nOLen);
	RfcComponents_SnmpPacket_Protocol(tszMsgBuffer, &nMsgLen, &st_SNMPProtocol);

	if (!XClient_UDPSelect_Create(&hSocket))
	{
		printf("XClient_UDPSelect_Create:%lX\n", XClient_GetLastError());
		return -1;
	}
	if (!XClient_UDPSelect_Bind(hSocket, nPort + 10000))
	{
		printf("XClient_UDPSelect_Bind:%lX\n", XClient_GetLastError());
		return -1;
	}
	if (!XClient_UDPSelect_Connect(hSocket, lpszSnmpAddr, nPort))
	{
		printf("XClient_UDPSelect_Connect:%lX\n", XClient_GetLastError());
		return -1;
	}
	if (!XClient_UDPSelect_SendMsg(hSocket, tszMsgBuffer, nMsgLen))
	{
		printf("XClient_UDPSelect_SendMsg:%lX\n", XClient_GetLastError());
		return -1;
	}
	nMsgLen = sizeof(tszMsgBuffer);
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_SNMPProtocol, '\0', sizeof(RFCSNMP_PROTOCOL));
	if (!XClient_UDPSelect_RecvMsg(hSocket, tszMsgBuffer, &nMsgLen, NULL, false))
	{
		printf("XClient_UDPSelect_RecvMsg:%lX\n", XClient_GetLastError());
		return -1;
	}
	
	int nOLen = 0;
	XCHAR tszOIDBuffer[128];
	memset(tszOIDBuffer, '\0', sizeof(tszOIDBuffer));
	RfcComponents_SnmpParse_Protocol(tszMsgBuffer, nMsgLen, &st_SNMPProtocol);
	RfcComponents_SnmpHelp_OIDToStr(st_SNMPProtocol.st_BindVar.tszOIDStr, st_SNMPProtocol.st_BindVar.nOLen, tszOIDBuffer, &nOLen);
	printf("%s:%s\n", st_SNMPProtocol.tszCommname, st_SNMPProtocol.st_BindVar.tszTetStr);
	XClient_UDPSelect_Close(hSocket);
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}