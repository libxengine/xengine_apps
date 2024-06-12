#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>
#if 1 == _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_StreamMedia/SDPProtocol_Define.h>
#include <XEngine_Include/XEngine_StreamMedia/SDPProtocol_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_StreamMedia/StreamMedia_SDPProtocol.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_SDPProtocol/SDPProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_SDPProtocol/SDPProtocol_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/StreamMedia_SDPProtocol.lib")
#endif
#endif
//Linux::g++ -std=c++17 -Wall -g StreamMedia_APPSDPProtocol.cpp -o StreamMedia_APPSDPProtocol.exe -lXEngine_BaseLib -lStreamMedia_SDPProtocol

void CreateSDP(XCHAR* ptszMsgBuffer, int* pInt_Len)
{
	XNETHANDLE xhToken = 0;
	SDPProtocol_Packet_Create(&xhToken);

	SDPProtocol_Packet_Owner(xhToken, _X("XEngine"), 123456789, _X("192.168.1.101"));
	SDPProtocol_Packet_Session(xhToken, _X("h264"));
	SDPProtocol_Packet_KeepTime(xhToken);
	SDPProtocol_Packet_Control(xhToken, -1);
	SDPProtocol_Packet_Bundle(xhToken);
	//配置视频属性
	XCHAR** pptszAVList;
	BaseLib_OperatorMemory_Malloc((XPPPMEM)&pptszAVList, 5, 64);
	strcpy(pptszAVList[0], "96");
	strcpy(pptszAVList[1], "97");
	strcpy(pptszAVList[2], "98");
	strcpy(pptszAVList[3], "99");
	strcpy(pptszAVList[4], "100");

	SDPProtocol_Packet_AddMedia(xhToken, _X("video"), "RTP/AVP", &pptszAVList, 5, 0);
	STREAMMEDIA_SDPPROTOCOL_MEDIAINFO st_SDPMediaVideo;
	memset(&st_SDPMediaVideo, '\0', sizeof(STREAMMEDIA_SDPPROTOCOL_MEDIAINFO));

	st_SDPMediaVideo.nTrackID = 1;
	st_SDPMediaVideo.st_RTPMap.nSampleRate = 90000;
	strcpy(st_SDPMediaVideo.st_RTPMap.tszCodecName, _X("H264"));

	st_SDPMediaVideo.st_FmtpVideo.nFrameRate = 25;
	st_SDPMediaVideo.st_FmtpVideo.nPacketMode = 1;
	st_SDPMediaVideo.st_FmtpVideo.tszLeaveId[0] = 0x4D;
	st_SDPMediaVideo.st_FmtpVideo.tszLeaveId[1] = 0x40;
	st_SDPMediaVideo.st_FmtpVideo.tszLeaveId[2] = 0x33;
	st_SDPMediaVideo.st_FmtpVideo.nFrameXSize = 1080;
	st_SDPMediaVideo.st_FmtpVideo.nFrameYSize = 720;

	strcpy(st_SDPMediaVideo.st_FmtpVideo.tszPPSBase, _X("aO48gA"));
	strcpy(st_SDPMediaVideo.st_FmtpVideo.tszSPSBase, _X("Z01AM5p0FCNCAAEEugA9CQEeMGVA"));
	SDPProtocol_Packet_VideoFmt(xhToken, 96, &st_SDPMediaVideo);
	SDPProtocol_Packet_Control(xhToken, 0);
	//配置音频属性
	SDPProtocol_Packet_AddMedia(xhToken, _X("audio"), "RTP/AVP", &pptszAVList, 1, 1);
	SDPProtocol_Packet_CName(xhToken, 111111, _X("79a9722580589zr5"), _X("video-666q08to"));

	STREAMMEDIA_SDPPROTOCOL_MEDIAINFO st_SDPMediaAudio;
	memset(&st_SDPMediaAudio, '\0', sizeof(STREAMMEDIA_SDPPROTOCOL_MEDIAINFO));
	st_SDPMediaAudio.nTrackID = 2;
	st_SDPMediaAudio.st_RTPMap.nChannel = 2;
	st_SDPMediaAudio.st_RTPMap.nSampleRate = 80000;
	strcpy(st_SDPMediaAudio.st_RTPMap.tszCodecName, _X("mpeg4-generic"));

	st_SDPMediaAudio.st_FmtpAudio.nConfig = 1400;
	st_SDPMediaAudio.st_FmtpAudio.nDeltaLen = 3;
	st_SDPMediaAudio.st_FmtpAudio.nIndexLen = 3;
	st_SDPMediaAudio.st_FmtpAudio.nProfileID = 1;
	st_SDPMediaAudio.st_FmtpAudio.nSizeLen = 13;
	strcpy(st_SDPMediaAudio.st_FmtpAudio.tszMode, "AAC-hbr");
	SDPProtocol_Packet_AudioFmt(xhToken, 98, &st_SDPMediaAudio);
	SDPProtocol_Packet_CName(xhToken, 222222222);
	SDPProtocol_Packet_Control(xhToken, 1);
	
	STREAMMEDIA_SDPPROTOCOL_RTCP st_RTCPAttr = {};

	st_RTCPAttr.bCcmFir = true;
	st_RTCPAttr.bGoogRemb = true;
	st_RTCPAttr.bNAck = true;
	st_RTCPAttr.bNAckPli = true;
	st_RTCPAttr.bTransportCC = true;
    sprintf(st_RTCPAttr.st_RTPMap.tszNameStr, "H264");
	sprintf(st_RTCPAttr.st_RTPMap.tszSampleStr, "90000");

	st_RTCPAttr.st_FMtp.nLevelCodec = 1;
	st_RTCPAttr.st_FMtp.nPacketMode = 1;
	memcpy(st_RTCPAttr.st_FMtp.tszProLevel, "42e01f", 6);
	SDPProtocol_Packet_AVAttr(xhToken, 106, &st_RTCPAttr);
	//附加信息
	SDPProtocol_Packet_OptionalMediaName(xhToken, "medianame");
	SDPProtocol_Packet_OptionalContact(xhToken, "486179@qq.com", "13699444444");
	SDPProtocol_Packet_OptionalBandwidth(xhToken, 10000);
	SDPProtocol_Packet_OptionalRange(xhToken);
	SDPProtocol_Packet_OptionalAddAttr(xhToken, "this is a exp");

	SDPProtocol_Packet_GetPacket(xhToken, ptszMsgBuffer, pInt_Len);
	SDPProtocol_Packet_Destory(xhToken);
}

