﻿#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/NetHelp_DDSQueue.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_DDSQueue/XDDS_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_DDSQueue/XDDS_Error.h"

//g++ -std=c++17 -Wall -g NetHelp_APPPublish.cpp -o NetHelp_APPPublish.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp -lXEngine_BaseLib -lNetHelp_DDSQueue -ljsoncpp -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp,--disable-new-dtags

void CALLBACK DDSProtocol_Notify(int nNotifyEvent, int nDomainId, XENGINE_PROTOCOL_XDDS* pSt_XDDSProtocol, LPVOID lParam)
{
	printf("Notify:%d %d", nDomainId, nNotifyEvent);
}
int main(int argc, char** argv)
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
	if (!XDDS_CommApi_Init("192.168.1.10"))
#else
	if (!XDDS_CommApi_Init("192.168.1.4"))
#endif
	{
		printf("初始化失败!\n");
		return -1;
	}
	XDDS_CommApi_SetAttr(DDSProtocol_Notify);
	if (!XDDS_CommApi_DomainCreate(1000))
	{
		printf("XDDS_CommApi_DomainCreate 失败!\n");
		return -1;
	}
	if (!XDDS_CommApi_TopicCreate(1000, "aa"))
	{
		printf("XDDS_CommApi_TopicCreate 失败!\n");
		return -1;
	}
	printf("ok!\n");
	XNETHANDLE xhPublish;
	if (!XDDS_Publish_Create(&xhPublish, 1000, "aa"))
	{
		printf("XDDS_Publish_Create 失败!\n");
		return -1;
	}
	int nTime = 0;
	while (nTime < 20)
	{
		nTime++;
		XDDS_Publish_SendMsg(xhPublish, "hello", 5);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	XDDS_Publish_Delete(xhPublish);
	XDDS_CommApi_TopicDelete(1000, "aa");
	XDDS_CommApi_DomainDelete(1000);
	XDDS_CommApi_Destory();

#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}