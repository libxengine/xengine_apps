#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_SDPProtocol.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_SDPProtocol/SDPProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_SDPProtocol/SDPProtocol_Error.h"

//Linux::g++ -std=c++17 -Wall -g RfcComponents_APPSDPProtocol.cpp -o RfcComponents_APPSDPProtocol.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents -lXEngine_BaseLib -lRfcComponents_SDPProtocol -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g RfcComponents_APPSDPProtocol.cpp -o RfcComponents_APPSDPProtocol.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_RfcComponents -lXEngine_BaseLib -lRfcComponents_SDPProtocol

void CreateSDP(TCHAR* ptszMsgBuffer, int* pInt_Len)
{
	XNETHANDLE xhToken = 0;
	RfcComponents_SDPPacket_Create(&xhToken);

	RfcComponents_SDPPacket_Owner(xhToken, _T("qyt"), 123456789, _T("192.168.1.101"));
	RfcComponents_SDPPacket_Session(xhToken, _T("video.h264"));
	RfcComponents_SDPPacket_KeepTime(xhToken);
	//配置视频属性
	RfcComponents_SDPPacket_AddMedia(xhToken, _T("video"), 96);
	RFCCOMPONENTS_SDPPROTOCOL_MEDIAVIDEO st_SDPMediaVideo;
	memset(&st_SDPMediaVideo, '\0', sizeof(RFCCOMPONENTS_SDPPROTOCOL_MEDIAVIDEO));

	st_SDPMediaVideo.nTrackID = 1;
	st_SDPMediaVideo.st_RTPMap.nCodecNumber = 96;
	st_SDPMediaVideo.st_RTPMap.nSampleRate = 90000;
	strcpy(st_SDPMediaVideo.st_RTPMap.tszCodecName, _T("H264"));

	st_SDPMediaVideo.st_FmtpVideo.nFrameRate = 25;
	st_SDPMediaVideo.st_FmtpVideo.nPacketMode = 1;
	st_SDPMediaVideo.st_FmtpVideo.tszLeaveId[0] = 0x4D;
	st_SDPMediaVideo.st_FmtpVideo.tszLeaveId[1] = 0x40;
	st_SDPMediaVideo.st_FmtpVideo.tszLeaveId[2] = 0x33;
	st_SDPMediaVideo.st_FmtpVideo.nFrameXSize = 1080;
	st_SDPMediaVideo.st_FmtpVideo.nFrameYSize = 720;

	strcpy(st_SDPMediaVideo.st_FmtpVideo.tszPPSBase, _T("aO48gA"));
	strcpy(st_SDPMediaVideo.st_FmtpVideo.tszSPSBase, _T("Z01AM5p0FCNCAAEEugA9CQEeMGVA"));

	RfcComponents_SDPPacket_AttrVideo(xhToken, &st_SDPMediaVideo);
	//配置音频属性
	RfcComponents_SDPPacket_AddMedia(xhToken, _T("audio"), 97);

	RFCCOMPONENTS_SDPPROTOCOL_MEDIAAUDIO st_SDPMediaAudio;
	memset(&st_SDPMediaAudio, '\0', sizeof(RFCCOMPONENTS_SDPPROTOCOL_MEDIAAUDIO));
	st_SDPMediaAudio.nTrackID = 2;
	st_SDPMediaAudio.st_RTPMap.nChannel = 2;
	st_SDPMediaAudio.st_RTPMap.nCodecNumber = 97;
	st_SDPMediaAudio.st_RTPMap.nSampleRate = 80000;
	strcpy(st_SDPMediaAudio.st_RTPMap.tszCodecName, _T("mpeg4-generic"));

	st_SDPMediaAudio.st_FmtpAudio.nConfig = 1400;
	st_SDPMediaAudio.st_FmtpAudio.nDeltaLen = 3;
	st_SDPMediaAudio.st_FmtpAudio.nIndexLen = 3;
	st_SDPMediaAudio.st_FmtpAudio.nProfileID = 1;
	st_SDPMediaAudio.st_FmtpAudio.nSizeLen = 13;
	strcpy(st_SDPMediaAudio.st_FmtpAudio.tszMode, "AAC-hbr");
	RfcComponents_SDPPacket_AttrAudio(xhToken, &st_SDPMediaAudio);
	//附加信息
	RfcComponents_SDPPacket_OptionalMediaName(xhToken, "medianame");
	RfcComponents_SDPPacket_OptionalContact(xhToken, "486179@qq.com", "13699444444");
	RfcComponents_SDPPacket_OptionalBandwidth(xhToken, 10000);
	RfcComponents_SDPPacket_OptionalRange(xhToken);
	RfcComponents_SDPPacket_OptionalAddAttr(xhToken, "this is a exp");

	RfcComponents_SDPPacket_GetPacket(xhToken, ptszMsgBuffer, pInt_Len);
	RfcComponents_SDPPacket_Destory(xhToken);
}
void CreateSDP265(TCHAR* ptszMsgBuffer, int* pInt_Len)
{
	XNETHANDLE xhToken = 0;
	RfcComponents_SDPPacket_Create(&xhToken);

	RfcComponents_SDPPacket_Owner(xhToken, _T("qyt"), 123456789, _T("192.168.1.101"));
	RfcComponents_SDPPacket_Session(xhToken, _T("video.h265"));
	RfcComponents_SDPPacket_KeepTime(xhToken);
	//配置视频属性
	RfcComponents_SDPPacket_AddMedia(xhToken, _T("video"), 96);
	RFCCOMPONENTS_SDPPROTOCOL_MEDIAVIDEO st_SDPMediaVideo;
	memset(&st_SDPMediaVideo, '\0', sizeof(RFCCOMPONENTS_SDPPROTOCOL_MEDIAVIDEO));

	st_SDPMediaVideo.nTrackID = 1;
	st_SDPMediaVideo.st_RTPMap.nCodecNumber = 96;
	st_SDPMediaVideo.st_RTPMap.nSampleRate = 90000;
	strcpy(st_SDPMediaVideo.st_RTPMap.tszCodecName, _T("H265"));

	st_SDPMediaVideo.st_FmtpVideo.nFrameXSize = 1920;
	st_SDPMediaVideo.st_FmtpVideo.nFrameYSize = 1080;
	st_SDPMediaVideo.st_FmtpVideo.nFrameRate = 24;

	st_SDPMediaVideo.st_FmtpVideo.nProSpace = 0;
	st_SDPMediaVideo.st_FmtpVideo.nProID = 1;
	st_SDPMediaVideo.st_FmtpVideo.nFlags = 0;
	st_SDPMediaVideo.st_FmtpVideo.nLevelID = 0;

	strcpy(st_SDPMediaVideo.st_FmtpVideo.tszICStr, _T("000000000000"));
	strcpy(st_SDPMediaVideo.st_FmtpVideo.tszLeaveId, _T("QAEMAf//AWAAAAMAAAMAAAMAAAMAAJSQJA=="));
	strcpy(st_SDPMediaVideo.st_FmtpVideo.tszPPSBase, _T("RAHBpXwIkA=="));
	strcpy(st_SDPMediaVideo.st_FmtpVideo.tszSPSBase, _T("QgEBAWAAAAMAAAMAAAMAAAMAAKAB4CACHH+WUmSRthpYiqkxMMvs+vN+WfXhRGJy7ZA="));

	RfcComponents_SDPPacket_AttrVideo(xhToken, &st_SDPMediaVideo);
	//附加信息
	RfcComponents_SDPPacket_OptionalMediaName(xhToken, "medianame");
	RfcComponents_SDPPacket_OptionalContact(xhToken, "486179@qq.com", "13699444444");
	RfcComponents_SDPPacket_OptionalBandwidth(xhToken, 10000);
	RfcComponents_SDPPacket_OptionalRange(xhToken);
	RfcComponents_SDPPacket_OptionalAddAttr(xhToken, "this is a exp");

	RfcComponents_SDPPacket_GetPacket(xhToken, ptszMsgBuffer, pInt_Len);
	RfcComponents_SDPPacket_Destory(xhToken);
}
void ParseSDP(LPCTSTR lpszMsgBuffer, int nLen)
{
	XNETHANDLE xhToken = 0;
	RfcComponents_SDPParse_Create(&xhToken, lpszMsgBuffer, nLen);

	int nVersion = 0;
	RfcComponents_SDPParse_GetVersion(xhToken, &nVersion);

	printf("nVersion:%d\n", nVersion);
	__int64x nSessionID = 0;
	__int64x nSessionVer = 0;
	int nIPVer = 0;
	TCHAR tszUserName[64];
	TCHAR tszIPVer[64];

	memset(tszUserName, '\0', sizeof(tszUserName));
	memset(tszIPVer, '\0', sizeof(tszIPVer));

	RfcComponents_SDPParse_GetOwner(xhToken, tszUserName, &nSessionID, &nSessionVer, &nIPVer, tszIPVer);
	printf("RfcComponents_SDPParse_GetOwner:%s %" PRId64" %s\n", tszUserName, nSessionVer, tszIPVer);
	nIPVer = 0;
	int nTTL = 0;
	int nCount = 0;
	TCHAR tszIPAddr[64];
	memset(tszIPAddr, '\0', sizeof(tszIPAddr));

	RfcComponents_SDPParse_GetConnect(xhToken, &nIPVer, tszIPAddr, &nTTL, &nCount);

	BOOL bVideo = FALSE;
	TCHAR tszSessionName[64];
	TCHAR tszSessionValue[64];
	memset(tszSessionValue, '\0', sizeof(tszSessionValue));
	memset(tszSessionName, '\0', sizeof(tszSessionName));
	RfcComponents_SDPParse_GetSession(xhToken, tszSessionValue, tszSessionName, &bVideo);

	__int64x nTimeStart = 0;
	__int64x nTimeEnd = 0;
	RfcComponents_SDPParse_GetTime(xhToken, &nTimeStart, &nTimeEnd);

	int nACount = 0;
	int nVCount = 0;
	RFCCOMPONENTS_SDPPROTOCOL_AVVIDEO **ppSt_ListVideo;
	RFCCOMPONENTS_SDPPROTOCOL_AVAUDIO **ppSt_ListAudio;
	RfcComponents_SDPParse_GetMediaVideo(xhToken, &ppSt_ListVideo, &nVCount);
	RfcComponents_SDPParse_GetMediaAudio(xhToken, &ppSt_ListAudio, &nACount);
	for (int i = 0; i < nACount; i++)
	{
		printf("%s\n", ppSt_ListAudio[i]->tszAVType);
	}
	for (int i = 0; i < nVCount; i++)
	{
		printf("%s\n", ppSt_ListVideo[i]->tszAVType);
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListVideo, nVCount);
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListAudio, nACount);

	TCHAR tszMediaName[64];
	memset(tszMediaName, '\0', sizeof(tszMediaName));
	RfcComponents_SDPParse_OptionalMediaName(xhToken, tszMediaName);

	TCHAR tszEmailAddr[64];
	TCHAR tszPhoneNumber[64];
	memset(tszEmailAddr, '\0', sizeof(tszEmailAddr));
	memset(tszPhoneNumber, '\0', sizeof(tszPhoneNumber));
	RfcComponents_SDPParse_OptionalContact(xhToken, tszEmailAddr, tszPhoneNumber);

	int nBValue = 0;
	int nBType = 0;
	RfcComponents_SDPParse_OptionalBandwidth(xhToken, &nBValue, &nBType);

	int nListCount = 0;
	RFCCOMPONENTS_SDPPROTOCOL_ATTR **ppSt_ListAttr;
	RfcComponents_SDPParse_OptionalAttr(xhToken, &ppSt_ListAttr, &nListCount);
	RfcComponents_SDPParse_Destory(xhToken);
}

int main()
{
	int nSDPLen = 0;
	TCHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	CreateSDP(tszMsgBuffer, &nSDPLen);
	printf("%s\n", tszMsgBuffer);

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	CreateSDP265(tszMsgBuffer, &nSDPLen);
	printf("%s\n", tszMsgBuffer);

	ParseSDP(tszMsgBuffer, nSDPLen);
	return 0;
}