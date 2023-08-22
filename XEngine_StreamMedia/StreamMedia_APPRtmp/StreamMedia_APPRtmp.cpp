#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <winsock2.h>
#include <Windows.h>
#include <tchar.h>
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
#include <XEngine_Include/XEngine_Core/ManagePool_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#include <XEngine_Include/XEngine_StreamMedia/RTMPProtocol_Define.h>
#include <XEngine_Include/XEngine_StreamMedia/RTMPProtocol_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AVCollect_Define.h>
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AudioCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_Core/XEngine_Core.lib")
#pragma comment(lib,"XEngine_Client/XClient_Socket.lib")
#pragma comment(lib,"XEngine_StreamMedia/StreamMedia_RTMPProtocol.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVHelp.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Core/NetCore_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Core/NetCore_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_ManagePool/ManagePool_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_RTMPProtocol/RTMPProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_RTMPProtocol/RTMPProtocol_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVCollect/AVCollect_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_VideoCodec/VideoCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AudioCodec/AudioCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Core.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_Socket.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/StreamMedia_RTMPProtocol.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVHelp.lib")
#endif
#endif

//Linux::g++ -std=c++17 -Wall -g StreamMedia_APPRtmp.cpp -o StreamMedia_APPRtmp.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCodec -lXEngine_BaseLib -lXEngine_Core -lXClient_Socket -lNetHelp_APIHelp -lStreamMedia_RTMPProtocol -lXEngine_AVHelp -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCodec,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g StreamMedia_APPRtmp.cpp -o StreamMedia_APPRtmp.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCodec -lXEngine_BaseLib -lXEngine_Core -lXClient_Socket -lNetHelp_APIHelp -lStreamMedia_RTMPProtocol -lXEngine_AVHelp

bool bServer = false;
XSOCKET hClient = 0;
XHANDLE xhXCore = NULL;
bool bConnect = false;
FILE* pSt_File;

XCHAR tszClientAddr[128];