void ParseSDP(LPCXSTR lpszMsgBuffer, int nLen)
{
	XNETHANDLE xhToken = 0;
	SDPProtocol_Parse_Create(&xhToken, lpszMsgBuffer, nLen);

	int nVersion = 0;
	SDPProtocol_Parse_GetVersion(xhToken, &nVersion);

	printf("nVersion:%d\n", nVersion);
	__int64x nSessionID = 0;
	__int64x nSessionVer = 0;
	int nIPVer = 0;
	XCHAR tszUserName[64];
	XCHAR tszIPVer[64];

	memset(tszUserName, '\0', sizeof(tszUserName));
	memset(tszIPVer, '\0', sizeof(tszIPVer));

	SDPProtocol_Parse_GetOwner(xhToken, tszUserName, &nSessionID, &nSessionVer, &nIPVer, tszIPVer);
	printf("SDPProtocol_Parse_GetOwner:%s %lld %s\n", tszUserName, nSessionVer, tszIPVer);
	nIPVer = 0;
	int nTTL = 0;
	int nCount = 0;
	XCHAR tszIPAddr[64];
	memset(tszIPAddr, '\0', sizeof(tszIPAddr));

	SDPProtocol_Parse_GetConnect(xhToken, &nIPVer, tszIPAddr, &nTTL, &nCount);

	bool bVideo = false;
	XCHAR tszSessionName[64];
	XCHAR tszSessionValue[64];
	memset(tszSessionValue, '\0', sizeof(tszSessionValue));
	memset(tszSessionName, '\0', sizeof(tszSessionName));
	SDPProtocol_Parse_GetSession(xhToken, tszSessionValue, tszSessionName, &bVideo);

	__int64x nTimeStart = 0;
	__int64x nTimeEnd = 0;
	SDPProtocol_Parse_GetTime(xhToken, &nTimeStart, &nTimeEnd);

	int nListCount = 0;
	STREAMMEDIA_SDPPROTOCOL_AVMEDIA** ppSt_AVMedia;;
	SDPProtocol_Parse_GetAVMedia(xhToken, &ppSt_AVMedia, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		printf("%s %s %d %d\n", ppSt_AVMedia[i]->tszAVType, ppSt_AVMedia[i]->tszProto, ppSt_AVMedia[i]->nCodec, ppSt_AVMedia[i]->nCodecNumber);
		printf("AVList Value:");
		for (int j = 0; j < ppSt_AVMedia[i]->nListCount; j++)
		{
			printf("%s ", ppSt_AVMedia[i]->pptszAVList[j]);
		}
		
		printf("\n");
	}
	
	int nACount = 0;
	STREAMMEDIA_SDPPROTOCOL_ATTR** ppSt_ListAttr;
	SDPProtocol_Parse_GetAttr(xhToken, &ppSt_ListAttr, &nACount);
	for (int i = 0; i < nACount; i++)
	{
		printf("Key:%s Value:%s\n", ppSt_ListAttr[i]->tszAttrKey, ppSt_ListAttr[i]->tszAttrValue);
	}
	int nIndex1 = -1;
	int nIndex2 = -1;
	SDPProtocol_Parse_AttrBundle(&ppSt_ListAttr, nACount, &nIndex1, &nIndex2);

	STREAMMEDIA_SDPPROTOCOL_MEDIAINFO st_ATTRVideo = {};
	SDPProtocol_Parse_RTPMapVideo(&ppSt_ListAttr, nACount, 96, &st_ATTRVideo);

	bool bRTCPMux = false;
	bool bRTCPRSize = false;
	SDPProtocol_Parse_RtcpComm(&ppSt_ListAttr, nACount, &bRTCPMux, &bRTCPRSize);
	
	STREAMMEDIA_SDPPROTOCOL_RTCP st_SDPRtcp = {};
	SDPProtocol_Parse_AVAttr(&ppSt_ListAttr, nACount, 106, &st_SDPRtcp);
	SDPProtocol_Parse_AVAttr(&ppSt_ListAttr, nACount, 9, &st_SDPRtcp);

	int nSsrcCount = 0;
	STREAMMEDIA_SDPPROTOCOL_CNAME** ppSt_CNameList;
	SDPProtocol_Parse_AttrCName(&ppSt_ListAttr, nACount, &ppSt_CNameList, &nSsrcCount);

	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_AVMedia[0]->pptszAVList, ppSt_AVMedia[0]->nListCount);
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListAttr, nACount);
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_AVMedia, nListCount);
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_CNameList, nSsrcCount);

	XCHAR tszMediaName[64];
	memset(tszMediaName, '\0', sizeof(tszMediaName));
	SDPProtocol_Parse_OptionalMediaName(xhToken, tszMediaName);

	XCHAR tszEmailAddr[64];
	XCHAR tszPhoneNumber[64];
	memset(tszEmailAddr, '\0', sizeof(tszEmailAddr));
	memset(tszPhoneNumber, '\0', sizeof(tszPhoneNumber));
	SDPProtocol_Parse_OptionalContact(xhToken, tszEmailAddr, tszPhoneNumber);

	int nBValue = 0;
	int nBType = 0;
	SDPProtocol_Parse_OptionalBandwidth(xhToken, &nBValue, &nBType);

	SDPProtocol_Parse_Destory(xhToken);
}

int main()
{
	int nSDPLen = 0;
	XCHAR tszMsgBuffer[8192];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	CreateSDP(tszMsgBuffer, &nSDPLen);
	printf("%s\n", tszMsgBuffer);

	ParseSDP(tszMsgBuffer, nSDPLen);
	return 0;
}