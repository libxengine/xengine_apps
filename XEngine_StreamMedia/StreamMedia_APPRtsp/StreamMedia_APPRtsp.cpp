#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/StreamMedia_RTSPProtocol.lib")
#else
#include <arpa/inet.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_RTSPProtocol/RTSPProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_RTSPProtocol/RTSPProtocol_Error.h"

//Linux::g++ -std=c++17 -Wall -g StreamMedia_APPRtsp.cpp -o StreamMedia_APPRtsp.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia -lXEngine_BaseLib -lStreamMedia_RTSPProtocol -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g StreamMedia_APPRtsp.cpp -o StreamMedia_APPRtsp.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_StreamMedia -lXEngine_BaseLib -lStreamMedia_RTSPProtocol

void TestUDP_RtspProtocol()
{
	int nMsgLen = 0;
	LPCTSTR lpszRTSPUrl = _T("rtsp://127.0.0.1/live/1");
	LPCTSTR lpszSession = _T("3B2241FA");
	TCHAR tszMsgBuffer[1024];

	RTSPPROTOCOL_REQUEST st_RtspRequest;
	RTSPPROTOCOL_RESPONSE st_RtspResponse;
	RTSPPROTOCOL_SETUP st_RtspSetup;

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_RtspRequest, '\0', sizeof(RTSPPROTOCOL_REQUEST));
	memset(&st_RtspResponse, '\0', sizeof(RTSPPROTOCOL_RESPONSE));
	memset(&st_RtspSetup, '\0', sizeof(RTSPPROTOCOL_SETUP));

	RTSPProtocol_ClientPacket_Setup(tszMsgBuffer, &nMsgLen, lpszRTSPUrl, IPPROTO_UDP, 1, 63505, 63506);
	RTSPProtocol_CoreParse_Parse(&st_RtspRequest, tszMsgBuffer, nMsgLen);
	RTSPProtocol_CoreHelp_Transport(st_RtspRequest.st_ExtInfo.tszTransport, &st_RtspSetup.nIPProtol, &st_RtspSetup.nClientRTPPort, &st_RtspSetup.nClientRTCPPort);

	st_RtspSetup.nServerRTPPort = 10001;
	st_RtspSetup.nServerRTCPPort = 10002;
	strcpy(st_RtspSetup.tszDestAddr, _T("192.168.1.108"));
	strcpy(st_RtspSetup.tszSourceAddr, _T("192.168.1.115"));

	RTSPProtocol_CorePacket_Setup(tszMsgBuffer, &nMsgLen, lpszSession, &st_RtspSetup, st_RtspRequest.nCseq);
	RTSPProtocol_ClientParse_Parse(&st_RtspResponse, tszMsgBuffer, nMsgLen);
	return;
}
void TestTCP_RtspProtocol()
{
	int nMsgLen = 0;
	LPCTSTR lpszRTSPUrl = _T("rtsp://127.0.0.1/live/1");
	LPCTSTR lpszSession = _T("3B2241FA");
	TCHAR tszMsgBuffer[1024];

	RTSPPROTOCOL_REQUEST st_RtspRequest;
	RTSPPROTOCOL_RESPONSE st_RtspResponse;
	RTSPPROTOCOL_SETUP st_RtspSetup;
	RTSPPROTOCOL_AUTHINFO st_AuthInfo;

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_RtspRequest, '\0', sizeof(RTSPPROTOCOL_REQUEST));
	memset(&st_RtspResponse, '\0', sizeof(RTSPPROTOCOL_RESPONSE));
	memset(&st_RtspSetup, '\0', sizeof(RTSPPROTOCOL_SETUP));
	memset(&st_AuthInfo, '\0', sizeof(RTSPPROTOCOL_AUTHINFO));

	strcpy(st_AuthInfo.tszAuthUser, "123123aa");
	strcpy(st_AuthInfo.tszAuthPass, "123123");

	RTSPProtocol_ClientPacket_Setup(tszMsgBuffer, &nMsgLen, lpszRTSPUrl, IPPROTO_TCP, 1, 1, 2, 3, &st_AuthInfo);
	RTSPProtocol_CoreParse_Parse(&st_RtspRequest, tszMsgBuffer, nMsgLen);
	RTSPProtocol_CoreHelp_Transport(st_RtspRequest.st_ExtInfo.tszTransport, &st_RtspSetup.nIPProtol, &st_RtspSetup.nClientRTPPort, &st_RtspSetup.nClientRTCPPort);

	RTSPProtocol_CorePacket_Setup(tszMsgBuffer, &nMsgLen, lpszSession, &st_RtspSetup, st_RtspRequest.nCseq);
	RTSPProtocol_ClientParse_Parse(&st_RtspResponse, tszMsgBuffer, nMsgLen);
	return;
}

int main()
{
	TestUDP_RtspProtocol();
	TestTCP_RtspProtocol();
	return 0;
}