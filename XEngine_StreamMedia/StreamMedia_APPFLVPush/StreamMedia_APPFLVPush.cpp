#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/StreamMedia_XClient.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVCollect.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AVCollect/AVCollect_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_VideoCoder/VideoCoder_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AudioCoder/AudioCoder_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_XClient/XClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_XClient/XClient_Error.h"

//g++ -std=c++17 -Wall -g StreamMedia_APPFLVPush.cpp -o StreamMedia_APPFLVPush.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia -lXEngine_BaseLib -lStreamMedia_XClient -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia,--disable-new-dtags

FILE* pSt_VFile;
FILE* pSt_AFile;
LPCTSTR lpszVFile = _T("D:\\h264 file\\480p.264");
LPCTSTR lpszAFile = _T("D:\\h264 file\\test.aac");

int fread_video(LPVOID lParam, uint8_t* puszMsgBuffer, int nSize)
{
	//printf("%d\n", nSize);
	int nRet = fread(puszMsgBuffer, 1, 1024, pSt_VFile);
	if (nRet <= 0)
	{
		fclose(pSt_VFile);
		pSt_VFile = fopen(lpszVFile, "rb");
		nRet = fread(puszMsgBuffer, 1, nSize, pSt_VFile);
		return nRet;
	}
	return nRet;
}
int fread_audio(LPVOID lParam, uint8_t* puszMsgBuffer, int nSize)
{
	printf("%d\n", nSize);
	int nRet = fread(puszMsgBuffer, 1, 1024, pSt_AFile);
	if (nRet <= 0)
	{
		fclose(pSt_AFile);
		pSt_AFile = fopen(lpszAFile, "rb");
		nRet = fread(puszMsgBuffer, 1, nSize, pSt_AFile);
		return nRet;
	}
	return nRet;
}

int Test_RTMPPush()
{
	XNETHANDLE xhStream = 0;
	LPCTSTR lpszUrl = _T("rtmp://stream.xyry.org/live/qyt");
	BOOL bMemory = FALSE;

	if (bMemory)
	{
		pSt_VFile = fopen(lpszVFile, "rb");
		if (NULL == pSt_VFile)
		{
			printf("fopen:%d\n", errno);
			return -1;
		}
		pSt_AFile = fopen(lpszAFile, "rb");
		if (NULL == pSt_VFile)
		{
			printf("fopen:%d\n", errno);
			return -1;
		}

		if (!XClient_FilePush_Init(&xhStream))
		{
			printf("XClient_FilePush_Push:%lX\n", StreamClient_GetLastError());
			return -1;
		}
		if (!XClient_FilePush_Input(xhStream, NULL, NULL, fread_video, NULL, NULL, NULL))
		{
			printf("XClient_FilePush_Input:%lX\n", StreamClient_GetLastError());
			//return -1;
		}
		if (!XClient_FilePush_Output(xhStream, lpszUrl))
		{
			printf("XClient_FilePush_Output:%lX\n", StreamClient_GetLastError());
			return -1;
		}
		if (!XClient_FilePush_Start(xhStream))
		{
			printf("XClient_FilePush_Output:%lX\n", StreamClient_GetLastError());
			return -1;
		}
	}
	else
	{
		if (!XClient_FilePush_Init(&xhStream))
		{
			printf("XClient_FilePush_Push:%lX\n", StreamClient_GetLastError());
			return -1;
		}
		if (!XClient_FilePush_Input(xhStream, lpszVFile, NULL))
		{
			printf("XClient_FilePush_Input:%lX\n", StreamClient_GetLastError());
			return -1;
		}
		if (!XClient_FilePush_Output(xhStream, lpszUrl))
		{
			printf("XClient_FilePush_Output:%lX\n", StreamClient_GetLastError());
			return -1;
		}
		if (!XClient_FilePush_Start(xhStream))
		{
			printf("XClient_FilePush_Output:%lX\n", StreamClient_GetLastError());
			return -1;
		}
	}

	BOOL bIsPush = TRUE;
	while (bIsPush)
	{
		XClient_FilePush_GetStatus(xhStream, &bIsPush);
	}
	XClient_FilePush_Close(xhStream);
	return 1;
}