bool CALLBACK XEngine_TCPXCore_Login(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	strcpy(tszClientAddr, lpszClientAddr);
	RTMPProtocol_Parse_Insert(lpszClientAddr, hSocket);
	printf("XEngine_TCPXCore_Login:%s = %d\n", lpszClientAddr, hSocket);
	return true;
}
void CALLBACK XEngine_TCPXCore_Recv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	//printf("XEngine_TCPXCore_Recv:%s = %d\n", lpszClientAddr, nMsgLen);
	RTMPProtocol_Parse_Send(lpszClientAddr, lpszRecvMsg, nMsgLen);
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

		if (XClient_TCPSelect_RecvMsg(hClient, tszMsgBuffer, &nMSGLen))
		{
			RTMPProtocol_Parse_Send(tszClientAddr, tszMsgBuffer, nMSGLen);
		}
	}
}
void RTMPProtocol_Thread_Process()
{
	while (true)
	{
		if (RTMPProtocol_Parse_WaitEvent(1))
		{
			int nListCount = 0;
			XENGINE_MANAGEPOOL_TASKEVENT** ppSt_ListAddr;
			RTMPProtocol_Parse_GetPool(1, &ppSt_ListAddr, &nListCount);
			for (int i = 0; i < nListCount; i++)
			{
				for (int j = 0; j < ppSt_ListAddr[i]->nPktCount; j++)
				{
					int nMsgLen = 0;
					int nSDLen = 0;
					XCHAR* ptszMsgBuffer = NULL;
					XCHAR tszSDBuffer[4096];
					XENGINE_RTMPHDR st_RTMPHdr;

					memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
					memset(&st_RTMPHdr, '\0', sizeof(XENGINE_RTMPHDR));

					if (RTMPProtocol_Parse_Recv(ppSt_ListAddr[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_RTMPHdr))
					{
						if (XENGINE_STREAMMEDIA_RTMP_MSGTYPE_CONNREQ == st_RTMPHdr.byTypeID)
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
						else if (XENGINE_STREAMMEDIA_RTMP_MSGTYPE_CONNACK == st_RTMPHdr.byTypeID)
						{
							XBYTE byVersion = 0;
							XENGINE_RTMPCONNECT st_RTMPClient;
							XENGINE_RTMPCONNECT st_RTMPServer;

							memset(&st_RTMPClient, '\0', sizeof(XENGINE_RTMPCONNECT));
							memset(&st_RTMPServer, '\0', sizeof(XENGINE_RTMPCONNECT));

							RTMPProtocol_Help_ParseConnect(&byVersion, &st_RTMPServer, &st_RTMPClient, ptszMsgBuffer, nMsgLen);
							if (!bServer)
							{
								bConnect = true;
								XClient_TCPSelect_SendMsg(hClient, (LPCXSTR)&st_RTMPServer, sizeof(XENGINE_RTMPCONNECT));
							}
						}
						else if (XENGINE_STREAMMEDIA_RTMP_MSGTYPE_CONTROL == st_RTMPHdr.byTypeID)
						{
							XENGINE_RTMPPROTOCOLCONTROL st_RTMPControl;
							memset(&st_RTMPControl, '\0', sizeof(XENGINE_RTMPPROTOCOLCONTROL));

							RTMPProtocol_Help_ParseProtocolControl(&st_RTMPControl, st_RTMPHdr.byTypeID, ptszMsgBuffer, nMsgLen);
						}
						else if (XENGINE_STREAMMEDIA_RTMP_MSGTYPE_DATA == st_RTMPHdr.byTypeID)
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
						else if (XENGINE_STREAMMEDIA_RTMP_MSGTYPE_AUDIO == st_RTMPHdr.byTypeID)
						{
							XENGINE_RTMPAUDIO st_RTMPAudio;
							memset(&st_RTMPAudio, '\0', sizeof(XENGINE_RTMPAUDIO));

							memcpy(&st_RTMPAudio, ptszMsgBuffer, sizeof(XENGINE_RTMPAUDIO));

							printf("ENUM_XENGINE_STREAMMEDIA_RTMP_PARSE_PKTTYPE_AUDIO:%d = %d %d %d %d\n", nMsgLen, st_RTMPAudio.byAudioFmt, st_RTMPAudio.byAudioRate, st_RTMPAudio.byAudioSize, st_RTMPAudio.byAudioType);
						}
						else if (XENGINE_STREAMMEDIA_RTMP_MSGTYPE_VIDEO == st_RTMPHdr.byTypeID)
						{
							int nFLen = XENGINE_MEMORY_SIZE_MAX;
							XCHAR* ptszFBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_MAX);
							XENGINE_RTMPVIDEO st_RTMPVideo;
							XENGINE_RTMPVIDEOPARAM st_RTMPVParam;

							memset(&st_RTMPVParam, '\0', sizeof(XENGINE_RTMPVIDEOPARAM));
							memset(&st_RTMPVideo, '\0', sizeof(XENGINE_RTMPVIDEO));

							memcpy(&st_RTMPVideo, ptszMsgBuffer, sizeof(XENGINE_RTMPVIDEO));
							RTMPProtocol_Help_ParseVideo(&st_RTMPVideo, ptszFBuffer, &nFLen, ptszMsgBuffer + sizeof(XENGINE_RTMPVIDEO), nMsgLen - sizeof(XENGINE_RTMPVIDEO), &st_RTMPVParam);
							
							if (st_RTMPVideo.byAVCType == 1)
							{
								fwrite(ptszFBuffer, 1, nFLen, pSt_File);
							}
							else
							{
								fwrite(ptszFBuffer, 1, nFLen, pSt_File);
							}
							free(ptszFBuffer);
							printf("ENUM_XENGINE_STREAMMEDIA_RTMP_PARSE_PKTTYPE_VIDEO:%d = %d %d %d\n", nMsgLen, st_RTMPVideo.byCodecID, st_RTMPVideo.byFrameType, st_RTMPVideo.byAVCType);
						}
						else if (XENGINE_STREAMMEDIA_RTMP_MSGTYPE_COMMAND == st_RTMPHdr.byTypeID)
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

							if (bServer)
							{
								st_RTMPCommand.nProCount = 0;
								st_RTMPCommand.nObCount = 0;
								if (0 == _tcsxnicmp(XENGINE_STREAMMEDIA_RTMP_COMMAND_PUBLISH, st_RTMPCommand.tszCMDName, strlen(XENGINE_STREAMMEDIA_RTMP_COMMAND_PUBLISH)))
								{
									st_RTMPCommand.nProCount = 2;
									BaseLib_OperatorMemory_Malloc((XPPPMEM)&st_RTMPCommand.ppSt_CMDProperty, st_RTMPCommand.nProCount, sizeof(XENGINE_RTMPCMDPROPERTY));

									st_RTMPCommand.nCMDId = 0;
									strcpy(st_RTMPCommand.tszCMDName, XENGINE_STREAMMEDIA_RTMP_COMMAND_ONPUBLISH);

									_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[0]->tszKeyBuffer, "code");
									st_RTMPCommand.ppSt_CMDProperty[0]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_PLTYPE_AFM0_STRING;
									st_RTMPCommand.ppSt_CMDProperty[0]->st_CMDOBJect.nMLen = 23;
									_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[0]->st_CMDOBJect.tszMsgBuffer, "NetStream.Publish.Start");

									_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[1]->tszKeyBuffer, "description");
									st_RTMPCommand.ppSt_CMDProperty[1]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_PLTYPE_AFM0_STRING;
									st_RTMPCommand.ppSt_CMDProperty[1]->st_CMDOBJect.nMLen = 26;
									_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[1]->st_CMDOBJect.tszMsgBuffer, "Started publishing stream.");

									RTMPProtocol_Help_PKTCommand(tszSDBuffer, &nSDLen, 5, &st_RTMPCommand, true, false);
									NetCore_TCPXCore_SendEx(xhXCore, tszClientAddr, tszSDBuffer, nSDLen);
									BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDProperty, st_RTMPCommand.nProCount);
									BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDObject, st_RTMPCommand.nObCount);
									//////////////////////////////////////////////////////////////////////////
									st_RTMPCommand.nProCount = 4;
									st_RTMPCommand.nObCount = 0;
									BaseLib_OperatorMemory_Malloc((XPPPMEM)&st_RTMPCommand.ppSt_CMDProperty, st_RTMPCommand.nProCount, sizeof(XENGINE_RTMPCMDPROPERTY));

									strcpy(st_RTMPCommand.tszCMDName, XENGINE_STREAMMEDIA_RTMP_COMMAND_ONSTATUS);

									_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[0]->tszKeyBuffer, "level");
									st_RTMPCommand.ppSt_CMDProperty[0]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_PLTYPE_AFM0_STRING;
									st_RTMPCommand.ppSt_CMDProperty[0]->st_CMDOBJect.nMLen = 6;
									_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[0]->st_CMDOBJect.tszMsgBuffer, "status");

									_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[1]->tszKeyBuffer, "code");
									st_RTMPCommand.ppSt_CMDProperty[1]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_PLTYPE_AFM0_STRING;
									st_RTMPCommand.ppSt_CMDProperty[1]->st_CMDOBJect.nMLen = 23;
									_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[1]->st_CMDOBJect.tszMsgBuffer, "NetStream.Publish.Start");

									_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[2]->tszKeyBuffer, "description");
									st_RTMPCommand.ppSt_CMDProperty[2]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_PLTYPE_AFM0_STRING;
									st_RTMPCommand.ppSt_CMDProperty[2]->st_CMDOBJect.nMLen = 26;
									_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[2]->st_CMDOBJect.tszMsgBuffer, "Started publishing stream.");

									_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[3]->tszKeyBuffer, "clientid");
									st_RTMPCommand.ppSt_CMDProperty[3]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_PLTYPE_AFM0_STRING;
									st_RTMPCommand.ppSt_CMDProperty[3]->st_CMDOBJect.nMLen = 8;
									_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[3]->st_CMDOBJect.tszMsgBuffer, "ASAICiss");

									RTMPProtocol_Help_PKTCommand(tszSDBuffer, &nSDLen, 5, &st_RTMPCommand, true, false);
									NetCore_TCPXCore_SendEx(xhXCore, tszClientAddr, tszSDBuffer, nSDLen);
									BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDProperty, st_RTMPCommand.nProCount);
									BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDObject, st_RTMPCommand.nObCount);
								}
								else if (0 == _tcsxnicmp(XENGINE_STREAMMEDIA_RTMP_COMMAND_CREATE, st_RTMPCommand.tszCMDName, strlen(XENGINE_STREAMMEDIA_RTMP_COMMAND_CREATE)))
								{
									st_RTMPCommand.nObCount = 1;
									BaseLib_OperatorMemory_Malloc((XPPPMEM)&st_RTMPCommand.ppSt_CMDObject, st_RTMPCommand.nObCount, sizeof(XENGINE_RTMPCMDOBJECT));

									strcpy(st_RTMPCommand.tszCMDName, XENGINE_STREAMMEDIA_RTMP_COMMAND_RESULT);

									double dlValue = 1;
									st_RTMPCommand.ppSt_CMDObject[0]->byType = XENGINE_STREAMMEDIA_RTMP_PLTYPE_AFM0_INT64;
									st_RTMPCommand.ppSt_CMDObject[0]->nMLen = 8;
									memcpy(st_RTMPCommand.ppSt_CMDObject[0]->tszMsgBuffer, &dlValue, 8);
									
									RTMPProtocol_Help_PKTCommand(tszSDBuffer, &nSDLen, st_RTMPHdr.nChunkType, &st_RTMPCommand, true, false);
									NetCore_TCPXCore_SendEx(xhXCore, tszClientAddr, tszSDBuffer, nSDLen);
								}
								else if (0 == _tcsxnicmp(XENGINE_STREAMMEDIA_RTMP_COMMAND_CONNECT, st_RTMPCommand.tszCMDName, strlen(XENGINE_STREAMMEDIA_RTMP_COMMAND_CONNECT)))
								{
									if (bServer)
									{
										RTMPProtocol_Help_PKTProtocolControl(tszSDBuffer, &nSDLen, XENGINE_STREAMMEDIA_RTMP_MSGTYPE_WINDOWSIZE, 2500000);
										NetCore_TCPXCore_SendEx(xhXCore, tszClientAddr, tszSDBuffer, nSDLen);

										RTMPProtocol_Help_PKTProtocolControl(tszSDBuffer, &nSDLen, XENGINE_STREAMMEDIA_RTMP_MSGTYPE_BANDWIDTH, 2500000, 0x02);
										NetCore_TCPXCore_SendEx(xhXCore, tszClientAddr, tszSDBuffer, nSDLen);

										RTMPProtocol_Help_PKTProtocolControl(tszSDBuffer, &nSDLen, XENGINE_STREAMMEDIA_RTMP_MSGTYPE_SETCHUNK, 60000);
										NetCore_TCPXCore_SendEx(xhXCore, tszClientAddr, tszSDBuffer, nSDLen);
									}

									strcpy(st_RTMPCommand.tszCMDName, XENGINE_STREAMMEDIA_RTMP_COMMAND_RESULT);
									RTMPProtocol_Help_PKTCommand(tszSDBuffer, &nSDLen, st_RTMPHdr.nChunkType, &st_RTMPCommand);
									NetCore_TCPXCore_SendEx(xhXCore, tszClientAddr, tszSDBuffer, nSDLen);
								}
								else
								{
									strcpy(st_RTMPCommand.tszCMDName, XENGINE_STREAMMEDIA_RTMP_COMMAND_RESULT);
									RTMPProtocol_Help_PKTCommand(tszSDBuffer, &nSDLen, st_RTMPHdr.nChunkType, &st_RTMPCommand);
									NetCore_TCPXCore_SendEx(xhXCore, tszClientAddr, tszSDBuffer, nSDLen);
								}
							}
						}
						BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
					}
				}
			}
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListAddr, nListCount);
		}
	}
}

