#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVCollect.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_VideoCoder.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AudioCoder.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVHelp.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVPacket.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AVCollect/AVCollect_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AVCollect/AVCollect_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_VideoCoder/VideoCoder_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_VideoCoder/VideoCoder_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AudioCoder/AudioCoder_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AudioCoder/AudioCoder_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AVHelp/AVHelp_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AVHelp/AVHelp_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AVPacket/AVPacket_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AVPacket/AVPacket_Error.h"

//Linux::g++ -std=c++17 -Wall -g AVCoder_APPRecordMaster.cpp -o AVCoder_APPRecordMaster.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCoder -lXEngine_BaseLib -lXEngine_AVCollect -lXEngine_VideoCoder -lXEngine_AudioCoder -lXEngine_AVHelp -lXEngine_AVPacket -lpthread -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCoder,--disable-new-dtags
//MacOS::g++ -std=c++17 -Wall -g AVCoder_APPRecordMaster.cpp -o AVCoder_APPRecordMaster.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_AVCoder -lXEngine_BaseLib -lXEngine_AVCollect -lXEngine_VideoCoder -lXEngine_AudioCoder -lXEngine_AVHelp -lXEngine_AVPacket -lpthread
BOOL bAudio = TRUE;
XNETHANDLE xhVideo = 0;
XNETHANDLE xhAudio = 0;
XNETHANDLE xhFilter = 0;
XHANDLE xhPacket = NULL;
FILE* pSt_AudioFile = NULL;
FILE* pSt_VideoFile = NULL;
__int64x nBitRate;
int nSmpRate;
int nChannel;

#ifdef _MSC_BUILD
LPCTSTR lpszVideoFile = "D:\\xengine_apps\\Debug\\Video.h264";
LPCTSTR lpszAudioFile = "D:\\xengine_apps\\Debug\\Audio.aac";
LPCTSTR lpszMP4File = "D:\\xengine_apps\\Debug\\1.mp4";
#else
LPCTSTR lpszVideoFile = "Video.h264";
LPCTSTR lpszAudioFile = "Audio.aac";
LPCTSTR lpszMP4File = "1.mp4";
#endif

void CALLBACK XEngine_AVCollect_CBScreen(uint8_t* punStringY, int nYLen, uint8_t* punStringU, int nULen, uint8_t* punStringV, int nVLen, LPVOID lParam)
{
	int nFLen = 1024 * 1024 * 10;
	int nELen = 1024 * 1024 * 10;
	TCHAR* ptszFilterBuffer = (TCHAR*)malloc(nFLen);
	TCHAR* ptszEncodeBuffer = (TCHAR*)malloc(nELen);
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
void CALLBACK XEngine_AVCollect_CBAudio(uint8_t* punStringAudio, int nVLen, LPVOID lParam)
{
	int nListCount = 0;
	AVCODEC_AUDIO_MSGBUFFER** ppSt_ListMsgBuffer;
	AudioCodec_Stream_EnCodec(xhAudio, punStringAudio, nVLen, &ppSt_ListMsgBuffer, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		BYTE byAACHdr[8];
		AVHelp_Packet_AACHdr(byAACHdr, nSmpRate, nChannel, ppSt_ListMsgBuffer[i]->nMsgLen);

		fwrite(byAACHdr, 1, 7, pSt_AudioFile);
		fwrite(ppSt_ListMsgBuffer[i]->pbyMsgBuffer, 1, ppSt_ListMsgBuffer[i]->nMsgLen, pSt_AudioFile);
	}
	BaseLib_OperatorMemory_Free((void***)&ppSt_ListMsgBuffer, nListCount);
}

void CALLBACK XEngine_AVPacket_Callback(XHANDLE xhNet, int nCvtType, int nCvtFrame, double dlTime, LPVOID lParam)
{
	printf(_T("提示:正在打包...第 %d 个%s,当前时间:%lf\r\n"), nCvtFrame, nCvtType == 1 ? _T("视频帧") : _T("音频帧"), dlTime);
}
void XEngine_AVPacket_Thread()
{
	while (1)
	{
		BOOL bIsRun = FALSE;
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
		xhSound = AVCollect_Audio_Init("virtual-audio-capturer", XEngine_AVCollect_CBAudio);
		if (NULL == xhSound)
		{
			printf(_T("初始化音频采集器失败"));
			return -1;
		}
		ENUM_AVCOLLECT_AUDIOSAMPLEFORMAT enAudioFmt;
		AVCollect_Audio_GetInfo(xhSound, &enAudioFmt, &nBitRate, &nSmpRate, &nChannel);
		//文件保存需要的属性
		if (!AudioCodec_Stream_EnInit(&xhAudio, ENUM_AVCODEC_AUDIO_TYPE_AAC, nSmpRate, nChannel, nBitRate, 0, ENUM_AVCOLLECT_AUDIO_SAMPLE_FMT_FLTP))
		{
			printf(_T("初始化编码器失败"));
			return -1;
		}
		int nLen = 0;
		if (!AudioCodec_Stream_SetResample(xhAudio, &nLen, nSmpRate, nSmpRate, ENUM_AVCOLLECT_AUDIO_SAMPLE_FMT_S16, ENUM_AVCOLLECT_AUDIO_SAMPLE_FMT_FLTP, nChannel, nChannel))
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
	strcpy(st_AVScreen.tszVideoSize, "1920x1080");
	strcpy(st_AVScreen.tszVideoDev, "video=screen-capture-recorder");

	XHANDLE xhScreen = AVCollect_Video_Init(&st_AVScreen, XEngine_AVCollect_CBScreen);
	if (NULL == xhScreen)
	{
		printf(_T("初始化屏幕采集失败"));
		return -1;
	}
	int nWidth = 0;
	int nHeight = 0;
	AVCollect_Video_GetInfo(xhScreen, &nWidth, &nHeight, &nBitRate);
	//视频编码
	if (!VideoCodec_Stream_EnInit(&xhVideo, nWidth, nHeight, ENUM_ENTENGINE_AVCODEC_VEDIO_TYPE_H264, nBitRate))
	{
		printf(_T("初始化失败"));
		return -1;
	}
	TCHAR tszFilterStr[MAX_PATH];
	memset(tszFilterStr, '\0', MAX_PATH);

	sprintf(tszFilterStr, _T("drawtext=fontfile=Arial.ttf:fontcolor=green:fontsize=30:x=100:y=10:text='www.xyry.org'"));
	if (!VideoCodec_Help_FilterInit(&xhFilter, tszFilterStr, nWidth, nHeight, ENUM_AVCOLLECT_VIDEO_FMT_YUV420P, 24))
	{
		printf(_T("初始化过滤器失败"));
		return -1;
	}
	//获得路径名
	pSt_VideoFile = fopen(lpszVideoFile, "wb");
	AVCollect_Video_Start(xhScreen);

	std::this_thread::sleep_for(std::chrono::seconds(20));

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

