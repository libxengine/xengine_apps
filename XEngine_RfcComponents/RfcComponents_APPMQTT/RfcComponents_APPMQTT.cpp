#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#else
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
using namespace std;

#if 1 == _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/NetCore_Define.h>
#include <XEngine_Include/XEngine_Core/NetCore_Error.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/MQTTProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/MQTTProtocol_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_Core/XEngine_Core.lib")
#pragma comment(lib,"XEngine_Client/XClient_Socket.lib")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_MQTTProtocol.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Core/NetCore_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Core/NetCore_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_ManagePool/ManagePool_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_MQTTProtocol/MQTTProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_MQTTProtocol/MQTTProtocol_Error.h"
#ifdef _MSC_BUILD
#ifdef _WIN64
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_Core.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XClient_Socket.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/RfcComponents_MQTTProtocol.lib")
#else
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Core.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_Socket.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_MQTTProtocol.lib")
#endif
#endif
#endif

//Linux::g++ -std=c++20 -Wall -g RfcComponents_APPMQTT.cpp -o RfcComponents_APPMQTT.exe -lXEngine_BaseLib -lXEngine_Core -lXClient_Socket -lRfcComponents_MQTTProtocol -lpthread 

bool RfcComponents_APPMQTT_Process(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen);

XHANDLE xhToken = NULL;

bool XCALLBACK NetCore_CB_Login(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	printf("NetCore_CB_Login:%s\n", lpszClientAddr);
	MQTTProtocol_Parse_Insert(lpszClientAddr);
	return true;
}
void XCALLBACK NetCore_CB_Recv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	printf("NetCore_CB_Recv:%s-%d\n", lpszClientAddr, nMsgLen);
	RfcComponents_APPMQTT_Process(lpszClientAddr, lpszRecvMsg, nMsgLen);
}
void XCALLBACK NetCore_CB_Close(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	printf("NetCore_CB_Close:%s\n", lpszClientAddr);
	MQTTProtocol_Parse_Delete(lpszClientAddr);
}

void Packet_Property(MQTTPROTOCOL_HDRPROPERTY*** pppSt_HDRProperty, int nListCount)
{
	BaseLib_Memory_Malloc((XPPPMEM)pppSt_HDRProperty, nListCount, sizeof(MQTTPROTOCOL_HDRPROPERTY));

	(*pppSt_HDRProperty)[0]->nProLen = 4;
	(*pppSt_HDRProperty)[0]->st_unValue.nValue = 1024000;
	(*pppSt_HDRProperty)[0]->byProFlag = XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_PROPERTY_PACKMAX;

	(*pppSt_HDRProperty)[1]->nProLen = 1;
	(*pppSt_HDRProperty)[1]->st_unValue.byValue = 1;
	(*pppSt_HDRProperty)[1]->byProFlag = XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_PROPERTY_REVERAVAI;

	(*pppSt_HDRProperty)[2]->nProLen = 1;
	(*pppSt_HDRProperty)[2]->st_unValue.byValue = 1;
	(*pppSt_HDRProperty)[2]->byProFlag = XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_PROPERTY_SHAREDSUBAVAI;

	(*pppSt_HDRProperty)[3]->nProLen = 1;
	(*pppSt_HDRProperty)[3]->st_unValue.byValue = 1;
	(*pppSt_HDRProperty)[3]->byProFlag = XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_PROPERTY_SUBIDAVAI;

	(*pppSt_HDRProperty)[4]->nProLen = 2;
	(*pppSt_HDRProperty)[4]->st_unValue.wValue = 65535;
	(*pppSt_HDRProperty)[4]->byProFlag = XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_PROPERTY_ALIASMAX;

	(*pppSt_HDRProperty)[5]->nProLen = 1;
	(*pppSt_HDRProperty)[5]->st_unValue.byValue = 1;
	(*pppSt_HDRProperty)[5]->byProFlag = XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_PROPERTY_WILDCARDSUBAVAI;
}
bool RfcComponents_APPMQTT_Process(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 0;
	int nRVLen = 0;
	XCHAR tszSDBuffer[1024] = {};
	XCHAR tszRVBuffer[1024] = {};
	MQTTPROTOCOL_FIXEDHEADER st_MQFixedHdr = {};

	MQTTProtocol_Parse_Send(lpszClientAddr, lpszMsgBuffer, nMsgLen);

	int nMSGLen = 0;
	XCHAR* ptszMSGBuffer = NULL;
	if (MQTTProtocol_Parse_Recv(lpszClientAddr, &st_MQFixedHdr, &ptszMSGBuffer, &nMsgLen))
	{
		//是不是连接
		if (XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_CONNECT == st_MQFixedHdr.byMsgType)
		{
			int nListCount = 0;
			MQTTPROTOCOL_HDRPROPERTY** ppSt_HDRProperty;
			MQTTPROTOCOL_HDRCONNNECT st_HDRConnect = {};
			MQTTPROTOCOL_USERINFO st_USerInfo = {};

			MQTTProtocol_Parse_Connect(ptszMSGBuffer, nMsgLen, &st_HDRConnect, &st_USerInfo, &ppSt_HDRProperty, &nListCount);
			BaseLib_Memory_Free((XPPPMEM)&ppSt_HDRProperty, nListCount);

			nListCount = 0;
			Packet_Property(&ppSt_HDRProperty, nListCount);
			MQTTProtocol_Packet_REPConnect(tszRVBuffer, &nRVLen, 0, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_REASON_SUCCESS, &ppSt_HDRProperty, nListCount);
			MQTTProtocol_Packet_Header(tszSDBuffer, &nSDLen, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_CONNACK, tszRVBuffer, nRVLen);
			NetCore_TCPXCore_SendEx(xhToken, lpszClientAddr, tszSDBuffer, nSDLen);
		}
		else if (XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_SUBSCRIBE == st_MQFixedHdr.byMsgType)
		{

		}
		else if (XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_UNSUBSCRIBE == st_MQFixedHdr.byMsgType)
		{
			
		}
		else if (XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_PUBLISH == st_MQFixedHdr.byMsgType)
		{
			
		}
		else if (XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_PINGREQ == st_MQFixedHdr.byMsgType)
		{
			
		}
	}
	return true;
}