bool RTMP_Parse()
{
	xhXCore = NetCore_TCPXCore_StartEx(1935);
	RTMPProtocol_Parse_Init(1);
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

	RTMPProtocol_Parse_Destory();
	NetCore_TCPXCore_DestroyEx(xhXCore);
	return true;
}

bool RTMP_Packet()
{
	LPCXSTR lpszAddr = _X("10.0.3.155");
	
	if (!XClient_TCPSelect_Create(&hClient, lpszAddr, 1935))
	{
		printf("connect\n");
		return false;
	}
	pSt_File = fopen("D:\\1.hevc", "wb");
	_xstprintf(tszClientAddr, _X("%d"), hClient);
	RTMPProtocol_Parse_Init(1);
	RTMPProtocol_Parse_Insert(tszClientAddr, false);

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
	XClient_TCPSelect_SendMsg(hClient, tszSDBuffer, nSDLen);

	while (true)
	{
		if (bConnect)
		{
			break;
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	//发送控制命令设置CHUNK大小
	RTMPProtocol_Help_PKTProtocolControl(tszSDBuffer, &nSDLen, XENGINE_STREAMMEDIA_RTMP_MSGTYPE_SETCHUNK, 65535);
	XClient_TCPSelect_SendMsg(hClient, tszSDBuffer, nSDLen);
	//发送连接命令
	XENGINE_RTMPCOMMAND st_RTMPCommand;
	memset(&st_RTMPCommand, '\0', sizeof(XENGINE_RTMPCOMMAND));

	st_RTMPCommand.nCMDId = 1;
	_tcsxcpy(st_RTMPCommand.tszCMDName, _X("connect"));

	st_RTMPCommand.nProCount = 8;
	BaseLib_OperatorMemory_Malloc((XPPPMEM)&st_RTMPCommand.ppSt_CMDProperty, st_RTMPCommand.nProCount, sizeof(XENGINE_RTMPCMDPROPERTY));
	_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[0]->tszKeyBuffer, "app");
	st_RTMPCommand.ppSt_CMDProperty[0]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_PLTYPE_AFM0_STRING;
	st_RTMPCommand.ppSt_CMDProperty[0]->st_CMDOBJect.nMLen = 4;
	_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[0]->st_CMDOBJect.tszMsgBuffer, "live");

	_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[1]->tszKeyBuffer, "flashVer");
	st_RTMPCommand.ppSt_CMDProperty[1]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_PLTYPE_AFM0_STRING;
	st_RTMPCommand.ppSt_CMDProperty[1]->st_CMDOBJect.nMLen = 13;
	_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[1]->st_CMDOBJect.tszMsgBuffer, "LNX 9,0,124,2");

	_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[2]->tszKeyBuffer, "tcUrl");
	st_RTMPCommand.ppSt_CMDProperty[2]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_PLTYPE_AFM0_STRING;
	st_RTMPCommand.ppSt_CMDProperty[2]->st_CMDOBJect.nMLen = 27;
	_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[2]->st_CMDOBJect.tszMsgBuffer, "rtmp://10.0.3.155:1935/live");

	_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[3]->tszKeyBuffer, "fpad");
	st_RTMPCommand.ppSt_CMDProperty[3]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_PLTYPE_AFM0_BOOL;
	st_RTMPCommand.ppSt_CMDProperty[3]->st_CMDOBJect.nMLen = 1;
	st_RTMPCommand.ppSt_CMDProperty[3]->st_CMDOBJect.tszMsgBuffer[0] = 0x00;

	_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[4]->tszKeyBuffer, "capabilities");
	st_RTMPCommand.ppSt_CMDProperty[4]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_PLTYPE_AFM0_INT64;
	st_RTMPCommand.ppSt_CMDProperty[4]->st_CMDOBJect.nMLen = 8;
	double dlValue = 15;
	memcpy(st_RTMPCommand.ppSt_CMDProperty[4]->st_CMDOBJect.tszMsgBuffer, &dlValue, 8);

	_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[5]->tszKeyBuffer, "audioCodecs");
	st_RTMPCommand.ppSt_CMDProperty[5]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_PLTYPE_AFM0_INT64;
	st_RTMPCommand.ppSt_CMDProperty[5]->st_CMDOBJect.nMLen = 8;
	dlValue = 4071;
	memcpy(st_RTMPCommand.ppSt_CMDProperty[5]->st_CMDOBJect.tszMsgBuffer, &dlValue, 8);

	_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[6]->tszKeyBuffer, "videoCodecs");
	st_RTMPCommand.ppSt_CMDProperty[6]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_PLTYPE_AFM0_INT64;
	st_RTMPCommand.ppSt_CMDProperty[6]->st_CMDOBJect.nMLen = 8;
	dlValue = 252;
	memcpy(st_RTMPCommand.ppSt_CMDProperty[6]->st_CMDOBJect.tszMsgBuffer, &dlValue, 8);

	_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[7]->tszKeyBuffer, "videoFunction");
	st_RTMPCommand.ppSt_CMDProperty[7]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_PLTYPE_AFM0_INT64;
	st_RTMPCommand.ppSt_CMDProperty[7]->st_CMDOBJect.nMLen = 8;
	dlValue = 1;
	memcpy(st_RTMPCommand.ppSt_CMDProperty[7]->st_CMDOBJect.tszMsgBuffer, &dlValue, 8);

	RTMPProtocol_Help_PKTCommand(tszSDBuffer, &nSDLen, 3, &st_RTMPCommand, false, false);
	XClient_TCPSelect_SendMsg(hClient, tszSDBuffer, nSDLen);
	BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDProperty, st_RTMPCommand.nProCount);
	//createstream
	RTMPProtocol_Help_PKTProtocolControl(tszSDBuffer, &nSDLen, XENGINE_STREAMMEDIA_RTMP_MSGTYPE_WINDOWSIZE, 2500000);
	XClient_TCPSelect_SendMsg(hClient, tszSDBuffer, nSDLen);

	memset(&st_RTMPCommand, '\0', sizeof(XENGINE_RTMPCOMMAND));

	st_RTMPCommand.nCMDId = 2;
	_tcsxcpy(st_RTMPCommand.tszCMDName, XENGINE_STREAMMEDIA_RTMP_COMMAND_CREATE);
	RTMPProtocol_Help_PKTCommand(tszSDBuffer, &nSDLen, 3, &st_RTMPCommand, true, false);
	XClient_TCPSelect_SendMsg(hClient, tszSDBuffer, nSDLen);

	memset(&st_RTMPCommand, '\0', sizeof(XENGINE_RTMPCOMMAND));

	st_RTMPCommand.nCMDId = 3;
	_tcsxcpy(st_RTMPCommand.tszCMDName, XENGINE_STREAMMEDIA_RTMP_COMMAND_CHECK);
	RTMPProtocol_Help_PKTCommand(tszSDBuffer, &nSDLen, 3, &st_RTMPCommand, true, false);
	XClient_TCPSelect_SendMsg(hClient, tszSDBuffer, nSDLen);
	//get 
	memset(&st_RTMPCommand, '\0', sizeof(XENGINE_RTMPCOMMAND));

	st_RTMPCommand.nCMDId = 3;
	_tcsxcpy(st_RTMPCommand.tszCMDName, XENGINE_STREAMMEDIA_RTMP_COMMAND_GETSTREAMLEN);

	st_RTMPCommand.nObCount = 1;
	BaseLib_OperatorMemory_Malloc((XPPPMEM)&st_RTMPCommand.ppSt_CMDObject, st_RTMPCommand.nObCount, sizeof(XENGINE_RTMPCMDOBJECT));
	
	st_RTMPCommand.ppSt_CMDObject[0]->nMLen = 10;
	st_RTMPCommand.ppSt_CMDObject[0]->byType = XENGINE_STREAMMEDIA_RTMP_PLTYPE_AFM0_STRING;
	_tcsxcpy(st_RTMPCommand.ppSt_CMDObject[0]->tszMsgBuffer, "h265");
	RTMPProtocol_Help_PKTCommand(tszSDBuffer, &nSDLen, 8, &st_RTMPCommand, true, false);
	XClient_TCPSelect_SendMsg(hClient, tszSDBuffer, nSDLen);
	BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDObject, st_RTMPCommand.nObCount);
	//play
	memset(&st_RTMPCommand, '\0', sizeof(XENGINE_RTMPCOMMAND));

	st_RTMPCommand.nCMDId = 4;
	_tcsxcpy(st_RTMPCommand.tszCMDName, XENGINE_STREAMMEDIA_RTMP_COMMAND_PLAY);

	st_RTMPCommand.nObCount = 1;
	BaseLib_OperatorMemory_Malloc((XPPPMEM)&st_RTMPCommand.ppSt_CMDObject, st_RTMPCommand.nObCount, sizeof(XENGINE_RTMPCMDOBJECT));

	st_RTMPCommand.ppSt_CMDObject[0]->nMLen = 4;
	st_RTMPCommand.ppSt_CMDObject[0]->byType = XENGINE_STREAMMEDIA_RTMP_PLTYPE_AFM0_STRING;
	_tcsxcpy(st_RTMPCommand.ppSt_CMDObject[0]->tszMsgBuffer, "h265");

	RTMPProtocol_Help_PKTCommand(tszSDBuffer, &nSDLen, 8, &st_RTMPCommand, true, false);
	XClient_TCPSelect_SendMsg(hClient, tszSDBuffer, nSDLen);
	BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDObject, st_RTMPCommand.nObCount);

	std::this_thread::sleep_for(std::chrono::seconds(10000));
	XClient_TCPSelect_Close(hClient);
	RTMPProtocol_Parse_Destory();
	return true;
}

int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	if (bServer)
	{
		RTMP_Parse();
	}
	else
	{
		RTMP_Packet();
	}

#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}
