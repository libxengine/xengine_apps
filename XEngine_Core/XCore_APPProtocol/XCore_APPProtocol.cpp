#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <tchar.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib,"Ws2_32.lib")
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
using namespace std;

#ifdef _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/Protocol_Define.h>
#include <XEngine_Include/XEngine_Core/Protocol_Error.h>
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_Core/XEngine_Protocol.lib")
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Protocol/Protocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Protocol/Protocol_Error.h"
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Protocol.lib")
#endif

//linux:g++ -std=gnu++17 -Wall -g XCore_APPProtocol.cpp -o XCore_APPProtocol.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -lXEngine_BaseLib -lXEngine_Protocol -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core,--disable-new-dtags
//macos:g++ -std=gnu++17 -Wall -g XCore_APPProtocol.cpp -o XCore_APPProtocol.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_Core -lXEngine_BaseLib -lXEngine_Protocol

bool CALLBACK XEngine_Protocol_Callback_Trace(LPCXSTR lpszSource, LPCXSTR lpszDestAddr, LPCXSTR lpszRecvAddr, int nTTL, XENGINE_VALTIME st_VALTime, XPVOID lParam)
{
	printf("Src:%s Dst:%s Rcv:%s TTL:%d Time:%lld.%llu\n", lpszSource, lpszDestAddr, lpszRecvAddr, nTTL, st_VALTime.tv_sec, st_VALTime.tv_usec);
	return true;
}
int Protocol_TestPing()
{
	LPCXSTR lpszSourceAddr = _X("192.168.252.128");
	LPCXSTR lpszDestAddr = _X("42.194.178.57");
	XENGINE_VALTIME st_VALTime;

	memset(&st_VALTime, '\0', sizeof(XENGINE_VALTIME));

	if (!Protocol_Icmp_Ping(lpszSourceAddr, lpszDestAddr, &st_VALTime))
	{
		printf("errno\n");
		return 0;
	}

	printf("%lld %llu\n", st_VALTime.tv_sec, st_VALTime.tv_usec);

	return 0;
}

int Protocol_TestTrace()
{
	LPCXSTR lpszSourceAddr = _X("192.168.252.128");
	LPCXSTR lpszDestAddr = _X("42.194.178.57");

	if (!Protocol_Icmp_Traceroute(lpszSourceAddr, lpszDestAddr, XEngine_Protocol_Callback_Trace))
	{
		printf("errno\n");
		return 0;
	}

	return 0;
}
//ONLY LINUX
int Protocol_TestTCPRaw()
{
	XSOCKET hSDSocket;
	XSOCKET hRVSocket;
	LPCXSTR lpszSourceAddr = "192.168.1.8";

	if (!Protocol_TCPRaw_Init(&hSDSocket, &hRVSocket))
	{
		printf("errno\n");
		return 0;
	}
	for (int i = 7; i < 255; i++)
	{
		int nMsgLen = 2048;
		XCHAR tszMsgBuffer[1024];
		PROTOCOL_RAWSOCKET_NETPARAM st_RAWSocket;

		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		memset(&st_RAWSocket, '\0', sizeof(PROTOCOL_RAWSOCKET_NETPARAM));

		st_RAWSocket.usTTL = 10;
		st_RAWSocket.nSPort = 1080;
		st_RAWSocket.nDPort = 1080;
#ifdef _MSC_BUILD
		st_RAWSocket.nIDProfile = GetCurrentProcessId();
#else
		st_RAWSocket.nIDProfile = getpid();
#endif

		strcpy(st_RAWSocket.tszSrcAddr, lpszSourceAddr);
		sprintf(st_RAWSocket.tszDstAddr, "192.168.1.%d", i);

		st_RAWSocket.st_TCPHdr.nSequeue = 1;
		st_RAWSocket.st_TCPHdr.nFlag = 0x02;
		st_RAWSocket.st_TCPHdr.nWinSize = htons(2000);
		Protocol_TCPRaw_Packet(tszMsgBuffer, &nMsgLen, &st_RAWSocket);

		struct sockaddr_in st_SockAddr;
		memset(&st_SockAddr, '\0', sizeof(struct sockaddr_in));

		st_SockAddr.sin_port = htons(1080);
		st_SockAddr.sin_family = AF_INET;
		inet_pton(AF_INET, st_RAWSocket.tszDstAddr, &st_SockAddr.sin_addr);

		int nRet = sendto(hSDSocket, tszMsgBuffer, nMsgLen, 0, (struct sockaddr*)&st_SockAddr, sizeof(struct sockaddr_in));
		if (nRet <= 0)
		{
			printf("%d\n", errno);
			continue;
		}

		nRet = recvfrom(hRVSocket, tszMsgBuffer, sizeof(tszMsgBuffer), 0, NULL, NULL);
		if (nRet > 0)
		{
			memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
			memset(&st_RAWSocket, '\0', sizeof(PROTOCOL_RAWSOCKET_NETPARAM));

			Protocol_TCPRaw_Parse(tszMsgBuffer, nRet, &st_RAWSocket);
			continue;
		}
	}
	return 0;
}

int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	
	Protocol_TestPing();
	Protocol_TestTrace();
	Protocol_TestTCPRaw();

#ifdef _MSC_BUILD
	WSACleanup();
#endif
	std::this_thread::sleep_for(std::chrono::seconds(10));
	return 0;
}