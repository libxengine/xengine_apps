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
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVFormat_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVFormat_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVFormat.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVHelp.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_VideoCodec/VideoCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFormat/AVFormat_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFormat/AVFormat_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Error.h"
#ifdef _MSC_BUILD
#ifdef _WIN64
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_AVFormat.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_AVHelp.lib")
#else
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVFormat.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVHelp.lib")
#endif
#endif
#endif

//Linux::g++ -std=c++20 -Wall -g AVCodec_APPFormat.cpp -o AVCodec_APPFormat.exe -lXEngine_BaseLib -lXEngine_AVFormat -lXEngine_AVHelp
FILE* pSt_File;
int AVFormat_Packet_RW(XPVOID lParam, uint8_t* puszMsgBuffer, int nSize)
{
	static int nCount = 0;
	int nRet = fread(puszMsgBuffer, 1, 256, pSt_File);

	printf("%d\n", nCount += nRet);
	return nRet;
}

int AVPacket_Test_FileLink()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszSrcFile1 = "D:\\h264 file\\480p.264";
	LPCXSTR lpszSrcFile2 = "D:\\h264 file\\480p.264";
	LPCXSTR lpszDstFile = "D:\\h264 file\\out.mp4";
#else
	LPCXSTR lpszSrcFile1 = "480p.264";
	LPCXSTR lpszSrcFile2 = "480p.264";
	LPCXSTR lpszDstFile = "out.mp4";
