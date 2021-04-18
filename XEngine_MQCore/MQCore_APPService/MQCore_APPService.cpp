#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_Socket.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/MQCore_XMQService.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_MQCore/MQCore_XMQService/XMQService_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_MQCore/MQCore_XMQService/XMQService_Error.h"

//g++ -std=c++17 -Wall -g MQCore_APPService.cpp -o MQCore_APPService.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia -lXEngine_BaseLib -lXClient_Socket -lMQCore_XMQService -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_MQCore,--disable-new-dtags

SOCKET m_Socket;


void MQ_Post(LPCTSTR lpszMsgBuffer, TCHAR* ptszMsgBuffer, int* pInt_Len)
{
	LPCTSTR lpszAddr = _T("XYRYMQ_CODE");

	XENGINE_PROTOCOLHDR st_ProtocolHdr;
	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

	st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_XMQ;
	st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_MQ_REQPOST;
	st_ProtocolHdr.byVersion = 1;
	st_ProtocolHdr.wPacketSerial = 0;          //序列号,如果永久存在包，这个值需要设置，你自己管理，不然你无法获取多个包
	st_ProtocolHdr.byIsReply = TRUE;          //是否获取投递成功还是失败的协议包
	st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

	XENGINE_PROTOCOL_XMQ st_ProtoXMQ;
	memset(&st_ProtoXMQ, '\0', sizeof(XENGINE_PROTOCOL_XMQ));
	//保存时间，单位秒，如果为0，获取一次后被抛弃。-1 永久存在，PacketKey不能为空，如果有多个永久存在的包wPacketSerial 最好设置值,不然你无法获取多个包
	st_ProtoXMQ.nKeepTime = 0;
	strcpy(st_ProtoXMQ.tszMQKey, lpszAddr);
	//如果不需要包的唯一识别码，可以不要，全部置为0即可
	st_ProtocolHdr.unPacketSize = sizeof(XENGINE_PROTOCOL_XMQ) + _tcslen(lpszMsgBuffer);

	*pInt_Len = sizeof(XENGINE_PROTOCOLHDR) + st_ProtocolHdr.unPacketSize;
	memcpy(ptszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
	memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), &st_ProtoXMQ, sizeof(XENGINE_PROTOCOL_XMQ));
	memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR) + sizeof(XENGINE_PROTOCOL_XMQ), lpszMsgBuffer, _tcslen(lpszMsgBuffer));
}
void MQ_Get(TCHAR* ptszMsgBuffer, int* pInt_Len)
{
	LPCTSTR lpszAddr = _T("XYRYMQ_CODE");
	XENGINE_PROTOCOLHDR st_ProtocolHdr;
	XENGINE_PROTOCOL_XMQ st_ProtoXMQ;

	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));
	memset(&st_ProtoXMQ, '\0', sizeof(XENGINE_PROTOCOL_XMQ));

	st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_XMQ;
	st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_MQ_REQGET;
	st_ProtocolHdr.byVersion = 1;
	st_ProtocolHdr.wPacketSerial = 0;
	st_ProtocolHdr.byIsReply = TRUE;                  //必须为真
	st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

	st_ProtocolHdr.unPacketSize = sizeof(XENGINE_PROTOCOL_XMQ);
	strcpy(st_ProtoXMQ.tszMQKey, lpszAddr);

	*pInt_Len = sizeof(XENGINE_PROTOCOLHDR) + st_ProtocolHdr.unPacketSize;
	memcpy(ptszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
	memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), &st_ProtoXMQ, sizeof(XENGINE_PROTOCOL_XMQ));
}
//离开包
void MQ_Leave(TCHAR* ptszMsgBuffer, int* pInt_Len)
{
	XENGINE_PROTOCOLHDR st_ProtocolHdr;
	XENGINE_PROTOCOL_XMQ st_ProtoXMQ;

	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));
	memset(&st_ProtoXMQ, '\0', sizeof(XENGINE_PROTOCOL_XMQ));

	st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_XMQ;
	st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_MQ_REQDEL;
	st_ProtocolHdr.byVersion = 1;
	st_ProtocolHdr.wPacketSerial = 0;
	st_ProtocolHdr.unPacketSize = 1;
	st_ProtocolHdr.byIsReply = FALSE;
	st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

	strcpy(st_ProtoXMQ.tszMQKey, "XYRYMQ_CODE");

	st_ProtocolHdr.unPacketSize = sizeof(XENGINE_PROTOCOL_XMQ);

	*pInt_Len = sizeof(XENGINE_PROTOCOLHDR) + st_ProtocolHdr.unPacketSize;
	memcpy(ptszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
	memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), &st_ProtoXMQ, sizeof(XENGINE_PROTOCOL_XMQ));
}
void MQ_Login(TCHAR* ptszMsgBuffer, int* pInt_Len)
{
	XENGINE_PROTOCOLHDR st_ProtocolHdr;
	XENGINE_PROTOCOL_USERAUTH st_ProtocolAuth;

	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));
	memset(&st_ProtocolAuth, '\0', sizeof(XENGINE_PROTOCOL_USERAUTH));

	st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_AUTH;
	st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQLOGIN;
	st_ProtocolHdr.byVersion = 1;
	st_ProtocolHdr.wPacketSerial = 0;
	st_ProtocolHdr.unPacketSize = 1;
	st_ProtocolHdr.byIsReply = TRUE;
	st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;
	st_ProtocolHdr.unPacketSize = sizeof(XENGINE_PROTOCOL_USERAUTH);

	strcpy(st_ProtocolAuth.tszUserName, "123123aa");
	strcpy(st_ProtocolAuth.tszPassword, "123123");

	*pInt_Len = sizeof(XENGINE_PROTOCOLHDR) + st_ProtocolHdr.unPacketSize;
	memcpy(ptszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
	memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), &st_ProtocolAuth, sizeof(XENGINE_PROTOCOL_USERAUTH));
}
//UDP
int main2(int argc, char** argv)
{
	BOOL bSend = FALSE;
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
	LPCTSTR lpszServiceAddr = _T("192.168.1.6");
	LPCTSTR lpszMsgBuffer = _T("123456789aaa");
	int nLen = 0;

	TCHAR tszClientAddr[64];
	TCHAR tszMsgBuffer[2048];

	memset(tszClientAddr, '\0', sizeof(tszClientAddr));
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	SOCKET hSocket;
	if (!XClient_UDPSelect_Create(&hSocket, lpszServiceAddr, 5656))
	{
		printf("连接失败！\n");
		return -1;
	}
	printf("连接成功！\n");
	if (bSend)
	{
		MQ_Post(lpszMsgBuffer, tszMsgBuffer, &nLen);
		if (!XClient_UDPSelect_SendMsg(hSocket, tszMsgBuffer, nLen))
		{
			printf("发送投递失败！\n");
			return -1;
		}
		Sleep(1000);
		printf("投递数据包成功！\n");
	}
	else
	{
		if (!XClient_UDPSelect_Bind(hSocket, 56561))
		{
			return FALSE;
		}
		MQ_Get(tszMsgBuffer, &nLen);
		if (!XClient_UDPSelect_SendMsg(hSocket, tszMsgBuffer, nLen))
		{
			printf("发送获取失败！\n");
			return -1;
		}

		while (TRUE)
		{
			nLen = 2048;
			memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
			if (XClient_UDPSelect_RecvMsg(hSocket, tszMsgBuffer, &nLen, tszClientAddr))
			{
				XENGINE_PROTOCOLHDR st_ProtocolHdr;
				memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

				memcpy(&st_ProtocolHdr, tszMsgBuffer, sizeof(XENGINE_PROTOCOLHDR));
				printf("接受到数据：%d 类型：%X，长度：%d，内容：%s\n", nLen, st_ProtocolHdr.unOperatorCode, st_ProtocolHdr.unPacketSize, tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR));
				break;
			}
			Sleep(1);
		}
	}

	nLen = 2048;
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	//MQ_Leave(tszMsgBuffer, &nLen);
	//XClient_UDPSelect_SendMsg(hSocket, tszMsgBuffer, nLen);
	return 0;
}
//TCP
int main(int argc, char** argv)
{
	BOOL bSend = FALSE;
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
	LPCTSTR lpszServiceAddr = _T("127.0.0.1");
	LPCTSTR lpszMsgBuffer = _T("123456789aaa");
	int nLen = 0;
	TCHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	if (!XClient_TCPSelect_Create(lpszServiceAddr, 5655, &m_Socket))
	{
		printf("连接失败！\n");
		return -1;
	}
	printf("连接成功！\n");

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	MQ_Login(tszMsgBuffer, &nLen);
	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nLen))
	{
		printf("发送获取失败！\n");
		return -1;
	}
	nLen = 2048;
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	if (!XClient_TCPSelect_RecvMsg(m_Socket, tszMsgBuffer, &nLen, FALSE))
	{
		printf("接受数据失败！\n");
		return -1;
	}
	XENGINE_PROTOCOLHDR st_ProtocolHdr;
	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

	memcpy(&st_ProtocolHdr, tszMsgBuffer, sizeof(XENGINE_PROTOCOLHDR));
	if (st_ProtocolHdr.wReserve == 0)
	{
		printf("登录成功！\n");
	}
	else
	{
		printf("登录失败！\n");
	}

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	MQ_Post(lpszMsgBuffer, tszMsgBuffer, &nLen);
	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nLen))
	{
		printf("发送投递失败！\n");
		return -1;
	}
	nLen = 2048;
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	if (!XClient_TCPSelect_RecvMsg(m_Socket, tszMsgBuffer, &nLen, FALSE))
	{
		printf("接受数据失败！\n");
		return -1;
	}

	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));
	memcpy(&st_ProtocolHdr, tszMsgBuffer, sizeof(XENGINE_PROTOCOLHDR));
	if (st_ProtocolHdr.wReserve == 0)
	{
		printf("投递成功！\n");
	}
	else
	{
		printf("投递失败！\n");
	}
	Sleep(2000);

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	MQ_Get(tszMsgBuffer, &nLen);
	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nLen))
	{
		printf("发送获取失败！\n");
		return -1;
	}

	while (TRUE)
	{
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		if (XClient_TCPSelect_RecvMsg(m_Socket, tszMsgBuffer, &nLen))
		{
			XENGINE_PROTOCOLHDR st_ProtocolHdr;
			memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

			memcpy(&st_ProtocolHdr, tszMsgBuffer, sizeof(XENGINE_PROTOCOLHDR));
			memcpy(&st_ProtocolHdr, tszMsgBuffer, sizeof(XENGINE_PROTOCOLHDR));
			if (0 == st_ProtocolHdr.wReserve)
			{
				printf("接受到数据：类型：%X，长度：%d，内容：%s\n", st_ProtocolHdr.unOperatorCode, st_ProtocolHdr.unPacketSize, tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR));
			}
			else
			{
				printf("获取消息队列数据失败,错误码:%d\n", st_ProtocolHdr.wReserve);
			}
			break;
		}
		Sleep(1);
	}
	XClient_TCPSelect_Close(m_Socket);
	return 0;
}
