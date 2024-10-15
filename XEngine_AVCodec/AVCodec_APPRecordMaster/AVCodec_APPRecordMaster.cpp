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
#include <XEngine_Include/XEngine_AVCodec/AVCollect_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVCollect_Error.h>
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AudioCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AudioCodec_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AVFormat_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVFormat_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AVFilter_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVFilter_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVCollect.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_VideoCodec.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AudioCodec.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVHelp.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVFormat.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVFilter.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVCollect/AVCollect_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVCollect/AVCollect_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_VideoCodec/VideoCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_VideoCodec/VideoCodec_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AudioCodec/AudioCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AudioCodec/AudioCodec_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFormat/AVFormat_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFormat/AVFormat_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFilter/AVFilter_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFilter/AVFilter_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVCollect.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_VideoCodec.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AudioCodec.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVHelp.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVFormat.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVFilter.lib")
#endif
#endif

//Linux::g++ -std=c++17 -Wall -g AVCodec_APPRecordMaster.cpp -o AVCodec_APPRecordMaster.exe -lXEngine_BaseLib -lXEngine_AVCollect -lXEngine_VideoCodec -lXEngine_AudioCodec -lXEngine_AVHelp -lXEngine_AVPacket -lpthread
bool bAudio = false;
XNETHANDLE xhVideo = 0;
XNETHANDLE xhAudio = 0;
XNETHANDLE xhFilter = 0;
XHANDLE xhPacket = NULL;
FILE* pSt_AudioFile = NULL;
FILE* pSt_VideoFile = NULL;
XENGINE_PROTOCOL_AVINFO st_AVInfo;


#ifdef _MSC_BUILD
LPCXSTR lpszVideoFile = "D:\\xengine_apps\\Debug\\Video.h264";
LPCXSTR lpszAudioFile = "D:\\xengine_apps\\Debug\\Audio.aac";
LPCXSTR lpszMP4File = "D:\\xengine_apps\\Debug\\1.mp4";
#else
LPCXSTR lpszVideoFile = "Video.h264";
LPCXSTR lpszAudioFile = "Audio.aac";
LPCXSTR lpszMP4File = "1.mp4";
#endif

void CALLBACK XEngine_AVCollect_CBScreen(uint8_t* ptszAVBuffer, int nAVLen, AVCOLLECT_TIMEINFO* pSt_TimeInfo, XPVOID lParam)
{
	int nFLen = 1024 * 1024 * 10;
	int nELen = 1024 * 1024 * 10;
	XBYTE* ptszFilterBuffer = (XBYTE*)malloc(nFLen);
	if (NULL == ptszFilterBuffer)
	{
		return;
	}
	memset(ptszFilterBuffer, '\0', nFLen);

	if (AVFilter_Video_Cvt(xhFilter, ptszAVBuffer, nAVLen, (uint8_t*)ptszFilterBuffer, &nFLen))
	{
		int nListCount = 0;
		AVCODEC_VIDEO_MSGBUFFER** ppSt_MSGBuffer;
		VideoCodec_Stream_EnCodec(xhVideo, ptszFilterBuffer, nFLen, &ppSt_MSGBuffer, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			fwrite(ppSt_MSGBuffer[i]->ptszAVBuffer, 1, ppSt_MSGBuffer[i]->nAVLen, pSt_VideoFile);
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_MSGBuffer, nListCount);
	}
	free(ptszFilterBuffer);
	ptszFilterBuffer = NULL;
}
void CALLBACK XEngine_AVCollect_CBAudio(uint8_t* ptszAVBuffer, int nAVLen, AVCOLLECT_TIMEINFO* pSt_TimeInfo, XPVOID lParam)
{
	int nListCount = 0;
	AVCODEC_AUDIO_MSGBUFFER** ppSt_ListMsgBuffer;
	AudioCodec_Stream_EnCodec(xhAudio, ptszAVBuffer, nAVLen, &ppSt_ListMsgBuffer, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		XBYTE byAACHdr[8];
		AVHelp_Packet_AACHdr(byAACHdr, st_AVInfo.st_AudioInfo.nSampleRate, st_AVInfo.st_AudioInfo.nChannel, ppSt_ListMsgBuffer[i]->nMsgLen);

		fwrite(byAACHdr, 1, 7, pSt_AudioFile);
		fwrite(ppSt_ListMsgBuffer[i]->ptszMsgBuffer, 1, ppSt_ListMsgBuffer[i]->nMsgLen, pSt_AudioFile);
		BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ppSt_ListMsgBuffer[i]->ptszMsgBuffer);
	}
	BaseLib_OperatorMemory_Free((void***)&ppSt_ListMsgBuffer, nListCount);
}

