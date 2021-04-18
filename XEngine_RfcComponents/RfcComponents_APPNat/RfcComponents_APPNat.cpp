#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_Socket.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_NatClient.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_NatClient/NatClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_NatClient/NatClient_Error.h"

//g++ -std=c++17 -Wall -g RfcComponents_APPNat.cpp -o RfcComponents_APPNat.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents -lXEngine_BaseLib -lXClient_Socket -lRfcComponents_NatClient -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents,--disable-new-dtags


int main()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	SOCKET m_Socket;
	int nMsgLen = 2048;
	TCHAR tszMsgBuffer[2048];
	UINT nToken[4];
	RFCCOMPONENTS_NATSTUN st_NatClient;

	memset(&st_NatClient, '\0', sizeof(RFCCOMPONENTS_NATSTUN));
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&nToken, '\0', sizeof(nToken));

	if (!XClient_UDPSelect_Create(&m_Socket, "42.192.166.120", 3478))
	{
		printf("创建套接字失败!,错误:%lX\n", NatClient_GetLastError());
		return -1;
	}
	XClient_UDPSelect_Bind(m_Socket, 3478);

	if (!RfcComponents_StunNat_Request(tszMsgBuffer, &nMsgLen, RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_BINDREQ, nToken))
	{
		printf("构建请求包缓冲区错误!\n");
		return -1;
	}
	if (!XClient_UDPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf("发送数据失败!,错误:%lX\n", XClient_GetLastError());
		return -1;
	}

	nMsgLen = 2048;
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	if (!XClient_UDPSelect_RecvMsg(m_Socket, tszMsgBuffer, &nMsgLen, FALSE))
	{
		printf("接受数据失败!,错误:%lX\n", XClient_GetLastError());
		return -1;
	}

	if (!RfcComponents_StunNat_Response(tszMsgBuffer, nMsgLen, &st_NatClient))
	{
		printf("处理数据失败!,错误:%lX\n", NatClient_GetLastError());
		return -1;
	}
	printf("%s %d %d %d %d %s %d %d\n", st_NatClient.tszServerVer, st_NatClient.xhToken[0], st_NatClient.xhToken[1], st_NatClient.xhToken[2], st_NatClient.xhToken[3], st_NatClient.st_MappedAddr.tszIPAddr, st_NatClient.st_ChangeAddr.nPort, st_NatClient.st_SrcAddr.nFamily);
	XClient_UDPSelect_Close(m_Socket);

#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}