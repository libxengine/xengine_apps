#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>
#include <thread>
using namespace std;
#if 1 == _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AVCollect_Define.h>
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AudioCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AVFrame_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVFrame_Error.h>
#include <XEngine_Include/XEngine_Client/StreamClient_Define.h>
#include <XEngine_Include/XEngine_Client/StreamClient_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_Client/XClient_Stream.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVHelp.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVFrame.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVCollect/AVCollect_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_VideoCodec/VideoCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AudioCodec/AudioCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFrame/AVFrame_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFrame/AVFrame_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Stream/StreamClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Stream/StreamClient_Error.h"
#ifdef _MSC_BUILD
#ifdef _WIN64
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XClient_Stream.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_AVHelp.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_AVFrame.lib")
#else
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_Stream.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVHelp.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVFrame.lib")
#endif
#endif
#endif

//Linux::g++ -std=c++20 -Wall -g XClient_Stream.cpp -o XClient_Stream.exe -lXEngine_BaseLib -lXClient_Stream -lXEngine_AVHelp -lXEngine_AVFrame

FILE* pSt_VFile;
FILE* pSt_AFile;
#ifdef _MSC_BUILD
LPCXSTR lpszVFile = _X("D:\\h264 file\\480p.264");
LPCXSTR lpszAFile = _X("D:\\h264 file\\test.aac");
#else
LPCXSTR lpszVFile = _X("480p.264");
LPCXSTR lpszAFile = _X("test.aac");
#endif

void XCALLBACK CBStream_Pull(uint8_t* puszMsgBuffer, int nSize, int nAVType, __int64x nPts, __int64x nDts, __int64x nDuration, double dlTime, XPVOID lParam)
{
	//fwrite(puszMsgBuffer, 1, nSize, pSt_VFile);
	printf("Size:%d,AV:%d,Time:%lf\n", nSize, nAVType, dlTime);
	return;
}
int Test_RTMPPush()
{
	XHANDLE xhStream = NULL;
	LPCXSTR lpszUrl = _X("rtmp://app.xyry.org/live/qyt");

	xhStream = XClient_StreamPush_FileInit();
	if (NULL == xhStream)
	{
		printf("XClient_StreamPush_FileInit:%lX\n", StreamClient_GetLastError());
		return -1;
	}
	if (!XClient_StreamPush_FileInput(xhStream, lpszVFile))
	{
		printf("XClient_StreamPush_FileInput:%lX\n", StreamClient_GetLastError());
		return -1;
	}
	if (!XClient_StreamPush_FileOutput(xhStream, lpszUrl))
	{
		printf("XClient_StreamPush_FileOutput:%lX\n", StreamClient_GetLastError());
		return -1;
	}
	if (!XClient_StreamPush_FileStart(xhStream))
	{
		printf("XClient_StreamPush_FileStart:%lX\n", StreamClient_GetLastError());
		return -1;
	}

	bool bIsPush = true;
	while (bIsPush)
	{
		XClient_StreamPush_FileGetStatus(xhStream, &bIsPush);
	}
	XClient_StreamPush_FileClose(xhStream);
	return 1;
}