#endif

	XHANDLE xhPacket = AVFormat_Packet_Init();
	if (NULL == xhPacket)
	{
		printf("AVFormat_Packet_Init:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	if (!AVFormat_Packet_Output(xhPacket, lpszDstFile))
	{
		printf("AVFormat_Packet_Output:%lX\n", AVFormat_GetLastError());
		return -1;
	}

	XHANDLE xhVideo1 = AVFormat_UNPack_Init();
	if (NULL == xhVideo1)
	{
		printf("AVFormat_UNPack_Init:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	if (!AVFormat_UNPack_Input(xhVideo1, lpszSrcFile1))
	{
		printf("AVFormat_UNPack_Init:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	XHANDLE xhVideo2 = AVFormat_UNPack_Init();
	if (NULL == xhVideo2)
	{
		printf("AVFormat_UNPack_Init:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	if (!AVFormat_UNPack_Input(xhVideo2, lpszSrcFile2))
	{
		printf("AVFormat_UNPack_Init:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	//get video stream and create out put stream
	int nAVStream = 0;
	AVCODEC_STREAMINFO** ppSt_AVStream = {};
	AVFormat_UNPack_GetStream(xhVideo1, &ppSt_AVStream, &nAVStream);
	for (int i = 0; i < nAVStream; i++)
	{
		XHANDLE xhAVParameter = NULL;
		AVCODEC_TIMEBASE st_TimeBase = {};
		AVFormat_UNPack_GetTime(xhVideo1, ppSt_AVStream[i]->nAVIndex, NULL, &st_TimeBase);
		AVFormat_UNPack_GetAVCodec(xhVideo1, ppSt_AVStream[i]->nAVIndex, &xhAVParameter);

		AVFormat_Packet_StreamCreate(xhPacket, xhAVParameter);
		AVFormat_Packet_TimeBase(xhPacket, 0, &st_TimeBase);
	}
	BaseLib_Memory_Free((XPPPMEM)&ppSt_AVStream, nAVStream);
	
	if (!AVFormat_Packet_Start(xhPacket))
	{
		printf("AVFormat_UNPack_Start:%lX\n", AVFormat_GetLastError());
		return -1;
	}

	XBYTE* ptszMSGBuffer = (XBYTE*)malloc(XENGINE_MEMORY_SIZE_MID);
	if (NULL == ptszMSGBuffer)
	{
		return -1;
	}
	while (true)
	{
		int nAVIndex = 0;
		int nMSGLen = 0;
		AVCODEC_PACKETINFO st_AVPacket = {};
		memset(ptszMSGBuffer, 0, XENGINE_MEMORY_SIZE_MID);
		if (!AVFormat_UNPack_Read(xhVideo1, &nAVIndex, ptszMSGBuffer, &nMSGLen, &st_AVPacket))
		{
			break;
		}
		AVFormat_Packet_StreamWrite(xhPacket, 0, ptszMSGBuffer, nMSGLen);
	}
	//set last pts
	bool bChanged = true;
	AVFormat_Packet_SetLastPTS(xhPacket);
	while (true)
	{
		int nAVIndex = 0;
		int nMSGLen = 0;
		AVCODEC_PACKETINFO st_AVPacket = {};
		memset(ptszMSGBuffer, 0, XENGINE_MEMORY_SIZE_MID);
		if (!AVFormat_UNPack_Read(xhVideo2, &nAVIndex, ptszMSGBuffer, &nMSGLen, &st_AVPacket))
		{
			break;
		}
		AVFormat_Packet_StreamWrite(xhPacket, 0, ptszMSGBuffer, nMSGLen, NULL, NULL, false, bChanged);
		bChanged = false;
	}
	AVFormat_UNPack_Stop(xhVideo1);
	AVFormat_UNPack_Stop(xhVideo2);
	AVFormat_Packet_Stop(xhPacket);
	return 0;
}

int AVPacket_Test_FilePacket()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszVideoFile = "D:\\h264 file\\480p.264";
	LPCXSTR lpszAudioFile1 = "D:\\h264 file\\1.aac";
	LPCXSTR lpszAudioFile2 = "D:\\h264 file\\test.aac";
	LPCXSTR lpszDstFile = "D:\\h264 file\\480p.mp4";
#else
	LPCXSTR lpszVideoFile = "480p.264";
	LPCXSTR lpszAudioFile1 = "1.aac";
	LPCXSTR lpszAudioFile2 = "test.aac";
	LPCXSTR lpszDstFile = "480p.mp4";
#endif

	XHANDLE xhPacket = AVFormat_Packet_Init();
	if (NULL == xhPacket)
	{
		printf("AVFormat_Packet_Init:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	if (!AVFormat_Packet_Output(xhPacket, lpszDstFile))
	{
		printf("AVFormat_Packet_Output:%lX\n", AVFormat_GetLastError());
		return -1;
	}

	XHANDLE xhVideo = AVFormat_UNPack_Init();
	if (NULL == xhVideo)
	{
		printf("AVFormat_UNPack_Init:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	if (!AVFormat_UNPack_Input(xhVideo, lpszVideoFile))
	{
		printf("AVFormat_UNPack_Init:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	XHANDLE xhAudio1 = AVFormat_UNPack_Init();
	if (NULL == xhVideo)
	{
		printf("AVFormat_UNPack_Init:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	if (!AVFormat_UNPack_Input(xhAudio1, lpszAudioFile1))
	{
		printf("AVFormat_UNPack_Init:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	XHANDLE xhAudio2 = AVFormat_UNPack_Init();
	if (NULL == xhVideo)
	{
		printf("AVFormat_UNPack_Init:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	if (!AVFormat_UNPack_Input(xhAudio2, lpszAudioFile2))
	{
		printf("AVFormat_UNPack_Init:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	//get video stream and create out put stream
	int nAVStream = 0;
	AVCODEC_STREAMINFO** ppSt_AVStream = {};
	AVFormat_UNPack_GetStream(xhVideo, &ppSt_AVStream, &nAVStream);
	for (int i = 0; i < nAVStream; i++)
	{
		XHANDLE xhAVParameter = NULL;
		AVCODEC_TIMEBASE st_TimeBase = {};
		AVFormat_UNPack_GetTime(xhVideo, ppSt_AVStream[i]->nAVIndex, NULL, &st_TimeBase);
		AVFormat_UNPack_GetAVCodec(xhVideo, ppSt_AVStream[i]->nAVIndex, &xhAVParameter);

		AVFormat_Packet_StreamCreate(xhPacket, xhAVParameter);
		AVFormat_Packet_TimeBase(xhPacket, 0, &st_TimeBase);
	}
	BaseLib_Memory_Free((XPPPMEM)&ppSt_AVStream, nAVStream);
	//get audio 1 stream and create out put stream
	nAVStream = 0;
	AVFormat_UNPack_GetStream(xhAudio1, &ppSt_AVStream, &nAVStream);
	for (int i = 0; i < nAVStream; i++)
	{
		XHANDLE xhAVParameter = NULL;
		AVCODEC_TIMEBASE st_TimeBase = {};
		AVFormat_UNPack_GetTime(xhAudio1, ppSt_AVStream[i]->nAVIndex, NULL, &st_TimeBase);
		AVFormat_UNPack_GetAVCodec(xhAudio1, ppSt_AVStream[i]->nAVIndex, &xhAVParameter);

		AVFormat_Packet_StreamCreate(xhPacket, xhAVParameter);
		AVFormat_Packet_TimeBase(xhPacket, 1, &st_TimeBase);
	}
	BaseLib_Memory_Free((XPPPMEM)&ppSt_AVStream, nAVStream);
	//get audio 2 stream and create out put stream
	nAVStream = 0;
	AVFormat_UNPack_GetStream(xhAudio2, &ppSt_AVStream, &nAVStream);
	for (int i = 0; i < nAVStream; i++)
	{
		XHANDLE xhAVParameter = NULL;
		AVCODEC_TIMEBASE st_TimeBase = {};
		AVFormat_UNPack_GetTime(xhAudio2, ppSt_AVStream[i]->nAVIndex, NULL, &st_TimeBase);
		AVFormat_UNPack_GetAVCodec(xhAudio2, ppSt_AVStream[i]->nAVIndex, &xhAVParameter);

		AVFormat_Packet_StreamCreate(xhPacket, xhAVParameter);
		AVFormat_Packet_TimeBase(xhPacket, 2, &st_TimeBase);
	}
	BaseLib_Memory_Free((XPPPMEM)&ppSt_AVStream, nAVStream);

	if (!AVFormat_Packet_Start(xhPacket))
	{
		printf("AVFormat_UNPack_Start:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	
	XBYTE* ptszMSGBuffer = (XBYTE *)malloc(XENGINE_MEMORY_SIZE_MID);
	if (NULL == ptszMSGBuffer)
	{
		return -1;
	}
	while (true)
	{
		int nAVIndex = 0;
		int nMSGLen = 0;
		AVCODEC_PACKETINFO st_AVPacket = {};
		memset(ptszMSGBuffer, 0, XENGINE_MEMORY_SIZE_MID);
		if (!AVFormat_UNPack_Read(xhVideo, &nAVIndex, ptszMSGBuffer, &nMSGLen, &st_AVPacket))
		{
			break;
		}
		AVFormat_Packet_StreamWrite(xhPacket, 0, ptszMSGBuffer, nMSGLen);
	}
	while (true)
	{
		int nAVIndex = 0;
		int nMSGLen = 0;
		AVCODEC_PACKETINFO st_AVPacket = {};
		memset(ptszMSGBuffer, 0, XENGINE_MEMORY_SIZE_MID);
		if (!AVFormat_UNPack_Read(xhAudio1, &nAVIndex, ptszMSGBuffer, &nMSGLen, &st_AVPacket))
		{
			break;
		}
		AVFormat_Packet_StreamWrite(xhPacket, 1, ptszMSGBuffer, nMSGLen);
	}
	while (true)
	{
		int nAVIndex = 0;
		int nMSGLen = 0;
		AVCODEC_PACKETINFO st_AVPacket = {};
		memset(ptszMSGBuffer, 0, XENGINE_MEMORY_SIZE_MID);
		if (!AVFormat_UNPack_Read(xhAudio2, &nAVIndex, ptszMSGBuffer, &nMSGLen, &st_AVPacket))
		{
			break;
		}
		AVFormat_Packet_StreamWrite(xhPacket, 2, ptszMSGBuffer, nMSGLen);
	}
	AVFormat_UNPack_Stop(xhVideo);
	AVFormat_UNPack_Stop(xhAudio1);
	AVFormat_UNPack_Stop(xhAudio2);
	AVFormat_Packet_Stop(xhPacket);
	return 0;
}

int Test_RTMPPull()
{
	LPCXSTR lpszPullUrl = _X("rtmp://app.xyry.org/live/qyt");
	//LPCXSTR lpszPullUrl = _X("srt://10.0.3.155:10080?streamid=#!::r=live/livestream,m=request");
	LPCXSTR lpszPushUrl = _X("rtmp://app.xyry.org/live/123");

	XHANDLE xhUNPack = AVFormat_UNPack_Init();
	XHANDLE xhPacket = AVFormat_Packet_Init();
	if (NULL == xhUNPack)
	{
		return -1;
	}
	if (!AVFormat_UNPack_Input(xhUNPack, lpszPullUrl, 5))
	{
		return -1;
	}
	XHANDLE pSt_VideoParameter = NULL;
	XHANDLE pSt_AudioParameter = NULL;
	AVCODEC_TIMEBASE st_VideoTime = {};
	AVCODEC_TIMEBASE st_AudioTime = {};

	int nStreamCount = 0;
	AVCODEC_STREAMINFO** ppSt_PullStream;
	AVFormat_UNPack_GetStream(xhUNPack, &ppSt_PullStream, &nStreamCount);
	for (int i = 0; i < nStreamCount; i++)
	{
		if (ppSt_PullStream[i]->nAVCodecType == ENUM_AVFORMAT_STREAM_MEDIA_TYPE_VIDEO)
		{
			AVFormat_UNPack_GetAVCodec(xhUNPack, i, &pSt_VideoParameter);
			AVFormat_UNPack_GetTime(xhUNPack, i, NULL, &st_VideoTime);
		}
		else if (ppSt_PullStream[i]->nAVCodecType == ENUM_AVFORMAT_STREAM_MEDIA_TYPE_AUDIO)
		{
			AVFormat_UNPack_GetAVCodec(xhUNPack, i, &pSt_AudioParameter);
			AVFormat_UNPack_GetTime(xhUNPack, i, NULL, &st_AudioTime);
		}
		else
		{
			printf("Unknown Stream:%d\n", i);
		}
	}

	if (!AVFormat_Packet_Output(xhPacket, lpszPushUrl, _X("flv")))
	{
		printf("AVFormat_Packet_Output:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	AVFormat_Packet_StreamCreate(xhPacket, pSt_VideoParameter);
	AVFormat_Packet_StreamCreate(xhPacket, pSt_AudioParameter);
	AVFormat_Packet_TimeBase(xhPacket, 0, &st_VideoTime);
	AVFormat_Packet_TimeBase(xhPacket, 1, &st_AudioTime);
	AVFormat_Packet_Start(xhPacket);

	XBYTE* ptszMSGBuffer = (XBYTE*)malloc(XENGINE_MEMORY_SIZE_MID);
	if (NULL == ptszMSGBuffer)
	{
		return -1;
	}
	while (true)
	{
		int nMSGLen = 0;
		int nAVIndex = -1;
		memset(ptszMSGBuffer, 0, XENGINE_MEMORY_SIZE_MID);
		AVCODEC_PACKETINFO st_AVPacket = {};
		if (!AVFormat_UNPack_Read(xhUNPack, &nAVIndex, ptszMSGBuffer, &nMSGLen, &st_AVPacket))
		{
			break;
		}
		AVFormat_Packet_StreamWrite(xhPacket, nAVIndex, ptszMSGBuffer, nMSGLen, &st_AVPacket);
	}
	AVFormat_UNPack_Stop(xhUNPack);
	AVFormat_Packet_Stop(xhPacket);
	return 1;
}

int main()
{
	AVPacket_Test_FilePacket();
	AVPacket_Test_FileLink();
	Test_RTMPPull();
	return 1;
}