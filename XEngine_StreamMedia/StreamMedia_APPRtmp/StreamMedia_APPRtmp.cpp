﻿#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <winsock2.h>
#include <Windows.h>
#include <tchar.h>
#ifdef _XENGINE_USER_DIR_SYSTEM
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_Core/XEngine_Core.lib")
#pragma comment(lib,"XEngine_Client/XClient_Socket.lib")
#pragma comment(lib,"XEngine_StreamMedia/StreamMedia_RTMPProtocol.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVHelp.lib")
#else
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Core.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_Socket.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/StreamMedia_RTMPProtocol.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVHelp.lib")
#endif
#pragma comment(lib,"WS2_32")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
using namespace std;
#ifdef _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/NetCore_Define.h>
#include <XEngine_Include/XEngine_Core/NetCore_Error.h>
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#include <XEngine_Include/XEngine_StreamMedia/RTMPProtocol_Define.h>
#include <XEngine_Include/XEngine_StreamMedia/RTMPProtocol_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AVCollect_Define.h>
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AudioCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Error.h>
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Core/NetCore_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Core/NetCore_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_RTMPProtocol/RTMPProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_RTMPProtocol/RTMPProtocol_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVCollect/AVCollect_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_VideoCodec/VideoCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AudioCodec/AudioCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Error.h"
#endif

//Linux::g++ -std=c++17 -Wall -g StreamMedia_APPRtmp.cpp -o StreamMedia_APPRtmp.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCodec -lXEngine_BaseLib -lXEngine_Core -lNetHelp_APIHelp -lStreamMedia_RTMPProtocol -lXEngine_AVHelp -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCodec,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g StreamMedia_APPRtmp.cpp -o StreamMedia_APPRtmp.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_StreamMedia -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_AVCodec -lXEngine_BaseLib -lStreamMedia_RTMPProtocol -lXEngine_AVHelp

bool bClient = false;
XHANDLE xhXCore = NULL;
XSOCKET hSocket = 0;
XNETHANDLE xhToken = 0;
bool bConnect = false;
XCHAR tszClientAddr[128];

bool CALLBACK XEngine_TCPXCore_Login(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	strcpy(tszClientAddr, lpszClientAddr);
	printf("XEngine_TCPXCore_Login:%s = %d\n", lpszClientAddr, hSocket);
	return true;
}
void CALLBACK XEngine_TCPXCore_Recv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	//printf("XEngine_TCPXCore_Recv:%s = %d\n", lpszClientAddr, nMsgLen);
	RTMPProtocol_Parse_Send(xhToken, lpszRecvMsg, nMsgLen);
}
void CALLBACK XEngine_TCPXCore_Leave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	printf("XEngine_TCPXCore_Leave:%s\n", lpszClientAddr);
}


