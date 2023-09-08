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
#ifdef _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AVCollect_Define.h>
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AudioCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Error.h>
#include <XEngine_Include/XEngine_StreamMedia/StreamClient_Define.h>
#include <XEngine_Include/XEngine_StreamMedia/StreamClient_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_StreamMedia/StreamMedia_StreamClient.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVHelp.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVCollect/AVCollect_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_VideoCodec/VideoCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AudioCodec/AudioCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_StreamClient/StreamClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_StreamClient/StreamClient_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/StreamMedia_StreamClient.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVHelp.lib")
#endif
#endif

//Linux::g++ -std=c++17 -Wall -g StreamMedia_APPClient.cpp -o StreamMedia_APPClient.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCodec -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia -lXEngine_BaseLib -lXEngine_AVHelp -lStreamMedia_StreamClient -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCodec:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g StreamMedia_APPClient.cpp -o StreamMedia_APPClient.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_AVCodec -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_StreamMedia -lXEngine_BaseLib -lXEngine_AVHelp -lStreamMedia_StreamClient

FILE* pSt_VFile;
FILE* pSt_AFile;
#ifdef _MSC_BUILD
LPCXSTR lpszVFile = _X("D:\\h264 file\\480p.264");
LPCXSTR lpszAFile = _X("D:\\h264 file\\test.aac");
#else
LPCXSTR lpszVFile = _X("480p.264");
LPCXSTR lpszAFile = _X("test.aac");
#endif

void CBStream_Pull(uint8_t* puszMsgBuffer, int nSize, int nAVType, __int64x nPts, __int64x nDts, __int64x nDuration, double dlTime, XPVOID lParam)
{
	//fwrite(puszMsgBuffer, 1, nSize, pSt_VFile);
	printf("Size:%d,AV:%d,Time:%lf\n", nSize, nAVType, dlTime);
	return;
}
int Test_RTMPPush()
{
	XHANDLE xhStream = NULL;
	LPCXSTR lpszUrl = _X("rtmp://app.xyry.org/live/qyt");

	xhStream = StreamClient_FilePush_Init();
	if (NULL == xhStream)
	{
		printf("StreamClient_FilePush_Push:%lX\n", StreamClient_GetLastError());
		return -1;
	}
	if (!StreamClient_FilePush_Input(xhStream, lpszVFile))
	{
		printf("StreamClient_FilePush_Input:%lX\n", StreamClient_GetLastError());
		return -1;
	}
	if (!StreamClient_FilePush_Output(xhStream, lpszUrl))
	{
		printf("StreamClient_FilePush_Output:%lX\n", StreamClient_GetLastError());
		return -1;
	}
	if (!StreamClient_FilePush_Start(xhStream))
	{
		printf("StreamClient_FilePush_Output:%lX\n", StreamClient_GetLastError());
		return -1;
	}

	bool bIsPush = true;
	while (bIsPush)
	{
		StreamClient_FilePush_GetStatus(xhStream, &bIsPush);
	}
	StreamClient_FilePush_Close(xhStream);
	return 1;
}

int Test_LivePush()
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
	XBYTE tszSPSBuffer[MAX_PATH];
	XBYTE tszPPSBuffer[MAX_PATH];
	XENGINE_PROTOCOL_AVINFO st_MediaStream;

	memset(tszVBuffer, '\0', sizeof(tszVBuffer));
	memset(tszABuffer, '\0', sizeof(tszABuffer));
	memset(tszSPSBuffer, '\0', MAX_PATH);
	memset(tszPPSBuffer, '\0', MAX_PATH);
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
	st_MediaStream.st_AudioInfo.nSampleFmt = ENUM_AVCOLLECT_AUDIO_SAMPLE_FMT_FLTP;
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
	AVHelp_Parse_FrameInit(&xhAParse, ENUM_XENGINE_AVCODEC_AUDIO_TYPE_AAC);
	AVHelp_Parse_FrameInit(&xhVParse, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264);

	xhStream = StreamClient_CodecPush_Init(lpszUrl, &st_MediaStream);
	StreamClient_CodecPush_WriteHdr(xhStream);

	while (true)
	{
		if (st_MediaStream.st_VideoInfo.bEnable)
		{
			int nListCount = 0;
			AVHELP_FRAMEDATA** ppSt_Frame;
			AVHelp_Parse_FrameGet(xhVParse, tszVBuffer, nVLen, &ppSt_Frame, &nListCount);
			for (int i = 0; i < nListCount; i++)
			{
				StreamClient_CodecPush_PushVideo(xhStream, (LPCXSTR)ppSt_Frame[i]->ptszMsgBuffer, ppSt_Frame[i]->nMsgLen);
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
			AVHELP_FRAMEDATA** ppSt_Frame;
			AVHelp_Parse_FrameGet(xhAParse, tszABuffer, nALen, &ppSt_Frame, &nListCount);
			for (int i = 0; i < nListCount; i++)
			{
				//不需要AAC头
				StreamClient_CodecPush_PushAudio(xhStream, (LPCXSTR)ppSt_Frame[i]->ptszMsgBuffer + 7, ppSt_Frame[i]->nMsgLen - 7);
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
	AVHelp_Parse_FrameClose(xhVParse);
	AVHelp_Parse_FrameClose(xhAParse);
	StreamClient_CodecPush_Close(xhStream);
	return 0;
}

int Test_RTMPPull()
{
	//LPCXSTR lpszPullUrl = _X("rtmp://10.0.3.155/live/qyt");
	LPCXSTR lpszPullUrl = _X("srt://10.0.3.155:10080?streamid=#!::r=live/livestream,m=request");
	LPCXSTR lpszPushUrl = _X("rtmp://app.xyry.org/live/123");

	int nStreamCount = 0;
	STREAMMEDIA_PULLSTREAM** ppSt_PullStream;

	//pSt_VFile = fopen("D:\\windows-ffmpeg\\x64\\3.ts", "wb");
	XHANDLE xhStream = StreamClient_StreamPull_Init(lpszPullUrl, &ppSt_PullStream, &nStreamCount, CBStream_Pull);
	if (NULL == xhStream)
	{
		printf("XStream_FilePush_Push:%lX\n", StreamClient_GetLastError());
		return -1;
	}

	for (int i = 0; i < nStreamCount; i++)
	{
		if (ppSt_PullStream[i]->enStreamType == ENUM_STREAMMEIDA_STREAMCLIENT_STREAM_TYPE_VIDEO)
		{
			ppSt_PullStream[i]->bEnable = true;
		}
		else if (ppSt_PullStream[i]->enStreamType == ENUM_STREAMMEIDA_STREAMCLIENT_STREAM_TYPE_AUDIO)
		{
			ppSt_PullStream[i]->bEnable = true;
		}
	}
	/*
	if (!StreamClient_StreamPull_PushStream(xhStream, lpszPushUrl, &ppSt_PullStream, nStreamCount))
	{
		printf("XStream_StreamPull_PushStream:%lX\n", StreamClient_GetLastError());
		return -1;
	}*/
	XENGINE_PROTOCOL_AVINFO st_MediaStream;
	memset(&st_MediaStream, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));

	StreamClient_StreamPull_Start(xhStream);
	bool bPull = true;

	while (bPull)
	{
		StreamClient_StreamPull_GetStatus(xhStream, &bPull);
	}
	StreamClient_StreamPull_Close(xhStream);
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