#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_P2XPProtocol.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_P2XPDistribution.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_P2XPClient.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_P2xp/XEngine_P2XPProtocol/P2XPProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_P2xp/XEngine_P2XPProtocol/P2XPProtocol_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_P2xp/XEngine_P2XPDistribution/P2XPDistribution_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_P2xp/XEngine_P2XPDistribution/P2XPDistribution_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_P2xp/XEngine_P2XPClient/P2XPClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_P2xp/XEngine_P2XPClient/P2XPClient_Error.h"

//g++ -std=c++17 -Wall -g P2XP_APPClient.cpp -o P2XP_APPClient.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_P2xp -lXEngine_P2XPProtocol -lXEngine_P2XPDistribution -lXEngine_P2XPClient -lpthread -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_HelpComponents:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_P2xp,--disable-new-dtags

XNETHANDLE xhClient;
LPCTSTR lpszAddr = _T("192.168.1.10");
LPCTSTR lpszUser = _T("123123aa");
LPCTSTR lpszPass = _T("123123");

void CALLBACK P2XPClient_Callback(LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, LPVOID lParam)
{
	switch (pSt_ProtocolHdr->unOperatorCode)
	{
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPLOGIN:
		printf("P2XPClient_Callback:%s\n", lpszMsgBuffer);
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REQCONNECT:
		XENGINE_P2XPIO_PROTOCOL st_IONet;
		memset(&st_IONet, '\0', sizeof(XENGINE_P2XPIO_PROTOCOL));
		memcpy(&st_IONet, lpszMsgBuffer, sizeof(XENGINE_P2XPIO_PROTOCOL));
		P2XPDistribution_IONetWork_Create(&xhClient, st_IONet.tszConnectAddr, st_IONet.nDestPort, st_IONet.bIsTcp);
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REPCONNECT:
		printf("P2XPClient_Callback:%s\n", lpszMsgBuffer);
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REPLANLIST:
		printf("P2XPClient_Callback:%s\n", lpszMsgBuffer);
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REPWLANLIST:
		printf("P2XPClient_Callback:%s\n", lpszMsgBuffer);
		break;
	case XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_P2XP_REPUSERQUERY:
		printf("P2XPClient_Callback:%s\n", lpszMsgBuffer);
		break;
	default:
		break;
	}
}
void CALLBACK P2XPClient_CB_ClinetNet(LPCSTR lpszAddr, LPCSTR lpszBufferMsg, int nLen, BOOL bIsTcp, LPVOID lParam)
{

}
void CALLBACK P2XPClient_CB_ServiceNet(LPCSTR lpszAddr, LPCSTR lpszBufferMsg, int nLen, BOOL bIsTcp, LPVOID lParam)
{
}

int P2XPClient_Register()
{

	return 0;
}
int P2XPClient_Login()
{
	int nLen;
	TCHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	XENGINE_P2XPPEER_PROTOCOL st_ClientInfo;
	XENGINE_PROTOCOL_USERAUTH st_UserInfo;

	memset(&st_ClientInfo, '\0', sizeof(XENGINE_P2XPPEER_PROTOCOL));
	memset(&st_UserInfo, '\0', sizeof(XENGINE_PROTOCOL_USERAUTH));

	if (!P2XPClinet_Help_GetInfo(&st_ClientInfo))
	{
		printf(_T("获取信息失败"));
		return -1;
	}
	strcpy(st_UserInfo.tszUserName, lpszUser);
	strcpy(st_UserInfo.tszPassword, lpszPass);
	st_UserInfo.enClientType = ENUM_PROTOCOL_FOR_SERVICE_TYPE_USER;
	st_UserInfo.enDeviceType = ENUM_PROTOCOL_FOR_DEVICE_TYPE_PC;
	if (!P2XPProtocol_UserMsg_RequestLogin(&st_ClientInfo, &st_UserInfo, tszMsgBuffer, &nLen))
	{
		printf(_T("组包失败"));
		return -1;
	}
	if (!P2XPClient_CMDTransmission_Send(tszMsgBuffer, nLen))
	{
		printf(_T("发送失败"));
		return -1;
	}
	return 0;
}
int P2XPClient_CreateService()
{
	if (!P2XPDistribution_IONetWork_Init(10093, 10094))
	{
		printf(_T("创建失败!"));
	}
	else
	{
		printf(_T("创建成功!"));
	}
	P2XPDistribution_IONetWork_SetCallBack(P2XPClient_CB_ClinetNet, P2XPClient_CB_ServiceNet);
	return 0;
}

int P2XPClient_ListLAN()
{
	int nLen = 2048;
	TCHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	P2XPProtocol_UserMsg_RequestLan(tszMsgBuffer, &nLen, FALSE, 1000);
	P2XPClient_CMDTransmission_Send(tszMsgBuffer, nLen);
	return 0;
}
int P2XPClient_ListWAN()
{
	int nLen = 2048;
	TCHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	P2XPProtocol_UserMsg_RequestLan(tszMsgBuffer, &nLen, TRUE);
	P2XPClient_CMDTransmission_Send(tszMsgBuffer, nLen);
	return 0;
}

int P2XPClient_QueryUser()
{
	int nLen = 2048;
	TCHAR tszMsgBuffer[2048];
	XENGINE_P2XPPEER_PROTOCOL st_P2XPUser;

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_P2XPUser, '\0', sizeof(XENGINE_P2XPPEER_PROTOCOL));

	strcpy(st_P2XPUser.tszUserName, lpszUser);
	P2XPProtocol_UserMsg_RequestQueryUser(&st_P2XPUser, tszMsgBuffer, &nLen);
	P2XPClient_CMDTransmission_Send(tszMsgBuffer, nLen);
	return 0;
}
int P2XPClient_ConnectWithme()
{
	XENGINE_P2XPIO_PROTOCOL st_IONet;
	memset(&st_IONet, '\0', sizeof(XENGINE_P2XPIO_PROTOCOL));

	st_IONet.bIsTcp = TRUE;
	st_IONet.nDestPort = 10093;
	strcpy(st_IONet.tszConnectAddr, lpszAddr);
	strcpy(st_IONet.tszSourceUser, lpszUser);
	strcpy(st_IONet.tszDestUser, "aaadddzxc");

	int nLen = 2048;
	TCHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	P2XPProtocol_UserMsg_RequestConnect(&st_IONet, tszMsgBuffer, &nLen);
	P2XPClient_CMDTransmission_Send(tszMsgBuffer, nLen);
	return 0;
}


int main()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

	P2XPClient_Register();
	P2XPClient_Login();
	P2XPClient_CreateService();

#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}