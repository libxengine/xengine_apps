#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVCollect.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_VideoCodec.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AudioCodec.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVHelp.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVPacket.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
using namespace std;
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
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVPacket/AVPacket_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVPacket/AVPacket_Error.h"

//Linux::g++ -std=c++17 -Wall -g AVCodec_APPRecordMaster.cpp -o AVCodec_APPRecordMaster.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCodec -lXEngine_BaseLib -lXEngine_AVCollect -lXEngine_VideoCodec -lXEngine_AudioCodec -lXEngine_AVHelp -lXEngine_AVPacket -lpthread -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCodec,--disable-new-dtags
//MacOS::g++ -std=c++17 -Wall -g AVCodec_APPRecordMaster.cpp -o AVCodec_APPRecordMaster.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_AVCodec -lXEngine_BaseLib -lXEngine_AVCollect -lXEngine_VideoCodec -lXEngine_AudioCodec -lXEngine_AVHelp -lXEngine_AVPacket -lpthread
XBOOL bAudio = XFALSE;
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

void CALLBACK XEngine_AVCollect_CBScreen(uint8_t* punStringY, int nYLen, uint8_t* punStringU, int nULen, uint8_t* punStringV, int nVLen, XPVOID lParam)
{
	int nFLen = 1024 * 1024 * 10;
	int nELen = 1024 * 1024 * 10;
	XCHAR* ptszFilterBuffer = (XCHAR*)malloc(nFLen);
	XCHAR* ptszEncodeBuffer = (XCHAR*)malloc(nELen);
	if ((NULL == ptszFilterBuffer) || (NULL == ptszEncodeBuffer))
	{
		return;
	}
	memset(ptszFilterBuffer, '\0', nFLen);
	memset(ptszEncodeBuffer, '\0', nELen);

	if (VideoCodec_Help_FilterCvt(xhFilter, punStringY, punStringU, punStringV, nYLen, nULen, nVLen, (uint8_t*)ptszFilterBuffer, &nFLen))
	{
		VideoCodec_Stream_EnCodec(xhVideo, (uint8_t*)ptszFilterBuffer, NULL, NULL, nFLen, 0, 0, (uint8_t*)ptszEncodeBuffer, &nELen);
		if (nELen > 0)
		{
			fwrite(ptszEncodeBuffer, 1, nELen, pSt_VideoFile);
		}
	}
	free(ptszFilterBuffer);
	free(ptszEncodeBuffer);

	ptszFilterBuffer = NULL;
	ptszEncodeBuffer = NULL;
}
void CALLBACK XEngine_AVCollect_CBAudio(uint8_t* punStringAudio, int nVLen, XPVOID lParam)
{
	int nListCount = 0;
	AVCODEC_AUDIO_MSGBUFFER** ppSt_ListMsgBuffer;
	AudioCodec_Stream_EnCodec(xhAudio, punStringAudio, nVLen, &ppSt_ListMsgBuffer, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		XBYTE byAACHdr[8];
		AVHelp_Packet_AACHdr(byAACHdr, st_AVInfo.st_AudioInfo.nSampleRate, st_AVInfo.st_AudioInfo.nChannel, ppSt_ListMsgBuffer[i]->nMsgLen);

		fwrite(byAACHdr, 1, 7, pSt_AudioFile);
		fwrite(ppSt_ListMsgBuffer[i]->pbyMsgBuffer, 1, ppSt_ListMsgBuffer[i]->nMsgLen, pSt_AudioFile);
	}
	BaseLib_OperatorMemory_Free((void***)&ppSt_ListMsgBuffer, nListCount);
}