void CALLBACK XEngine_AVPacket_Callback(XHANDLE xhNet, int nCvtType, __int64x nCvtFrame, double dlTime, XPVOID lParam)
{
	printf(_X("提示:正在打包...第 %lld 个%s,当前时间:%lf\r\n"), nCvtFrame, nCvtType == 1 ? _X("视频帧") : _X("音频帧"), dlTime);
}
void XEngine_AVPacket_Thread()
{
	while (1)
	{
		bool bIsRun = false;
		if (AVFormat_Packet_GetStatus(xhPacket, &bIsRun))
		{
			if (!bIsRun)
			{
				break;
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	AVFormat_Packet_Stop(xhPacket);
}

int main()
{
	int nACount = 0;
	int nVCount = 0;
	AVHELP_DEVICEINFO** ppSt_AudioList;
	AVHELP_DEVICEINFO** ppSt_VideoList;

	memset(&st_AVInfo, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));
	AVHelp_Device_EnumDevice(&ppSt_AudioList, &ppSt_VideoList, &nACount, &nVCount);
	for (int i = 0; i < nACount; i++)
	{
		printf("Audio:%s\n", ppSt_AudioList[i]->st_MetaInfo.tszStrKey);
	}
	for (int i = 0; i < nVCount; i++)
	{
		printf("Video:%s\n", ppSt_VideoList[i]->st_MetaInfo.tszStrKey);
	}
	BaseLib_OperatorMemory_Free((void***)&ppSt_AudioList, nACount);
	BaseLib_OperatorMemory_Free((void***)&ppSt_VideoList, nVCount);
	//启用音频
	XHANDLE xhSound = NULL;
	if (bAudio)
	{
		xhSound = AVCollect_Audio_Init("dshow", "audio=virtual-audio-capturer", XEngine_AVCollect_CBAudio);
		if (NULL == xhSound)
		{
			printf(_X("初始化音频采集器失败"));
			return -1;
		}
		AVCollect_Audio_GetInfo(xhSound, &st_AVInfo);
		//文件保存需要的属性
		st_AVInfo.st_AudioInfo.enAVCodec = ENUM_XENGINE_AVCODEC_AUDIO_TYPE_AAC;
		st_AVInfo.st_AudioInfo.nSampleFmt = ENUM_AVCODEC_AUDIO_SAMPLEFMT_FLTP;

		if (!AudioCodec_Stream_EnInit(&xhAudio, &st_AVInfo.st_AudioInfo))
		{
			printf(_X("初始化编码器失败"));
			return -1;
		}
		int nLen = 0;
		if (!AudioCodec_Stream_SetResample(xhAudio, &nLen, st_AVInfo.st_AudioInfo.nSampleRate, st_AVInfo.st_AudioInfo.nSampleRate, (ENUM_AVCODEC_AUDIO_SAMPLEFMT)st_AVInfo.st_AudioInfo.nSampleFmt, ENUM_AVCODEC_AUDIO_SAMPLEFMT_FLTP, st_AVInfo.st_AudioInfo.nChannel, st_AVInfo.st_AudioInfo.nChannel))
		{
			printf(_X("初始化重采样工具失败"));
			return -1;
		}
		//是否需要保存为文件
		pSt_AudioFile = fopen(lpszAudioFile, _X("wb"));
		AVCollect_Audio_Start(xhSound);
	}
	//屏幕采集
	AVCOLLECT_SCREENINFO st_AVScreen;
	memset(&st_AVScreen, '\0', sizeof(AVCOLLECT_SCREENINFO));

	st_AVScreen.nFrameRate = 24;
	st_AVScreen.nPosX = 0;
	st_AVScreen.nPosY = 0;

#ifdef _MSC_BUILD
	strcpy(st_AVScreen.tszVideoSize, "1920x1080");
	XHANDLE xhScreen = AVCollect_Video_Init("dshow", "video=screen-capture-recorder", &st_AVScreen, XEngine_AVCollect_CBScreen);
#else
	XHANDLE xhScreen = AVCollect_Video_Init("x11grab", ":0", &st_AVScreen, XEngine_AVCollect_CBScreen);
#endif
	if (NULL == xhScreen)
	{
		printf(_X("初始化屏幕采集失败"));
		return -1;
	}
	AVCollect_Video_GetInfo(xhScreen, &st_AVInfo);
	//视频编码
	st_AVInfo.st_VideoInfo.enAVCodec = ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264;
	if (!VideoCodec_Stream_EnInit(&xhVideo, &st_AVInfo.st_VideoInfo))
	{
		printf(_X("初始化失败"));
		return -1;
	}
	XCHAR tszFilterStr[MAX_PATH];
	memset(tszFilterStr, '\0', MAX_PATH);

	AVFILTER_VIDEO_INFO st_VideoInfo = {};
	st_VideoInfo.st_VideoInfo.nFormat = ENUM_AVCODEC_VIDEO_SAMPLEFMT_YUV420P;
	st_VideoInfo.st_VideoInfo.nFrameRate = 24;
	st_VideoInfo.st_VideoInfo.nHeight = st_AVInfo.st_VideoInfo.nHeight;
	st_VideoInfo.st_VideoInfo.nWidth = st_AVInfo.st_VideoInfo.nWidth;

	sprintf(tszFilterStr, _X("drawtext=fontfile=Arial.ttf:fontcolor=green:fontsize=30:x=100:y=10:text='www.xyry.org'"));
	if (!AVFilter_Video_Init(&xhFilter, tszFilterStr, &st_VideoInfo))
	{
		printf(_X("初始化过滤器失败"));
		return -1;
	}
	//获得路径名
	pSt_VideoFile = fopen(lpszVideoFile, "wb");
	AVCollect_Video_Start(xhScreen);

	std::this_thread::sleep_for(std::chrono::seconds(15));

	AVCollect_Video_Destory(xhScreen);
	AVCollect_Audio_Destory(xhSound);
	VideoCodec_Stream_Destroy(xhVideo);
	AudioCodec_Stream_Destroy(xhAudio);
	AVFilter_Video_Destroy(xhFilter);

	if (NULL != pSt_VideoFile)
	{
		fclose(pSt_VideoFile);
	}
	if (NULL != pSt_AudioFile)
	{
		fclose(pSt_AudioFile);
	}
	//是否需要打包
	xhPacket = AVFormat_Packet_Init(XEngine_AVPacket_Callback);
	if (NULL == xhPacket)
	{
		printf(_X("初始化打包工具失败"));
		return -1;
	}

	if (bAudio)
	{
		AVFormat_Packet_Input(xhPacket, lpszAudioFile);
	}
	AVFormat_Packet_Input(xhPacket, lpszVideoFile);

	if (!AVFormat_Packet_Output(xhPacket, lpszMP4File))
	{
		printf(_X("设置输出失败"));
		return -1;
	}
	if (!AVFormat_Packet_Start(xhPacket))
	{
		printf(_X("开始打包失败"));
		return -1;
	}
	thread m_AVThread(XEngine_AVPacket_Thread);
	m_AVThread.join();
	return 0;
}

