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
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_MQTTProtocol/MQTTProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_MQTTProtocol/MQTTProtocol_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Core.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_Socket.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_MQTTProtocol.lib")
#endif
#endif

//Linux::g++ -std=c++17 -Wall -g RfcComponents_APPMQTT.cpp -o RfcComponents_APPMQTT.exe -lXEngine_BaseLib -lXEngine_Core -lXClient_Socket -lRfcComponents_MQTTProtocol -lpthread 

bool RfcComponents_APPMQTT_Process(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen);

XHANDLE xhToken = NULL;
int nTmpLen = 0;
XCHAR tszTmpBuffer[2048];

bool CALLBACK NetCore_CB_Login(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	printf("NetCore_CB_Login:%s\n", lpszClientAddr);
	return true;
}
void CALLBACK NetCore_CB_Recv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	printf("NetCore_CB_Recv:%s-%d\n", lpszClientAddr, nMsgLen);

	memcpy(tszTmpBuffer + nTmpLen, lpszRecvMsg, nMsgLen);
	nTmpLen += nMsgLen;
	if (!MQTTProtocol_Parse_GetHdr(tszTmpBuffer, nTmpLen))
	{
		nTmpLen = nMsgLen;
		return;
	}
	RfcComponents_APPMQTT_Process(lpszClientAddr, tszTmpBuffer, nTmpLen);
	nTmpLen = 0;
	memset(tszTmpBuffer, '\0', sizeof(tszTmpBuffer));
	
}
void CALLBACK NetCore_CB_Close(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	printf("NetCore_CB_Close:%s\n", lpszClientAddr);
}

void Packet_Property(XCHAR* ptszMsgBuffer, int* pInt_Len, MQTTPROTOCOL_HDRPROPERTY*** pppSt_HDRProperty, int nListCount)
{
	BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_HDRProperty, nListCount, sizeof(MQTTPROTOCOL_HDRPROPERTY));

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
	XCHAR tszSDBuffer[1024];
	XCHAR tszRVBuffer[1024];

	MQTTPROTOCOL_INFORMATION st_MQTTProtcol;

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));
	memset(&st_MQTTProtcol, '\0', sizeof(MQTTPROTOCOL_INFORMATION));

	MQTTProtocol_Parse_Header(lpszMsgBuffer, nMsgLen, &st_MQTTProtcol);
	//是不是连接
	if (XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_CONNECT == st_MQTTProtcol.st_FixedHdr.byMsgType)
	{
		int nListCount = 6;
		MQTTPROTOCOL_HDRPROPERTY** ppSt_HDRProperty;

		Packet_Property(tszSDBuffer, &nSDLen, &ppSt_HDRProperty, nListCount);
		MQTTProtocol_Packet_REPConnect(tszRVBuffer, &nRVLen, 0, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_REASON_SUCCESS, &ppSt_HDRProperty, nListCount);
		MQTTProtocol_Packet_Header(tszSDBuffer, &nSDLen, nRVLen, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_CONNACK, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_FLAG_CONNACK);
		memcpy(tszSDBuffer + nSDLen, tszRVBuffer, nRVLen);
		nSDLen += nRVLen;

		NetCore_TCPXCore_SendEx(xhToken, lpszClientAddr, tszSDBuffer, nSDLen);
	}
	else if (XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_SUBSCRIBE == st_MQTTProtcol.st_FixedHdr.byMsgType)
	{
		MQTTProtocol_Packet_REPComm(tszRVBuffer, &nRVLen, st_MQTTProtcol.wMsgID, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_REASON_SUCCESS);
		MQTTProtocol_Packet_Header(tszSDBuffer, &nSDLen, nRVLen, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_SUBACK, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_FLAG_SUBACK);
		memcpy(tszSDBuffer + nSDLen, tszRVBuffer, nRVLen);
		nSDLen += nRVLen;

		NetCore_TCPXCore_SendEx(xhToken, lpszClientAddr, tszSDBuffer, nSDLen);
	}
	else if (XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_UNSUBSCRIBE == st_MQTTProtcol.st_FixedHdr.byMsgType)
	{
		MQTTProtocol_Packet_REPComm(tszRVBuffer, &nRVLen, st_MQTTProtcol.wMsgID, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_REASON_SUCCESS);
		MQTTProtocol_Packet_Header(tszSDBuffer, &nSDLen, nRVLen, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_UNSUBACK, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_FLAG_UNSUBACK);
		memcpy(tszSDBuffer + nSDLen, tszRVBuffer, nRVLen);
		nSDLen += nRVLen;

		NetCore_TCPXCore_SendEx(xhToken, lpszClientAddr, tszSDBuffer, nSDLen);
	}
	else if (XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_PUBLISH == st_MQTTProtcol.st_FixedHdr.byMsgType)
	{
		if ((XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_FLAG_PUBLISH_QOS1 == st_MQTTProtcol.st_FixedHdr.byMsgFlag) || (XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_FLAG_PUBLISH_QOS2 == st_MQTTProtcol.st_FixedHdr.byMsgFlag))
		{
			//需要回复
			MQTTProtocol_Packet_REPPublish(tszRVBuffer, &nRVLen, st_MQTTProtcol.wMsgID, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_REASON_SUCCESS);
			MQTTProtocol_Packet_Header(tszSDBuffer, &nSDLen, nRVLen, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_PUBACK, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_FLAG_PUBACK);
			memcpy(tszSDBuffer + nSDLen, tszRVBuffer, nRVLen);
			nSDLen += nRVLen;

			NetCore_TCPXCore_SendEx(xhToken, lpszClientAddr, tszSDBuffer, nSDLen);
		}
	}
	else if (XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_PINGREQ == st_MQTTProtcol.st_FixedHdr.byMsgType)
	{
		MQTTProtocol_Packet_Header(tszSDBuffer, &nSDLen, 0, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_PINGREP, 0);
		NetCore_TCPXCore_SendEx(xhToken, lpszClientAddr, tszSDBuffer, nSDLen);
	}
	return true;
}

