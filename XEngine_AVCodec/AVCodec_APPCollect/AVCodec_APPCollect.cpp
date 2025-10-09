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
#include <XEngine_Include/XEngine_AVCodec/AudioCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVCollect_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVCollect_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVCollect.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AudioCodec/AudioCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_VideoCodec/VideoCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVCollect/AVCollect_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVCollect/AVCollect_Error.h"
#ifdef _MSC_BUILD
#ifdef _WIN64
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_AVCollect.lib")
#else
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVCollect.lib")
#endif
#endif
#endif

//Linux MacOS:g++ -std=c++20 -Wall -g AVCodec_APPCollect.cpp -o AVCodec_APPCollect.exe -lXEngine_BaseLib -lXEngine_AVCollect

FILE* pSt_File;
int nWidth;
int nHeight;
void XCALLBACK XEngine_AVCollect_CBVideo(AVCODEC_VIDEO_MSGBUFFER* pSt_MSGBuffer, XPVOID lParam)
{
	fwrite(pSt_MSGBuffer->st_MSGBuffer.unData.ptszMSGBuffer, 1, pSt_MSGBuffer->st_MSGBuffer.nMSGLen[0], pSt_File);
	printf("%d\n", pSt_MSGBuffer->st_MSGBuffer.nMSGLen[0]);
}
void XCALLBACK XEngine_AVCollect_CBAudio(AVCODEC_AUDIO_MSGBUFFER* pSt_MSGBuffer, XPVOID lParam)
{

}
int main()
{
	XHANDLE xhVideo;
	XENGINE_PROTOCOL_AVINFO st_AVInfo;
	memset(&st_AVInfo, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));

#ifdef _MSC_BUILD
	pSt_File = fopen("D:\\xengine_apps\\Debug\\ds.yuv", "wb");
#else
	pSt_File = fopen("./ds.yuv", "wb");
#endif
	AVCOLLECT_SCREENINFO st_AVScreen;
	memset(&st_AVScreen, '\0', sizeof(AVCOLLECT_SCREENINFO));

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
	printf("AVCollect_Screen_GetInfo:%d %d %lld\n", st_AVInfo.st_VideoInfo.nWidth, st_AVInfo.st_VideoInfo.nHeight, st_AVInfo.st_VideoInfo.nBitRate);
	AVCollect_Video_Start(xhVideo);

	XHANDLE xhAudio = AVCollect_Audio_Init("dshow", _X("audio=virtual-audio-capturer"), XEngine_AVCollect_CBAudio);
	if (NULL == xhAudio)
	{
		printf(_X("初始化失败"));
		return -1;
	}
	AVCollect_Audio_GetInfo(xhAudio, &st_AVInfo);
	printf("AVCollect_Audio_GetInfo:%d %lld\n", st_AVInfo.st_AudioInfo.nSampleFmt, st_AVInfo.st_AudioInfo.nBitRate);
	AVCollect_Audio_Start(xhAudio);

	std::this_thread::sleep_for(std::chrono::seconds(15));
	AVCollect_Video_Destory(xhVideo);
	fclose(pSt_File);
	return 0;
}