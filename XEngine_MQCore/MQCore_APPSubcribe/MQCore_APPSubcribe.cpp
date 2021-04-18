#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/MQCore_XDDService.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_MQCore/MQCore_XDDService/XDDS_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_MQCore/MQCore_XDDService/XDDS_Error.h"

//g++ -std=c++17 -Wall -g MQCore_APPService.cpp -o MQCore_APPService.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia -lXEngine_BaseLib -lXClient_Socket -lMQCore_XMQService -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_MQCore,--disable-new-dtags

XNETHANDLE xhNetSub;
void CALLBACK XDDS_CommApi_Callback(int nNotifyEvent, int nDomainId, XENGINE_PROTOCOL_XDDS* pSt_XDDSProtocol, LPVOID lParam)
{
	printf("域ID:%d,主题:%s创建消息\n", nDomainId, pSt_XDDSProtocol->tszTopic);
	XDDS_SubScribe_Insert(&xhNetSub, pSt_XDDSProtocol);
}
int main(int argc, char** argv)
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

	if (!XDDS_CommApi_Init("192.168.1.10"))
	{
		printf("初始化失败!\n");
		return -1;
	}
	XDDS_CommApi_SetAttr(XDDS_CommApi_Callback);
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

	XNETHANDLE xhSubcribe;
	if (!XDDS_SubScribe_Create(&xhSubcribe, 1000, "aa"))
	{
		printf("XDDS_Publish_Create 失败!\n");
		return -1;
	}
	printf("ok!\n");
	int nTime = 0;
	while (nTime < 99999)
	{
		int nLen = 2048;
		TCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		if (XDDS_SubScribe_ReadMsg(xhSubcribe, tszMsgBuffer, &nLen))
		{
			nTime++;
			printf("recv:%d-%s\n", nLen, tszMsgBuffer);
		}
		if (XDDS_SubScribe_ReadMsg(xhNetSub, tszMsgBuffer, &nLen))
		{
			nTime++;
			printf("recv notify:%d-%s\n", nLen, tszMsgBuffer);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	XDDS_SubScribe_Delete(xhSubcribe);
	XDDS_CommApi_TopicDelete(1000, "aa");
	XDDS_CommApi_DomainDelete(1000);
	XDDS_CommApi_Destory();
#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}