int Test_LivePush()
{
	XNETHANDLE xhStream = 0;
	LPCTSTR lpszUrl = _T("rtmp://stream.xyry.org/live/qyt");

	XENGINE_PROTOCOL_AVINFO st_MediaStream;
	memset(&st_MediaStream, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));

	st_MediaStream.st_PushVideo.bEnable = TRUE;
	st_MediaStream.st_PushVideo.nBitRate = 64000;
	st_MediaStream.st_PushVideo.nFrameRate = 24;
	st_MediaStream.st_PushVideo.nWidth = 720;
	st_MediaStream.st_PushVideo.nHeight = 480;
	st_MediaStream.st_PushVideo.enAvCodec = 27;

	st_MediaStream.st_PushAudio.bEnable = FALSE;
	st_MediaStream.st_PushAudio.nChannel = 2;
	st_MediaStream.st_PushAudio.nBitRate = 64000;
	st_MediaStream.st_PushAudio.nSampleRate = 44100;
	st_MediaStream.st_PushAudio.nSampleFmt = ENUM_AVCOLLECT_AUDIO_SAMPLE_FMT_FLTP;
	st_MediaStream.st_PushAudio.enAvCodec = ENUM_AVCODEC_AUDIO_TYPE_AAC;
	XClient_CodecPush_Init(&xhStream, lpszUrl, &st_MediaStream, "flv", TRUE);
	//XClient_CodecPush_WriteHdr(xhStream);// 如果你有视频并且写入了SPSPPS信息到初始化,那么可以优先调用,否则必须写入后才可调用
	FILE* pSt_VFile = fopen(lpszVFile, "rb");
	if (NULL == pSt_VFile)
	{
		printf("fopen:%d\n", errno);
		return -1;
	}
	FILE* pSt_AFile = fopen(lpszAFile, "rb");
	if (NULL == pSt_AFile)
	{
		printf("fopen:%d\n", errno);
		return -1;
	}

	BOOL bInit = FALSE;
	while (TRUE)
	{
		TCHAR tszVBuffer[8096];
		memset(tszVBuffer, '\0', sizeof(tszVBuffer));
		int nLen = fread(tszVBuffer, 1, sizeof(tszVBuffer), pSt_VFile);
		if (nLen <= 0)
		{
			fseek(pSt_VFile, 0, SEEK_SET);
			continue;
		}
		while (1)
		{
			if (XClient_CodecPush_PushVideo(xhStream, tszVBuffer, nLen))
			{
				break;
			}
		}

		if (!bInit)
		{
			XClient_CodecPush_WriteHdr(xhStream);
			bInit = TRUE;
		}
		/*
		TCHAR tszABuffer[2048];
		memset(tszABuffer, '\0', sizeof(tszABuffer));

		int nRet = fread(tszABuffer, 1, sizeof(tszABuffer), pSt_AFile);
		if (nRet <= 0)
		{
			fseek(pSt_AFile, 0, SEEK_SET);
			continue;
		}
		XClient_CodecPush_PushAudio(xhStream, tszABuffer, nRet);
		*/
	}
	XClient_CodecPush_Close(xhStream);
	return 0;
}

XNETHANDLE xhStream;
void __stdcall XEngine_AVCollect_CBScreen(uint8_t* punStringY, int nYLen, uint8_t* punStringU, int nULen, uint8_t* punStringV, int nVLen, LPVOID lParam)
{
	XClient_StreamPush_PushVideo(xhStream, punStringY, nYLen, punStringU, nULen, punStringV, nVLen);
}
void __stdcall XEngine_AVCollect_CBAudio(uint8_t* punStringAudio, int nVLen, LPVOID lParam)
{

}
void test_Screen()
{
	XNETHANDLE xhScreen;
	XNETHANDLE xhAudio;
	XENGINE_PROTOCOL_AVINFO st_AVProtocol;

	memset(&st_AVProtocol, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));

	if (!AVCollect_Screen_Init(&xhScreen, XEngine_AVCollect_CBScreen, NULL, "1920x1080", 0, 0, 24))
	{
		return;
	}
	LPCTSTR lpszAudioStr = _T("virtual-audio-capturer");
	if (!AVCollect_Audio_Init(&xhAudio, lpszAudioStr, XEngine_AVCollect_CBAudio))
	{
		return;
	}
	int nWidth = 0;
	int nHeight = 0;
	int64_t nVideoBit = 0;
	int64_t nAudioBit = 0;
	int nSampleRate = 0;
	int nChannel = 0;
	ENUM_AVCOLLECT_AUDIOSAMPLEFORMAT enAudioFmt;
	AVCollect_Screen_GetInfo(xhScreen, &nWidth, &nHeight, &nVideoBit);
	AVCollect_Audio_GetInfo(xhAudio, &enAudioFmt, &nAudioBit, &nSampleRate, &nChannel);

	st_AVProtocol.st_PushVideo.bEnable = TRUE;
	st_AVProtocol.st_PushVideo.enAvCodec = ENUM_ENTENGINE_AVCODEC_VEDIO_TYPE_H264;
	st_AVProtocol.st_PushVideo.nBitRate = nVideoBit;
	st_AVProtocol.st_PushVideo.nFrameRate = 24;
	st_AVProtocol.st_PushVideo.nHeight = nHeight;
	st_AVProtocol.st_PushVideo.nWidth = nWidth;

	st_AVProtocol.st_PushAudio.bEnable = TRUE;
	st_AVProtocol.st_PushAudio.enAvCodec = ENUM_AVCODEC_AUDIO_TYPE_MP3;
	st_AVProtocol.st_PushAudio.nBitRate = nAudioBit;
	st_AVProtocol.st_PushAudio.nChannel = nChannel;
	st_AVProtocol.st_PushAudio.nSampleFmt = enAudioFmt;
	st_AVProtocol.st_PushAudio.nSampleRate = nSampleRate;

	if (!XClient_StreamPush_Init(&xhStream, "rtmp://stream.xyry.org/live/qyt", &st_AVProtocol, "flv"))
	{
		return;
	}
	AVCollect_Screen_Start(xhScreen);

	while (1)
	{
		Sleep(1000);
	}
}
int main()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	test_Screen();
	//Test_LivePush();
	//Test_RTMPPush();
	
#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}