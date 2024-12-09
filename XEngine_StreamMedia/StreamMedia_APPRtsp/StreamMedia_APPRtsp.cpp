#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#else
#include <arpa/inet.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if 1 == _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_StreamMedia/RTSPProtocol_Define.h>
#include <XEngine_Include/XEngine_StreamMedia/RTSPProtocol_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_StreamMedia/StreamMedia_RTSPProtocol.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_RTSPProtocol/RTSPProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_RTSPProtocol/RTSPProtocol_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/StreamMedia_RTSPProtocol.lib")
#endif
#endif
//Linux::g++ -std=c++17 -Wall -g StreamMedia_APPRtsp.cpp -o StreamMedia_APPRtsp.exe -lXEngine_BaseLib -lStreamMedia_RTSPProtocol 

void TestUDP_ParseREQProtocol(LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nListCount = 4;
	XCHAR** pptszListStr;

	BaseLib_Memory_Malloc((XPPPMEM)&pptszListStr, nListCount, MAX_PATH);

	strcpy((pptszListStr)[0], _X("CSeq: 5\r\n"));
	strcpy((pptszListStr)[1], _X("User-Agent: LibVLC/3.0.20 (LIVE555 Streaming Media v2016.11.28)\r\n"));
	strcpy((pptszListStr)[2], _X("Transport: RTP/AVP;unicast;client_port=58042-58043\r\n"));
	strcpy((pptszListStr)[3], _X("Session: fM9tTFsntU2y\r\n"));

	RTSPPROTOCOL_REQUEST st_RTSPRequest = {};
	LPCXSTR lpszMethodStr = _X("SETUP");
	LPCXSTR lpszURLStr = _X("rtsp://127.0.0.1:554/live/qyt/trackID=1");
	LPCXSTR lpszVERStr = _X("RTSP/1.0");

	RTSPProtocol_REQParse_Request(&st_RTSPRequest, lpszMethodStr, lpszURLStr, lpszVERStr, &pptszListStr, nListCount);
	BaseLib_Memory_Free((XPPPMEM)&pptszListStr, nListCount);
	return;
}
void TestUDP_ParseREPProtocol(LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	RTSPPROTOCOL_RESPONSE st_RTSPResponse = {};
	RTSPProtocol_REPParse_Request(&st_RTSPResponse, lpszMsgBuffer, nMsgLen);

	for (int i = 0; i < st_RTSPResponse.nRTPCount; i++)
	{
		printf("%s\n", st_RTSPResponse.ppSt_RTPInfo[i]->tszURLStr);
	}
	BaseLib_Memory_Free((XPPPMEM)&st_RTSPResponse.ppSt_RTPInfo, st_RTSPResponse.nRTPCount);
	return;
}
void TestUDP_PacketREPProtocol(XCHAR* ptszMsgBuffer, int* pInt_MSGLen)
{
	RTSPPROTOCOL_RESPONSE st_RTSPResponse = {};

	st_RTSPResponse.nCode = 200;
	st_RTSPResponse.nCSeq = 4;
	st_RTSPResponse.nPLen = 612;
	st_RTSPResponse.nRTPCount = 2;
	st_RTSPResponse.nTimeout = 65;

	BaseLib_Memory_Malloc((XPPPMEM)&st_RTSPResponse.ppSt_RTPInfo, st_RTSPResponse.nRTPCount, sizeof(RTSPPROTOCOL_RTPINFO));

	st_RTSPResponse.ppSt_RTPInfo[0]->nCSeq = 35011;
	st_RTSPResponse.ppSt_RTPInfo[0]->nNTPTime = 12312312345;
	strcpy(st_RTSPResponse.ppSt_RTPInfo[0]->tszURLStr, "rtsp://127.0.0.1:554/live/qyt/trackID=0");

	st_RTSPResponse.ppSt_RTPInfo[1]->nCSeq = 2342;
	st_RTSPResponse.ppSt_RTPInfo[1]->nNTPTime = 35341241;
	strcpy(st_RTSPResponse.ppSt_RTPInfo[1]->tszURLStr, "rtsp://127.0.0.1:554/live/qyt/trackID=1");

	st_RTSPResponse.st_OPTion.bAnnounce = true;
	st_RTSPResponse.st_OPTion.bDescribe = true;
	st_RTSPResponse.st_OPTion.bOptions = true;
	st_RTSPResponse.st_OPTion.bPlay = true;
	st_RTSPResponse.st_OPTion.bSetup = true;
	st_RTSPResponse.st_OPTion.bTeardown = true;

	st_RTSPResponse.st_Range.bNPTTime = true;

	st_RTSPResponse.st_TransportInfo.st_ClientPorts.nRTPPort = 58001;
	st_RTSPResponse.st_TransportInfo.st_ClientPorts.nRTCPPort = 58002;
	st_RTSPResponse.st_TransportInfo.st_ServerPorts.nRTPPort = 58011;
	st_RTSPResponse.st_TransportInfo.st_ServerPorts.nRTCPPort = 58012;
	st_RTSPResponse.st_TransportInfo.st_TransTypes.bUnicast = true;
	st_RTSPResponse.st_TransportInfo.st_TransFlags.bAVP = true;
	st_RTSPResponse.st_TransportInfo.st_TransFlags.bRTP = true;
	st_RTSPResponse.st_TransportInfo.st_TransFlags.bUDP = true;
	strcpy(st_RTSPResponse.st_TransportInfo.tszSSRCStr, "000002");

	strcpy(st_RTSPResponse.tszConBase, "rtsp://127.0.0.1:554/live/qyt/");
	strcpy(st_RTSPResponse.tszDate, "Wed, Nov 15 2023 09:32:49 GMT");
	strcpy(st_RTSPResponse.tszSession, "fM9tTFsntU2y");

	RTSPProtocol_REPPacket_Response(ptszMsgBuffer, pInt_MSGLen, &st_RTSPResponse);
	BaseLib_Memory_Free((XPPPMEM)&st_RTSPResponse.ppSt_RTPInfo, st_RTSPResponse.nRTPCount);
	printf("%s\n", ptszMsgBuffer);
}
void TestUDP_PacketREQProtocol(XCHAR* ptszMsgBuffer, int* pInt_MSGLen)
{
	RTSPPROTOCOL_REQUEST st_RTSPRequest = {};

	st_RTSPRequest.enMethod = ENUM_RTSPPROTOCOL_METHOD_TYPE_DESCRIBE;
	st_RTSPRequest.nCseq = 2;

	st_RTSPRequest.st_Range.bNPTTime = true;

	st_RTSPRequest.st_TransportInfo.st_TransFlags.bAVP = true;
	st_RTSPRequest.st_TransportInfo.st_TransFlags.bRTP = true;
	st_RTSPRequest.st_TransportInfo.st_TransFlags.bUDP = true;
	st_RTSPRequest.st_TransportInfo.st_TransTypes.bUnicast = true;
	st_RTSPRequest.st_TransportInfo.st_ClientPorts.nRTPPort = 30001;
	st_RTSPRequest.st_TransportInfo.st_ClientPorts.nRTCPPort = 30002;

	strcpy(st_RTSPRequest.tszSession, "D31D13D");
	
	RTSPProtocol_REQPacket_Request(ptszMsgBuffer, pInt_MSGLen, &st_RTSPRequest);
	printf("%s\n", ptszMsgBuffer);
}
int main()
{
	int nMsgLen = 0;
	XCHAR tszMsgBuffer[2048] = {};
	TestUDP_PacketREPProtocol(tszMsgBuffer, &nMsgLen);
	TestUDP_ParseREPProtocol(tszMsgBuffer, nMsgLen);

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	TestUDP_PacketREQProtocol(tszMsgBuffer, &nMsgLen);
	TestUDP_ParseREQProtocol(tszMsgBuffer, nMsgLen);
	return 0;
}