void RTMPProtocol_Thread_Recv()
{
	while (true)
	{
		int nMSGLen = 4096;
		XCHAR tszMsgBuffer[4096];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		if (XClient_TCPSelect_RecvMsg(hSocket, tszMsgBuffer, &nMSGLen))
		{
			RTMPProtocol_Parse_Send(xhToken, tszMsgBuffer, nMSGLen);
		}
	}
}
void RTMPProtocol_Thread_Process()
{
	while (true)
	{
		int nMsgLen = 0;
		int nSDLen = 0;
		XCHAR* ptszMsgBuffer = NULL;
		XCHAR tszSDBuffer[4096];
		XENGINE_RTMPHDR st_RTMPHdr;

		memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
		memset(&st_RTMPHdr, '\0', sizeof(XENGINE_RTMPHDR));

		if (RTMPProtocol_Parse_Recv(xhToken, &ptszMsgBuffer, &nMsgLen, &st_RTMPHdr))
		{
			if (ENUM_XENGINE_STREAMMEDIA_RTMP_PARSE_PKTTYPE_CONNREQ == st_RTMPHdr.enPKTType)
			{
				XBYTE byVersion = 0;
				XENGINE_RTMPCONNECT st_RTMPClient;
				XENGINE_RTMPCONNECT st_RTMPServer;
				
				memset(&st_RTMPClient, '\0', sizeof(XENGINE_RTMPCONNECT));
				memset(&st_RTMPServer, '\0', sizeof(XENGINE_RTMPCONNECT));

				RTMPProtocol_Help_ParseConnect(&byVersion, NULL, &st_RTMPClient, ptszMsgBuffer, nMsgLen);
				RTMPProtocol_Help_PKTConnect(tszSDBuffer, &nSDLen, &st_RTMPClient, byVersion, &st_RTMPServer);

				NetCore_TCPXCore_SendEx(xhXCore, tszClientAddr, tszSDBuffer, nSDLen);
			}
			else if (ENUM_XENGINE_STREAMMEDIA_RTMP_PARSE_PKTTYPE_CONNACK == st_RTMPHdr.enPKTType)
			{
				XBYTE byVersion = 0;
				XENGINE_RTMPCONNECT st_RTMPClient;
				XENGINE_RTMPCONNECT st_RTMPServer;

				memset(&st_RTMPClient, '\0', sizeof(XENGINE_RTMPCONNECT));
				memset(&st_RTMPServer, '\0', sizeof(XENGINE_RTMPCONNECT));
				
				RTMPProtocol_Help_ParseConnect(&byVersion, &st_RTMPServer, &st_RTMPClient, ptszMsgBuffer, nMsgLen);
				if (hSocket > 0)
				{
					bConnect = true;
					XClient_TCPSelect_SendMsg(hSocket, (LPCXSTR)&st_RTMPServer, sizeof(XENGINE_RTMPCONNECT));
				}
			}
			else if (ENUM_XENGINE_STREAMMEDIA_RTMP_PARSE_PKTTYPE_CONTROL == st_RTMPHdr.enPKTType)
			{
				XENGINE_RTMPCONTROL st_RTMPControl;
				memset(&st_RTMPControl, '\0', sizeof(XENGINE_RTMPCONTROL));

				RTMPProtocol_Help_ParseControl(&st_RTMPControl, st_RTMPHdr.byTypeID, ptszMsgBuffer, nMsgLen);

				if (!bClient)
				{
					RTMPProtocol_Help_PKTControl(tszSDBuffer, &nSDLen, XENGINE_STREAMMEDIA_RTMP_MSGTYPE_CONTROL_WINDOWSIZE, 2500000);
					NetCore_TCPXCore_SendEx(xhXCore, tszClientAddr, tszSDBuffer, nSDLen);

					RTMPProtocol_Help_PKTControl(tszSDBuffer, &nSDLen, XENGINE_STREAMMEDIA_RTMP_MSGTYPE_CONTROL_BANDWIDTH, 2500000, 0x02);
					NetCore_TCPXCore_SendEx(xhXCore, tszClientAddr, tszSDBuffer, nSDLen);

					RTMPProtocol_Help_PKTControl(tszSDBuffer, &nSDLen, XENGINE_STREAMMEDIA_RTMP_MSGTYPE_CONTROL_SETCHUNK, 60000);
					NetCore_TCPXCore_SendEx(xhXCore, tszClientAddr, tszSDBuffer, nSDLen);
				}
			}
			else if (ENUM_XENGINE_STREAMMEDIA_RTMP_PARSE_PKTTYPE_DATA == st_RTMPHdr.enPKTType)
			{
				XENGINE_RTMPDATA st_RTMPData;
				memset(&st_RTMPData, '\0', sizeof(XENGINE_RTMPDATA));

				RTMPProtocol_Help_ParseData(&st_RTMPData, ptszMsgBuffer, nMsgLen);
				printf("RTMPProtocol_Help_ParseData:%s %s \n", st_RTMPData.tszDataName, st_RTMPData.tszDataValue);
				for (int i = 0; i < st_RTMPData.nCount; i++)
				{
					printf("ENUM_XENGINE_STREAMMEDIA_RTMP_PARSE_PKTTYPE_DATA,Name:%s,Value:%s\n", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer);
				}
			}
			else if (ENUM_XENGINE_STREAMMEDIA_RTMP_PARSE_PKTTYPE_AUDIO == st_RTMPHdr.enPKTType)
			{
				XENGINE_RTMPAUDIO st_RTMPAudio;
				memset(&st_RTMPAudio, '\0', sizeof(XENGINE_RTMPAUDIO));

				memcpy(&st_RTMPAudio, ptszMsgBuffer, sizeof(XENGINE_RTMPAUDIO));

				printf("ENUM_XENGINE_STREAMMEDIA_RTMP_PARSE_PKTTYPE_AUDIO:%d = %d %d %d %d\n", nMsgLen, st_RTMPAudio.byAudioFmt, st_RTMPAudio.byAudioRate, st_RTMPAudio.byAudioSize, st_RTMPAudio.byAudioType);
			}
			else if (ENUM_XENGINE_STREAMMEDIA_RTMP_PARSE_PKTTYPE_VIDEO == st_RTMPHdr.enPKTType)
			{
				XENGINE_RTMPVIDEO st_RTMPVideo;
				memset(&st_RTMPVideo, '\0', sizeof(XENGINE_RTMPVIDEO));

				memcpy(&st_RTMPVideo, ptszMsgBuffer, sizeof(XENGINE_RTMPVIDEO));

				printf("ENUM_XENGINE_STREAMMEDIA_RTMP_PARSE_PKTTYPE_VIDEO:%d = %d %d\n", nMsgLen, st_RTMPVideo.byCodecID, st_RTMPVideo.byFrameType);
			}
			else if (ENUM_XENGINE_STREAMMEDIA_RTMP_PARSE_PKTTYPE_COMMAND == st_RTMPHdr.enPKTType)
			{
				XENGINE_RTMPCOMMAND st_RTMPCommand;
				memset(&st_RTMPCommand, '\0', sizeof(XENGINE_RTMPCOMMAND));

				RTMPProtocol_Help_ParseCommand(&st_RTMPCommand, ptszMsgBuffer, nMsgLen);
				for (int i = 0; i < st_RTMPCommand.nProCount; i++)
				{
					printf("ppSt_CMDProperty,Name:%s,Value:%s %s\n", st_RTMPCommand.tszCMDName, st_RTMPCommand.ppSt_CMDProperty[i]->tszKeyBuffer, st_RTMPCommand.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer);
				}
				for (int i = 0; i < st_RTMPCommand.nObCount; i++)
				{
					printf("ppSt_CMDObject,Name:%s,Value:%d %s\n", st_RTMPCommand.tszCMDName, st_RTMPCommand.ppSt_CMDObject[i]->byType, st_RTMPCommand.ppSt_CMDObject[i]->tszMsgBuffer);
				}

				BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDProperty, st_RTMPCommand.nProCount);
				BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDObject, st_RTMPCommand.nObCount);

				if (!bClient)
				{
					st_RTMPCommand.nProCount = 0;
					st_RTMPCommand.nObCount = 0;
					if (0 == _tcsxnicmp(XENGINE_STREAMMEDIA_RTMP_MSGTYPE_COMMAND_PUBLISH, st_RTMPCommand.tszCMDName, strlen(XENGINE_STREAMMEDIA_RTMP_MSGTYPE_COMMAND_PUBLISH)))
					{
						st_RTMPCommand.nProCount = 4;
						BaseLib_OperatorMemory_Malloc((XPPPMEM)&st_RTMPCommand.ppSt_CMDProperty, st_RTMPCommand.nProCount, sizeof(XENGINE_RTMPCMDPROPERTY));

						strcpy(st_RTMPCommand.tszCMDName, XENGINE_STREAMMEDIA_RTMP_MSGTYPE_COMMAND_ONSTATUS);

						_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[0]->tszKeyBuffer, "level");
						st_RTMPCommand.ppSt_CMDProperty[0]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_MSGTYPE_AFM0_STRING;
						st_RTMPCommand.ppSt_CMDProperty[0]->st_CMDOBJect.nMLen = 6;
						_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[0]->st_CMDOBJect.tszMsgBuffer, "status");

						_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[1]->tszKeyBuffer, "code");
						st_RTMPCommand.ppSt_CMDProperty[1]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_MSGTYPE_AFM0_STRING;
						st_RTMPCommand.ppSt_CMDProperty[1]->st_CMDOBJect.nMLen = 23;
						_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[1]->st_CMDOBJect.tszMsgBuffer, "NetStream.Publish.Start");

						_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[2]->tszKeyBuffer, "description");
						st_RTMPCommand.ppSt_CMDProperty[2]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_MSGTYPE_AFM0_STRING;
						st_RTMPCommand.ppSt_CMDProperty[2]->st_CMDOBJect.nMLen = 26;
						_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[2]->st_CMDOBJect.tszMsgBuffer, "Started publishing stream.");

						_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[3]->tszKeyBuffer, "clientid");
						st_RTMPCommand.ppSt_CMDProperty[3]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_MSGTYPE_AFM0_STRING;
						st_RTMPCommand.ppSt_CMDProperty[3]->st_CMDOBJect.nMLen = 8;
						_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[3]->st_CMDOBJect.tszMsgBuffer, "ASAICiss");

						RTMPProtocol_Help_PKTCommand(tszSDBuffer, &nSDLen, 5, &st_RTMPCommand);
					}
					else
					{
						strcpy(st_RTMPCommand.tszCMDName, XENGINE_STREAMMEDIA_RTMP_MSGTYPE_COMMAND_RESULT);
						RTMPProtocol_Help_PKTCommand(tszSDBuffer, &nSDLen, st_RTMPHdr.nChunkType, &st_RTMPCommand);
					}
					NetCore_TCPXCore_SendEx(xhXCore, tszClientAddr, tszSDBuffer, nSDLen);
				}
			}
			BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(0));
	}
}