void CALLBACK XEngine_AVPacket_Callback(XHANDLE xhNet, int nCvtType, int nCvtFrame, double dlTime, XPVOID lParam)
{
	printf(_T("提示:正在打包...第 %d 个%s,当前时间:%lf\r\n"), nCvtFrame, nCvtType == 1 ? _T("视频帧") : _T("音频帧"), dlTime);
}
void XEngine_AVPacket_Thread()
{
	while (1)
	{
		XBOOL bIsRun = XFALSE;
		if (AVPacket_FilePacket_GetStatus(xhPacket, &bIsRun))
		{
			if (!bIsRun)
			{
				break;
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	AVPacket_FilePacket_Stop(xhPacket);
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
		printf("Audio:%s\n", ppSt_AudioList[i]->st_MetaInfo.tszKey);
	}
	for (int i = 0; i < nVCount; i++)
	{
		printf("Video:%s\n", ppSt_VideoList[i]->st_MetaInfo.tszKey);
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
			printf(_T("初始化音频采集器失败"));
			return -1;
		}
		AVCollect_Audio_GetInfo(xhSound, &st_AVInfo);
		//文件保存需要的属性
		if (!AudioCodec_Stream_EnInit(&xhAudio, ENUM_AVCODEC_AUDIO_TYPE_AAC, st_AVInfo.st_AudioInfo.nSampleRate, st_AVInfo.st_AudioInfo.nChannel, st_AVInfo.st_AudioInfo.nBitRate, 0, ENUM_AVCOLLECT_AUDIO_SAMPLE_FMT_FLTP))
		{
			printf(_T("初始化编码器失败"));
			return -1;
		}
		int nLen = 0;
		if (!AudioCodec_Stream_SetResample(xhAudio, &nLen, st_AVInfo.st_AudioInfo.nSampleRate, st_AVInfo.st_AudioInfo.nSampleRate, (ENUM_AVCOLLECT_AUDIOSAMPLEFORMAT)st_AVInfo.st_AudioInfo.nSampleFmt, ENUM_AVCOLLECT_AUDIO_SAMPLE_FMT_FLTP, st_AVInfo.st_AudioInfo.nChannel, st_AVInfo.st_AudioInfo.nChannel))
		{
			printf(_T("初始化重采样工具失败"));
			return -1;
		}
		//是否需要保存为文件
		pSt_AudioFile = fopen(lpszAudioFile, _T("wb"));
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
		printf(_T("初始化屏幕采集失败"));
		return -1;
	}
	AVCollect_Video_GetInfo(xhScreen, &st_AVInfo);
	//视频编码
	if (!VideoCodec_Stream_EnInit(&xhVideo, st_AVInfo.st_VideoInfo.nWidth, st_AVInfo.st_VideoInfo.nHeight, ENUM_ENTENGINE_AVCODEC_VEDIO_TYPE_H264, st_AVInfo.st_VideoInfo.nBitRate))
	{
		printf(_T("初始化失败"));
		return -1;
	}
	XCHAR tszFilterStr[MAX_PATH];
	memset(tszFilterStr, '\0', MAX_PATH);

	sprintf(tszFilterStr, _T("drawtext=fontfile=Arial.ttf:fontcolor=green:fontsize=30:x=100:y=10:text='www.xyry.org'"));
	if (!VideoCodec_Help_FilterInit(&xhFilter, tszFilterStr, st_AVInfo.st_VideoInfo.nWidth, st_AVInfo.st_VideoInfo.nHeight, ENUM_AVCOLLECT_VIDEO_FMT_YUV420P, 24))
	{
		printf(_T("初始化过滤器失败"));
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
	VideoCodec_Help_FilterDestroy(xhFilter);

	if (NULL != pSt_VideoFile)
	{
		fclose(pSt_VideoFile);
	}
	if (NULL != pSt_AudioFile)
	{
		fclose(pSt_AudioFile);
	}

	//是否需要打包
	double dlVideoTime = 0;
	double dlAudioTime = 0;

	xhPacket = AVPacket_FilePacket_Init(XEngine_AVPacket_Callback);
	if (NULL == xhPacket)
	{
		printf(_T("初始化打包工具失败"));
		return -1;
	}

	if (bAudio)
	{
		AVPacket_FilePacket_Input(xhPacket, lpszAudioFile, &dlAudioTime);
	}
	AVPacket_FilePacket_Input(xhPacket, lpszVideoFile, &dlVideoTime);

	if (!AVPacket_FilePacket_Output(xhPacket, lpszMP4File))
	{
		printf(_T("设置输出失败"));
		return -1;
	}
	if (!AVPacket_FilePacket_Start(xhPacket))
	{
		printf(_T("开始打包失败"));
		return -1;
	}
	thread m_AVThread(XEngine_AVPacket_Thread);
	m_AVThread.join();
	return 0;
}

