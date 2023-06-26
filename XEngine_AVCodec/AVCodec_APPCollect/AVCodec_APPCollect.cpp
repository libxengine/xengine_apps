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
#ifdef _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_AVCodec/AVCollect_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVCollect_Error.h>
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVCollect.lib")
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVCollect/AVCollect_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVCollect/AVCollect_Error.h"
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVCollect.lib")
#endif

//Linux::g++ -std=c++17 -Wall -g AVCodec_APPCollect.cpp -o AVCodec_APPCollect.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCodec -lXEngine_BaseLib -lXEngine_AVCollect -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCodec,--disable-new-dtags
//MacOS::g++ -std=c++17 -Wall -g AVCodec_APPCollect.cpp -o AVCodec_APPCollect.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_AVCodec -lXEngine_BaseLib -lXEngine_AVCollect

FILE* pSt_File;
int nWidth;
int nHeight;
void CALLBACK XEngine_AVCollect_CBVideo(uint8_t* punStringY, int nYLen, uint8_t* punStringU, int nULen, uint8_t* punStringV, int nVLen, XPVOID lParam)
{
	fwrite(punStringY, 1, nWidth * nHeight, pSt_File);
	fwrite(punStringU, 1, nWidth * nHeight / 4, pSt_File);
	fwrite(punStringV, 1, nWidth * nHeight / 4, pSt_File);
	printf("%d %d %d\n", nYLen, nULen, nVLen);
}
void CALLBACK XEngine_AVCollect_CBAudio(uint8_t* punStringAudio, int nVLen, XPVOID lParam)
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
	xhVideo = AVCollect_Video_Init("dshow", "video=screen-capture-recorder", &st_AVScreen, XEngine_AVCollect_CBVideo);
#else
	xhVideo = AVCollect_Video_Init("x11grab", ":0", &st_AVScreen, XEngine_AVCollect_CBVideo);
#endif
	if (NULL == xhVideo)
	{
		printf(_X("初始化失败"));
		return -1;
	}
	AVCollect_Video_GetInfo(xhVideo, &st_AVInfo);
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
	while (1)
	{
		std::this_thread::sleep_for(std::chrono::seconds(15));
	}
	AVCollect_Video_Destory(xhVideo);
	return 0;
}