int main1()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

	XSOCKET hSocket = 0;
	if (!XClient_TCPSelect_Create(&hSocket, "10.10.12.114", 1883))
	{
		printf("XClient_TCPSelect_Create:%lX\n", XClient_GetLastError());
		return 0;
	}
	int nSDLen = 0;
	int nRVLen = 0;
	XCHAR tszSDBuffer[1024];
	XCHAR tszRVBuffer[1024];
	MQTTPROTOCOL_HDRCONNNECT st_HDRConnect;

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));
	memset(&st_HDRConnect, '\0', sizeof(MQTTPROTOCOL_HDRCONNNECT));

	st_HDRConnect.byFlagsClean = 1;
	st_HDRConnect.wHBTime = 60;
	//连接
	MQTTProtocol_Packet_REQConnect(tszRVBuffer, &nRVLen, &st_HDRConnect, "mqtt_123123aa");
	MQTTProtocol_Packet_Header(tszSDBuffer, &nSDLen, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_CONNECT, tszRVBuffer, nRVLen, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_FLAG_CONNECT);
	memcpy(tszSDBuffer + nSDLen, tszRVBuffer, nRVLen);
	nSDLen += nRVLen;

	XClient_TCPSelect_SendMsg(hSocket, tszSDBuffer, nSDLen);
	//取消订阅
	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));

	MQTTProtocol_Packet_REQUNSubscribe(tszRVBuffer, &nRVLen, 1101, "topic/name");
	MQTTProtocol_Packet_Header(tszSDBuffer, &nSDLen, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_UNSUBSCRIBE, tszRVBuffer, nRVLen, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_FLAG_UNSUBSCRIBE);
	memcpy(tszSDBuffer + nSDLen, tszRVBuffer, nRVLen);
	nSDLen += nRVLen;

	XClient_TCPSelect_SendMsg(hSocket, tszSDBuffer, nSDLen);
	RfcComponents_APPMQTT_Process("1111", tszRVBuffer, nRVLen);
	//订阅
	MQTTPROTOCOL_HDRSUBSCRIBE st_HDROPtion;

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));
	memset(&st_HDROPtion, '\0', sizeof(MQTTPROTOCOL_HDRSUBSCRIBE));
	
	MQTTProtocol_Packet_REQSubscribe(tszRVBuffer, &nRVLen, 1102, "topic/name", &st_HDROPtion);
	MQTTProtocol_Packet_Header(tszSDBuffer, &nSDLen, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_SUBSCRIBE, tszRVBuffer, nRVLen, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_FLAG_SUBSCRIBE);
	memcpy(tszSDBuffer + nSDLen, tszRVBuffer, nRVLen);
	nSDLen += nRVLen;

	XClient_TCPSelect_SendMsg(hSocket, tszSDBuffer, nSDLen);
	//发布消息
	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));

	MQTTProtocol_Packet_REQPublish(tszRVBuffer, &nRVLen, "topic/name", "123", 3, 123);
	memcpy(tszRVBuffer + nRVLen, "hello", 5);
	nRVLen += 5;

	MQTTProtocol_Packet_Header(tszSDBuffer, &nSDLen, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_PUBLISH, tszRVBuffer, nRVLen, 0);
	memcpy(tszSDBuffer + nSDLen, tszRVBuffer, nRVLen);
	nSDLen += nRVLen;

	XClient_TCPSelect_SendMsg(hSocket, tszSDBuffer, nSDLen);
	//PING
	nRVLen = 0;
	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	MQTTProtocol_Packet_Header(tszSDBuffer, &nSDLen, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_PINGREQ, tszRVBuffer, nRVLen, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_FLAG_PINGREQ);
	XClient_TCPSelect_SendMsg(hSocket, tszSDBuffer, nSDLen);
	//断开连接
	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));

	MQTTProtocol_Packet_DisConnect(tszRVBuffer, &nRVLen);
	MQTTProtocol_Packet_Header(tszSDBuffer, &nSDLen, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_DISCONN, tszRVBuffer, nRVLen, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_FLAG_DISCONN);
	memcpy(tszSDBuffer + nSDLen, tszRVBuffer, nRVLen);
	nSDLen += nRVLen;

	XClient_TCPSelect_SendMsg(hSocket, tszSDBuffer, nSDLen);
	XClient_TCPSelect_Close(hSocket);
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;

}
int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

	MQTTProtocol_Parse_Init(1);
	xhToken = NetCore_TCPXCore_StartEx(1883);
	if (NULL == xhToken)
	{
		printf("%lX\n", NetCore_GetLastError());
		return 0;
	}
	NetCore_TCPXCore_RegisterCallBackEx(xhToken, NetCore_CB_Login, NetCore_CB_Recv, NetCore_CB_Close);

	while (1)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	MQTTProtocol_Parse_Destory();

#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}