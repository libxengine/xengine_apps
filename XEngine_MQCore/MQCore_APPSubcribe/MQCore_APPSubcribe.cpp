#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
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
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_MQCore/MQCore_XDDService/XDDS_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_MQCore/MQCore_XDDService/XDDS_Error.h"

//g++ -std=c++17 -Wall -g MQCore_APPSubcribe.cpp -o MQCore_APPSubcribe.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_MQCore -lXEngine_BaseLib -lMQCore_XDDService -ljsoncpp -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_MQCore,--disable-new-dtags


void CALLBACK XDDS_CommApi_Callback(int nNotifyEvent, int nDomainId, XENGINE_PROTOCOL_XDDS* pSt_XDDSProtocol, LPVOID lParam)
{
	printf("域ID:%d,主题:%s创建消息\n", nDomainId, pSt_XDDSProtocol->tszTopic);
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
	while (nTime < 10)
	{
		int nListCount = 0;
		DDSSUBSCIRBE_MSGBUFFER** pppSt_MsgBuffer;

		XDDS_SubScribe_ReadMsg(xhSubcribe, &pppSt_MsgBuffer, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			printf("recv:%d = %s\n", pppSt_MsgBuffer[i]->nMsgLen, pppSt_MsgBuffer[i]->tszMsgBuffer);
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&pppSt_MsgBuffer, nListCount);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		nTime++;
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