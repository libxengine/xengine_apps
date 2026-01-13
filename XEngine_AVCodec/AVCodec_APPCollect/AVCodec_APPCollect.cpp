#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
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
#include <XEngine_Include/XEngine_AVCodec/AudioCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AVCollect_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVCollect_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_VideoCodec.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVCollect.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVHelp.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AudioCodec/AudioCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_VideoCodec/VideoCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_VideoCodec/VideoCodec_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVCollect/AVCollect_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVCollect/AVCollect_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Error.h"
#ifdef _MSC_BUILD
#ifdef _WIN64
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_VideoCodec.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_AVCollect.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_AVHelp.lib")
#else
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_VideoCodec.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVCollect.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVHelp.lib")
#endif
#endif
#endif

//Linux MacOS:g++ -std=c++20 -Wall -g AVCodec_APPCollect.cpp -o AVCodec_APPCollect.exe -lXEngine_BaseLib -lXEngine_VideoCodec -lXEngine_AVCollect -lXEngine_AVHelp

XHANDLE xhToken;
XENGINE_PROTOCOL_AVINFO st_AVInfo;
FILE* pSt_File;
int nWidth;
int nHeight;
void XCALLBACK XEngine_AVCollect_CBVideo(XHANDLE*** pSt_AVBuffer, XPVOID lParam)
{
	XHANDLE** ppSt_AVFrame;
	AVCODEC_TIMESTAMP st_TimeStamp = {};
	VideoCodec_Help_ScaleConvert(xhToken, (*pSt_AVBuffer)[0], &ppSt_AVFrame);

	XENGINE_MSGBUFFER st_MSGBuffer = {};
	AVHelp_Memory_GetVideoBuffer(ppSt_AVFrame[0], &st_MSGBuffer, false);
	printf("XEngine_AVCollect_CBVideo:%d\n", st_MSGBuffer.nMSGLen[0]);
	fwrite(st_MSGBuffer.unData.ptszMSGBuffer, 1, st_MSGBuffer.nMSGLen[0], pSt_File);
	BaseLib_Memory_MSGFree(&st_MSGBuffer);
	AVHelp_Memory_FreeAVList(&ppSt_AVFrame, 1, false);
}
void XCALLBACK XEngine_AVCollect_CBAudio(XHANDLE*** pSt_AVBuffer, XPVOID lParam)
{

}
int main()
{
	XHANDLE xhVideo;
	memset(&st_AVInfo, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));

#ifdef _MSC_BUILD
	pSt_File = fopen("D:\\h264 file\\ds.yuv", "wb");
#else
	pSt_File = fopen("./ds.yuv", "wb");
#endif
	AVCOLLECT_SCREENINFO st_AVScreen;
	memset(&st_AVScreen, '\0', sizeof(AVCOLLECT_SCREENINFO));

	sprintf(st_AVScreen.tszVideoSize, _X("1920x1080"));
#ifdef _MSC_BUILD
	xhVideo = AVCollect_Video_Init("gdigrab", "desktop", &st_AVScreen, XEngine_AVCollect_CBVideo);
#else
	xhVideo = AVCollect_Video_Init("x11grab", ":0.0", &st_AVScreen, XEngine_AVCollect_CBVideo);
#endif
	if (NULL == xhVideo)
	{
		printf(_X("初始化失败"));
		return -1;
	}
	AVCollect_Video_GetInfo(xhVideo, &st_AVInfo);

	xhToken = VideoCodec_Help_ScaleInit(st_AVInfo.st_VideoInfo.nWidth, st_AVInfo.st_VideoInfo.nHeight, st_AVInfo.st_VideoInfo.nFormat, st_AVInfo.st_VideoInfo.nWidth, st_AVInfo.st_VideoInfo.nHeight, ENUM_AVCODEC_VIDEO_SAMPLEFMT_YUV420P);
	printf("AVCollect_Screen_GetInfo:%d %d %lld\n", st_AVInfo.st_VideoInfo.nWidth, st_AVInfo.st_VideoInfo.nHeight, st_AVInfo.st_VideoInfo.nBitRate);
	AVCollect_Video_Start(xhVideo);
	/*
	XHANDLE xhAudio = AVCollect_Audio_Init("dshow", _X("audio=virtual-audio-capturer"), XEngine_AVCollect_CBAudio);
	if (NULL == xhAudio)
	{
		printf(_X("初始化失败"));
		return -1;
	}
	AVCollect_Audio_GetInfo(xhAudio, &st_AVInfo);
	printf("AVCollect_Audio_GetInfo:%d %lld\n", st_AVInfo.st_AudioInfo.nSampleFmt, st_AVInfo.st_AudioInfo.nBitRate);
	AVCollect_Audio_Start(xhAudio);
	*/
	std::this_thread::sleep_for(std::chrono::seconds(600));
	AVCollect_Video_Destory(xhVideo);
	fclose(pSt_File);
	return 0;
}