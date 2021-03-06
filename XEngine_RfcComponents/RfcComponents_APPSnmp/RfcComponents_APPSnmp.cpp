#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_Socket.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_SnmpProtocol.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_SnmpProtocol/SnmpProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_SnmpProtocol/SnmpProtocol_Error.h"

//Linux::g++ -std=c++17 -Wall -g RfcComponents_APPSnmp.cpp -o RfcComponents_APPSnmp.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents -lXEngine_BaseLib -lXClient_Socket -lRfcComponents_SnmpProtocol -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g RfcComponents_APPSnmp.cpp -o RfcComponents_APPSnmp.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_Client -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_RfcComponents -lXEngine_BaseLib -lXClient_Socket -lRfcComponents_SnmpProtocol

int main()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	SOCKET hSocket;
	int nMsgLen = 0;
	int nPort = 161;
	TCHAR tszMsgBuffer[1024];
	LPCTSTR lpszSnmpAddr = "192.168.1.12";
	LPCTSTR lpszOIDStr = "1.3.6.1.2.1.1.1";
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
	if (!XClient_UDPSelect_RecvMsg(hSocket, tszMsgBuffer, &nMsgLen, NULL, FALSE))
	{
		printf("XClient_UDPSelect_RecvMsg:%lX\n", XClient_GetLastError());
		return -1;
	}
	
	int nOLen = 0;
	TCHAR tszOIDBuffer[128];
	memset(tszOIDBuffer, '\0', sizeof(tszOIDBuffer));
	RfcComponents_SnmpParse_Protocol(tszMsgBuffer, nMsgLen, &st_SNMPProtocol);
	RfcComponents_SnmpHelp_OIDToStr(st_SNMPProtocol.st_BindVar.tszOIDStr, st_SNMPProtocol.st_BindVar.nOLen, tszOIDBuffer, &nOLen);
	printf("%s:%s\n", st_SNMPProtocol.tszCommname, st_SNMPProtocol.st_BindVar.tszTetStr);
	XClient_UDPSelect_Close(hSocket);
#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}