bool RfcComponents_APPMQTT_Recv(XSOCKET hSocket)
{
	int nMsgLen = 2048;
	XCHAR tszMsgBuffer[2048];
	MQTTPROTOCOL_INFORMATION st_MQTTProtcol;

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_MQTTProtcol, '\0', sizeof(MQTTPROTOCOL_INFORMATION));

	XClient_TCPSelect_RecvMsg(hSocket, tszMsgBuffer, &nMsgLen);

	MQTTProtocol_Parse_Header(tszMsgBuffer, nMsgLen, &st_MQTTProtcol);
	//是不是连接确认
	if (XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_CONNACK == st_MQTTProtcol.st_FixedHdr.byMsgType)
	{
		printf("connect ack\n");
	}
	else if (XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_UNSUBACK == st_MQTTProtcol.st_FixedHdr.byMsgType)
	{
		printf("unsubcribe ok\n");
	}
	else if (XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_SUBACK == st_MQTTProtcol.st_FixedHdr.byMsgType)
	{
		printf("subcribe ok\n");
	}
	else if (XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_PUBACK == st_MQTTProtcol.st_FixedHdr.byMsgType)
	{
		printf("publish ok\n");
	}
	else if (XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_PINGREP == st_MQTTProtcol.st_FixedHdr.byMsgType)
	{
		printf("ping ok\n");
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
	MQTTProtocol_Packet_Header(tszSDBuffer, &nSDLen, nRVLen, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_CONNECT, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_FLAG_CONNECT);
	memcpy(tszSDBuffer + nSDLen, tszRVBuffer, nRVLen);
	nSDLen += nRVLen;

	XClient_TCPSelect_SendMsg(hSocket, tszSDBuffer, nSDLen);
	RfcComponents_APPMQTT_Recv(hSocket);
	//取消订阅
	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));

	MQTTProtocol_Packet_REQUNSubscribe(tszRVBuffer, &nRVLen, 1101, "topic/name");
	MQTTProtocol_Packet_Header(tszSDBuffer, &nSDLen, nRVLen, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_UNSUBSCRIBE, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_FLAG_UNSUBSCRIBE);
	memcpy(tszSDBuffer + nSDLen, tszRVBuffer, nRVLen);
	nSDLen += nRVLen;

	XClient_TCPSelect_SendMsg(hSocket, tszSDBuffer, nSDLen);
	RfcComponents_APPMQTT_Recv(hSocket);
	//订阅
	MQTTPROTOCOL_HDRSUBSCRIBE st_HDROPtion;

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));
	memset(&st_HDROPtion, '\0', sizeof(MQTTPROTOCOL_HDRSUBSCRIBE));
	
	MQTTProtocol_Packet_REQSubscribe(tszRVBuffer, &nRVLen, 1102, "topic/name", &st_HDROPtion);
	MQTTProtocol_Packet_Header(tszSDBuffer, &nSDLen, nRVLen, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_SUBSCRIBE, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_FLAG_SUBSCRIBE);
	memcpy(tszSDBuffer + nSDLen, tszRVBuffer, nRVLen);
	nSDLen += nRVLen;

	XClient_TCPSelect_SendMsg(hSocket, tszSDBuffer, nSDLen);
	RfcComponents_APPMQTT_Recv(hSocket);
	//发布消息
	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));

	MQTTProtocol_Packet_REQPublish(tszRVBuffer, &nRVLen, "topic/name");
	memcpy(tszRVBuffer + nRVLen, "hello", 5);
	nRVLen += 5;

	MQTTProtocol_Packet_Header(tszSDBuffer, &nSDLen, nRVLen, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_PUBLISH, 0);
	memcpy(tszSDBuffer + nSDLen, tszRVBuffer, nRVLen);
	nSDLen += nRVLen;

	XClient_TCPSelect_SendMsg(hSocket, tszSDBuffer, nSDLen);
	//PING
	nRVLen = 0;
	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	MQTTProtocol_Packet_Header(tszSDBuffer, &nSDLen, nRVLen, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_PINGREQ, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_FLAG_PINGREQ);
	XClient_TCPSelect_SendMsg(hSocket, tszSDBuffer, nSDLen);
	RfcComponents_APPMQTT_Recv(hSocket);
	//断开连接
	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));

	MQTTProtocol_Packet_DisConnect(tszRVBuffer, &nRVLen);
	MQTTProtocol_Packet_Header(tszSDBuffer, &nSDLen, nRVLen, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_TYPE_DISCONN, XENGINE_RFCCOMPONENTS_MQTT_PROTOCOL_FLAG_DISCONN);
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
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}