int Test_CodecPush()
{
	XHANDLE xhStream = NULL;
	FILE* pSt_VFile = NULL;
	FILE* pSt_AFile = NULL;
	LPCXSTR lpszUrl = _X("rtmp://app.xyry.org/live/qyt");

	int nPos = 0;
	int nVLen = 0;
	int nALen = 0;
	XCHAR tszVBuffer[8096];
	XCHAR tszABuffer[8096];
	XBYTE tszSPSBuffer[XPATH_MAX];
	XBYTE tszPPSBuffer[XPATH_MAX];
	XENGINE_PROTOCOL_AVINFO st_MediaStream;

	memset(tszVBuffer, '\0', sizeof(tszVBuffer));
	memset(tszABuffer, '\0', sizeof(tszABuffer));
	memset(tszSPSBuffer, '\0', XPATH_MAX);
	memset(tszPPSBuffer, '\0', XPATH_MAX);
	memset(&st_MediaStream, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));

	st_MediaStream.st_VideoInfo.bEnable = true;
	st_MediaStream.st_VideoInfo.nBitRate = 64000;
	st_MediaStream.st_VideoInfo.nFrameRate = 24;
	st_MediaStream.st_VideoInfo.nWidth = 720;
	st_MediaStream.st_VideoInfo.nHeight = 480;
	st_MediaStream.st_VideoInfo.enAVCodec = ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264;
	if (st_MediaStream.st_VideoInfo.bEnable)
	{
		pSt_VFile = fopen(lpszVFile, "rb");
		if (NULL == pSt_VFile)
		{
			printf("fopen:%d\n", errno);
			return -1;
		}
		nVLen = fread(tszVBuffer, 1, sizeof(tszVBuffer), pSt_VFile);
		AVHelp_Parse_VideoHdr(tszVBuffer, nVLen, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264, NULL, tszSPSBuffer, tszPPSBuffer, NULL, NULL, NULL, NULL, NULL, &nPos);

		st_MediaStream.st_VideoInfo.nVLen = nPos;
		memcpy(st_MediaStream.st_VideoInfo.tszVInfo, tszVBuffer, nPos);
	}

	st_MediaStream.st_AudioInfo.bEnable = true;
	st_MediaStream.st_AudioInfo.nChannel = 2;
	st_MediaStream.st_AudioInfo.nBitRate = 64000;
	st_MediaStream.st_AudioInfo.nSampleRate = 44100;
	st_MediaStream.st_AudioInfo.nFrameSize = 1024; //aac frame size
	st_MediaStream.st_AudioInfo.nSampleFmt = ENUM_AVCODEC_AUDIO_SAMPLEFMT_FLTP;
	st_MediaStream.st_AudioInfo.enAVCodec = ENUM_XENGINE_AVCODEC_AUDIO_TYPE_AAC;
	if (st_MediaStream.st_AudioInfo.bEnable)
	{
		pSt_AFile = fopen(lpszAFile, "rb");
		if (NULL == pSt_AFile)
		{
			printf("fopen:%d\n", errno);
			return -1;
		}
		nALen = fread(tszABuffer, 1, sizeof(tszABuffer), pSt_AFile);
		int nProfile = 0;
		int nConfig = 0;
		AVHelp_Parse_AACInfo((const XBYTE*)tszABuffer, nALen, &st_MediaStream.st_AudioInfo.nChannel, &st_MediaStream.st_AudioInfo.nSampleRate, &nProfile, &nConfig);
	}

	XNETHANDLE xhAParse = 0;
	XNETHANDLE xhVParse = 0;
	AVFrame_Frame_ParseInit(&xhAParse, ENUM_XENGINE_AVCODEC_AUDIO_TYPE_AAC);
	AVFrame_Frame_ParseInit(&xhVParse, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264);

	xhStream = XClient_StreamPush_CodecInit(lpszUrl, &st_MediaStream);
	XClient_StreamPush_CodecWriteHdr(xhStream);

	while (true)
	{
		if (st_MediaStream.st_VideoInfo.bEnable)
		{
			int nListCount = 0;
			AVFRAME_PARSEDATA** ppSt_Frame;
			AVFrame_Frame_ParseGet(xhVParse, tszVBuffer, nVLen, &ppSt_Frame, &nListCount);
			for (int i = 0; i < nListCount; i++)
			{
				XClient_StreamPush_CodecVideo(xhStream, (LPCXSTR)ppSt_Frame[i]->ptszMsgBuffer, ppSt_Frame[i]->nMsgLen);
			}
			memset(tszVBuffer, '\0', sizeof(tszVBuffer));
			nVLen = fread(tszVBuffer, 1, sizeof(tszVBuffer), pSt_VFile);
			if (nVLen <= 0)
			{
				fseek(pSt_VFile, 0, SEEK_SET);
				continue;
			}
		}
		if (st_MediaStream.st_AudioInfo.bEnable)
		{
			int nListCount = 0;
			AVFRAME_PARSEDATA** ppSt_Frame;
			AVFrame_Frame_ParseGet(xhAParse, tszABuffer, nALen, &ppSt_Frame, &nListCount);
			for (int i = 0; i < nListCount; i++)
			{
				//不需要AAC头
				XClient_StreamPush_CodecAudio(xhStream, (LPCXSTR)ppSt_Frame[i]->ptszMsgBuffer + 7, ppSt_Frame[i]->nMsgLen - 7);
			}
			memset(tszABuffer, '\0', sizeof(tszABuffer));
			nALen = fread(tszABuffer, 1, sizeof(tszABuffer), pSt_AFile);
			if (nALen <= 0)
			{
				fseek(pSt_AFile, 0, SEEK_SET);
				continue;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(40));
	}
	AVFrame_Frame_ParseClose(xhVParse);
	AVFrame_Frame_ParseClose(xhAParse);
	XClient_StreamPush_CodecClose(xhStream);
	return 0;
}

int Test_RTMPPull()
{
	//LPCXSTR lpszPullUrl = _X("rtmp://10.0.3.155/live/qyt");
	LPCXSTR lpszPullUrl = _X("srt://10.0.3.155:10080?streamid=#!::r=live/livestream,m=request");
	LPCXSTR lpszPushUrl = _X("rtmp://app.xyry.org/live/123");

	int nStreamCount = 0;
	XCLIENT_STREAMPULL** ppSt_PullStream;

	//pSt_VFile = fopen("D:\\windows-ffmpeg\\x64\\3.ts", "wb");
	XHANDLE xhStream = XClient_StreamPull_Init(lpszPullUrl, &ppSt_PullStream, &nStreamCount, CBStream_Pull);
	if (NULL == xhStream)
	{
		printf("XClient_StreamPull_Init:%lX\n", StreamClient_GetLastError());
		return -1;
	}

	for (int i = 0; i < nStreamCount; i++)
	{
		if (ppSt_PullStream[i]->enStreamType == ENUM_XCLIENT_STREAM_MEDIA_TYPE_VIDEO)
		{
			ppSt_PullStream[i]->bEnable = true;
		}
		else if (ppSt_PullStream[i]->enStreamType == ENUM_XCLIENT_STREAM_MEDIA_TYPE_AUDIO)
		{
			ppSt_PullStream[i]->bEnable = true;
		}
	}
	/*
	if (!XClient_StreamPull_PushStream(xhStream, lpszPushUrl, &ppSt_PullStream, nStreamCount))
	{
		printf("XClient_StreamPull_PushStream:%lX\n", StreamClient_GetLastError());
		return -1;
	}*/
	XENGINE_PROTOCOL_AVINFO st_MediaStream;
	memset(&st_MediaStream, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));

	XClient_StreamPull_Start(xhStream);
	bool bPull = true;

	while (bPull)
	{
		XClient_StreamPull_GetStatus(xhStream, &bPull);
	}
	XClient_StreamPull_Close(xhStream);
	return 1;
}
int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	Test_RTMPPull();

#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}