bool RTMP_Parse()
{
	xhXCore = NetCore_TCPXCore_StartEx(1935);
	RTMPProtocol_Parse_Create(&xhToken);
	RTMPProtocol_Parse_SetChunkSize(xhToken, 4096);
	if (NULL != xhXCore)
	{
		printf(_X("NetCore_TCPXCore_StartEx Start Is Ok!\n"));
	}
	else
	{
		printf(_X("NetCore_TCPXCore_StartEx Start Is Failed!\n"));
	}
	memset(tszClientAddr, '\0', sizeof(tszClientAddr));

	NetCore_TCPXCore_RegisterCallBackEx(xhXCore, XEngine_TCPXCore_Login, XEngine_TCPXCore_Recv, XEngine_TCPXCore_Leave);
	std::thread st_Thread(RTMPProtocol_Thread_Process);
	st_Thread.detach();

	std::this_thread::sleep_for(std::chrono::seconds(10000));

	RTMPProtocol_Parse_Destory(xhToken);
	NetCore_TCPXCore_DestroyEx(xhXCore);
	return true;
}

bool RTMP_Packet()
{
	LPCXSTR lpszAddr = _X("42.194.178.57");
	
	if (!XClient_TCPSelect_Create(&hSocket, lpszAddr, 1935))
	{
		printf("connect\n");
		return false;
	}
	RTMPProtocol_Parse_Create(&xhToken, false);
	RTMPProtocol_Parse_SetChunkSize(xhToken, 4096);

	std::thread st_ThreadRecv(RTMPProtocol_Thread_Recv);
	std::thread st_ThreadProcess(RTMPProtocol_Thread_Process);

	st_ThreadRecv.detach();
	st_ThreadProcess.detach();

	int nSDLen = 0;
	XCHAR tszSDBuffer[4096];
	XENGINE_RTMPCONNECT st_RTMPConnect;

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(&st_RTMPConnect, '\0', sizeof(XENGINE_RTMPCONNECT));
	//连接
	RTMPProtocol_Help_PKTConnect(tszSDBuffer, &nSDLen, &st_RTMPConnect, 3);
	XClient_TCPSelect_SendMsg(hSocket, tszSDBuffer, nSDLen);

	while (true)
	{
		if (bConnect)
		{
			break;
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	//发送控制命令设置CHUNK大小
	RTMPProtocol_Help_PKTControl(tszSDBuffer, &nSDLen, XENGINE_STREAMMEDIA_RTMP_MSGTYPE_CONTROL_SETCHUNK, XENGINE_MEMORY_SIZE_MAX);
	XClient_TCPSelect_SendMsg(hSocket, tszSDBuffer, nSDLen);
	//发送连接命令
	XENGINE_RTMPCOMMAND st_RTMPCommand;
	memset(&st_RTMPCommand, '\0', sizeof(XENGINE_RTMPCOMMAND));

	st_RTMPCommand.nCMDId = 1;
	_tcsxcpy(st_RTMPCommand.tszCMDName, _X("connect"));

	st_RTMPCommand.nProCount = 6;
	BaseLib_OperatorMemory_Malloc((XPPPMEM)&st_RTMPCommand.ppSt_CMDProperty, st_RTMPCommand.nProCount, sizeof(XENGINE_RTMPCMDPROPERTY));
	_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[0]->tszKeyBuffer, "app");
	st_RTMPCommand.ppSt_CMDProperty[0]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_MSGTYPE_AFM0_STRING;
	st_RTMPCommand.ppSt_CMDProperty[0]->st_CMDOBJect.nMLen = 4;
	_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[0]->st_CMDOBJect.tszMsgBuffer, "live");

	_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[1]->tszKeyBuffer, "type");
	st_RTMPCommand.ppSt_CMDProperty[1]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_MSGTYPE_AFM0_STRING;
	st_RTMPCommand.ppSt_CMDProperty[1]->st_CMDOBJect.nMLen = 10;
	_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[1]->st_CMDOBJect.tszMsgBuffer, "nonprivate");

	_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[2]->tszKeyBuffer, "supportsGoAway");
	st_RTMPCommand.ppSt_CMDProperty[2]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_MSGTYPE_AFM0_BOOL;
	st_RTMPCommand.ppSt_CMDProperty[2]->st_CMDOBJect.nMLen = 1;
	st_RTMPCommand.ppSt_CMDProperty[2]->st_CMDOBJect.tszMsgBuffer[0] = 0x01;

	_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[3]->tszKeyBuffer, "flashVer");
	st_RTMPCommand.ppSt_CMDProperty[3]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_MSGTYPE_AFM0_STRING;
	st_RTMPCommand.ppSt_CMDProperty[3]->st_CMDOBJect.nMLen = 31;
	_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[3]->st_CMDOBJect.tszMsgBuffer, "FMLE/3.0 (compatible; FMSc/1.0)");

	_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[4]->tszKeyBuffer, "swfUrl");
	st_RTMPCommand.ppSt_CMDProperty[4]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_MSGTYPE_AFM0_STRING;
	st_RTMPCommand.ppSt_CMDProperty[4]->st_CMDOBJect.nMLen = 24;
	_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[4]->st_CMDOBJect.tszMsgBuffer, "rtmp://app.xyry.org/live");

	_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[5]->tszKeyBuffer, "tcUrl");
	st_RTMPCommand.ppSt_CMDProperty[5]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_MSGTYPE_AFM0_STRING;
	st_RTMPCommand.ppSt_CMDProperty[5]->st_CMDOBJect.nMLen = 24;
	_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[5]->st_CMDOBJect.tszMsgBuffer, "rtmp://app.xyry.org/live");

	RTMPProtocol_Help_PKTCommand(tszSDBuffer, &nSDLen, 3, &st_RTMPCommand, false, false);
	XClient_TCPSelect_SendMsg(hSocket, tszSDBuffer, nSDLen);
	BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDProperty, st_RTMPCommand.nProCount);
	//release发布流
	memset(&st_RTMPCommand, '\0', sizeof(XENGINE_RTMPCOMMAND));

	st_RTMPCommand.nCMDId = 2;
	_tcsxcpy(st_RTMPCommand.tszCMDName, XENGINE_STREAMMEDIA_RTMP_MSGTYPE_COMMAND_RELEASE);

	st_RTMPCommand.nObCount = 1;
	BaseLib_OperatorMemory_Malloc((XPPPMEM)&st_RTMPCommand.ppSt_CMDObject, st_RTMPCommand.nObCount, sizeof(XENGINE_RTMPCMDOBJECT));
	
	st_RTMPCommand.ppSt_CMDObject[0]->nMLen = 3;
	st_RTMPCommand.ppSt_CMDObject[0]->byType = XENGINE_STREAMMEDIA_RTMP_MSGTYPE_AFM0_STRING;
	_tcsxcpy(st_RTMPCommand.ppSt_CMDObject[0]->tszMsgBuffer, "qyt");
	RTMPProtocol_Help_PKTCommand(tszSDBuffer, &nSDLen, 3, &st_RTMPCommand, true, false);
	XClient_TCPSelect_SendMsg(hSocket, tszSDBuffer, nSDLen);
	BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDObject, st_RTMPCommand.nObCount);
	//fcpublish
	memset(&st_RTMPCommand, '\0', sizeof(XENGINE_RTMPCOMMAND));

	st_RTMPCommand.nCMDId = 3;
	_tcsxcpy(st_RTMPCommand.tszCMDName, XENGINE_STREAMMEDIA_RTMP_MSGTYPE_COMMAND_FCPUBLISH);

	st_RTMPCommand.nObCount = 1;
	BaseLib_OperatorMemory_Malloc((XPPPMEM)&st_RTMPCommand.ppSt_CMDObject, st_RTMPCommand.nObCount, sizeof(XENGINE_RTMPCMDOBJECT));

	st_RTMPCommand.ppSt_CMDObject[0]->nMLen = 3;
	st_RTMPCommand.ppSt_CMDObject[0]->byType = XENGINE_STREAMMEDIA_RTMP_MSGTYPE_AFM0_STRING;
	_tcsxcpy(st_RTMPCommand.ppSt_CMDObject[0]->tszMsgBuffer, "qyt");
	RTMPProtocol_Help_PKTCommand(tszSDBuffer, &nSDLen, 3, &st_RTMPCommand, true, false);
	XClient_TCPSelect_SendMsg(hSocket, tszSDBuffer, nSDLen);
	BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDObject, st_RTMPCommand.nObCount);
	//create 流
	memset(&st_RTMPCommand, '\0', sizeof(XENGINE_RTMPCOMMAND));

	st_RTMPCommand.nCMDId = 4;
	_tcsxcpy(st_RTMPCommand.tszCMDName, XENGINE_STREAMMEDIA_RTMP_MSGTYPE_COMMAND_CREATE);
	RTMPProtocol_Help_PKTCommand(tszSDBuffer, &nSDLen, 3, &st_RTMPCommand, true, false);
	XClient_TCPSelect_SendMsg(hSocket, tszSDBuffer, nSDLen);
	//check 流
	memset(&st_RTMPCommand, '\0', sizeof(XENGINE_RTMPCOMMAND));

	st_RTMPCommand.nCMDId = 5;
	_tcsxcpy(st_RTMPCommand.tszCMDName, XENGINE_STREAMMEDIA_RTMP_MSGTYPE_COMMAND_CHECK);
	RTMPProtocol_Help_PKTCommand(tszSDBuffer, &nSDLen, 3, &st_RTMPCommand, true, false);
	XClient_TCPSelect_SendMsg(hSocket, tszSDBuffer, nSDLen);
	//publish
	memset(&st_RTMPCommand, '\0', sizeof(XENGINE_RTMPCOMMAND));

	st_RTMPCommand.nCMDId = 3;
	_tcsxcpy(st_RTMPCommand.tszCMDName, XENGINE_STREAMMEDIA_RTMP_MSGTYPE_COMMAND_PUBLISH);

	st_RTMPCommand.nObCount = 2;
	BaseLib_OperatorMemory_Malloc((XPPPMEM)&st_RTMPCommand.ppSt_CMDObject, st_RTMPCommand.nObCount, sizeof(XENGINE_RTMPCMDOBJECT));

	st_RTMPCommand.ppSt_CMDObject[0]->nMLen = 3;
	st_RTMPCommand.ppSt_CMDObject[0]->byType = XENGINE_STREAMMEDIA_RTMP_MSGTYPE_AFM0_STRING;
	_tcsxcpy(st_RTMPCommand.ppSt_CMDObject[0]->tszMsgBuffer, "qyt");

	st_RTMPCommand.ppSt_CMDObject[1]->nMLen = 4;
	st_RTMPCommand.ppSt_CMDObject[1]->byType = XENGINE_STREAMMEDIA_RTMP_MSGTYPE_AFM0_STRING;
	_tcsxcpy(st_RTMPCommand.ppSt_CMDObject[1]->tszMsgBuffer, "live");
	RTMPProtocol_Help_PKTCommand(tszSDBuffer, &nSDLen, 3, &st_RTMPCommand, true, false);
	XClient_TCPSelect_SendMsg(hSocket, tszSDBuffer, nSDLen);
	BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDObject, st_RTMPCommand.nObCount);
	//写媒体数据
	int nListCount = 5;
	double nValue64 = 0;
	XENGINE_RTMPCMDPROPERTY** ppSt_PropertyList;
	BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_PropertyList, nListCount, sizeof(XENGINE_RTMPCMDOBJECT));

	_tcsxcpy(ppSt_PropertyList[0]->tszKeyBuffer, "duration");
	ppSt_PropertyList[0]->st_CMDOBJect.byType = 0x00;
	ppSt_PropertyList[0]->st_CMDOBJect.nMLen = 8;

	_tcsxcpy(ppSt_PropertyList[1]->tszKeyBuffer, "fileSize");
	ppSt_PropertyList[1]->st_CMDOBJect.byType = 0x00;
	ppSt_PropertyList[1]->st_CMDOBJect.nMLen = 8;

	nValue64 = 1920;
	_tcsxcpy(ppSt_PropertyList[2]->tszKeyBuffer, "width");
	ppSt_PropertyList[2]->st_CMDOBJect.byType = 0x00;
	ppSt_PropertyList[2]->st_CMDOBJect.nMLen = 8;
	memcpy(ppSt_PropertyList[2]->st_CMDOBJect.tszMsgBuffer, &nValue64, sizeof(nValue64));

	nValue64 = 1080;
	_tcsxcpy(ppSt_PropertyList[3]->tszKeyBuffer, "height");
	ppSt_PropertyList[3]->st_CMDOBJect.byType = 0x00;
	ppSt_PropertyList[3]->st_CMDOBJect.nMLen = 8;
	memcpy(ppSt_PropertyList[3]->st_CMDOBJect.tszMsgBuffer, &nValue64, sizeof(nValue64));

	nValue64 = 7;
	_tcsxcpy(ppSt_PropertyList[4]->tszKeyBuffer, "videocodecid");
	ppSt_PropertyList[4]->st_CMDOBJect.byType = 0x00;
	ppSt_PropertyList[4]->st_CMDOBJect.nMLen = 8;
	memcpy(ppSt_PropertyList[4]->st_CMDOBJect.tszMsgBuffer, &nValue64, sizeof(nValue64));

	nValue64 = 10000;
	_tcsxcpy(ppSt_PropertyList[5]->tszKeyBuffer, "videodatarate");
	ppSt_PropertyList[5]->st_CMDOBJect.byType = 0x00;
	ppSt_PropertyList[5]->st_CMDOBJect.nMLen = 8;
	memcpy(ppSt_PropertyList[5]->st_CMDOBJect.tszMsgBuffer, &nValue64, sizeof(nValue64));

	nValue64 = 60;
	_tcsxcpy(ppSt_PropertyList[6]->tszKeyBuffer, "framerate");
	ppSt_PropertyList[6]->st_CMDOBJect.byType = 0x00;
	ppSt_PropertyList[6]->st_CMDOBJect.nMLen = 8;
	memcpy(ppSt_PropertyList[6]->st_CMDOBJect.tszMsgBuffer, &nValue64, sizeof(nValue64));

	RTMPProtocol_Help_PKTData(tszSDBuffer, &nSDLen, 0x04, &ppSt_PropertyList, nListCount);
	XClient_TCPSelect_SendMsg(hSocket, tszSDBuffer, nSDLen);
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_PropertyList, nListCount);

	std::this_thread::sleep_for(std::chrono::seconds(10000));
	XClient_TCPSelect_Close(hSocket);
	RTMPProtocol_Parse_Destory(xhToken);
	return true;
}

int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	RTMP_Parse();
	//RTMP_Packet();

#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}