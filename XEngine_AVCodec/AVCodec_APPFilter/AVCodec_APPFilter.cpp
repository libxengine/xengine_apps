#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#if 1 == _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AudioCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVFilter_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVFilter_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVFilter.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVHelp.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_VideoCodec/VideoCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AudioCodec/AudioCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFilter/AVFilter_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFilter/AVFilter_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Error.h"
#ifdef _MSC_BUILD
#ifdef _WIN64
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_AVFilter.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_AVHelp.lib")
#else
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVFilter.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVHelp.lib")
#endif
#endif
#endif

//Linux MacOS:g++ -std=c++20 -Wall -g AVCodec_APPFilter.cpp -o AVCodec_APPFilter.exe -lXEngine_BaseLib -lXEngine_AVFilter -lXEngine_AVHelp

int Test_FilterAudio()
{
	AVFILTER_AUDIO_INFO st_AudioFilter = {};

	st_AudioFilter.st_AudioInfo.nChannel = 1;
	st_AudioFilter.st_AudioInfo.nNBSample = 1024;
	st_AudioFilter.st_AudioInfo.nSampleFmt = 1;
	st_AudioFilter.st_AudioInfo.nSampleRate = 11025;

	st_AudioFilter.st_AudioTime.nNum = 1;
	st_AudioFilter.st_AudioTime.nDen = 11025;

	//XHANDLE xhToken = AVFilter_Audio_Init(_X("volume=2.0"), &st_AudioFilter);
	XHANDLE xhToken = AVFilter_Audio_Init(_X("aresample=44100,aformat=sample_fmts=s16:channel_layouts=stereo"), &st_AudioFilter);

#ifdef _MSC_BUILD
	LPCXSTR lpszRFile = _X("D:\\audio\\3.pcm");
	LPCXSTR lpszWFile = _X("D:\\audio\\3_out.pcm");
#else
	LPCXSTR lpszRFile = _X("3.pcm");
	LPCXSTR lpszWFile = _X("3_out.pcm");
#endif

	FILE* pSt_RFile = _xtfopen(lpszRFile, _X("rb"));
	FILE* pSt_WFile = _xtfopen(lpszWFile, _X("wb"));

	while (true)
	{
		int nRSize = 2048;  //每帧数据
		int nWSize = 10240;
		XCHAR tszWBBuffer[10240] = {};
		XCHAR tszRDBuffer[8192] = {};
		
		int nRet = fread(tszRDBuffer, 1, nRSize, pSt_RFile);
		if (nRet <= 0)
		{
			break;
		}
		//填充内存
		XENGINE_MSGBUFFER st_MSGBuffer = {};
		AVCODEC_TIMESTAMP st_TimeStamp = {};

		st_MSGBuffer.nMSGLen[0] = nRet;
		BaseLib_Memory_MSGMalloc(&st_MSGBuffer);
		memcpy((XPVOID)st_MSGBuffer.unData.ptszMSGArray[0], tszRDBuffer, nRet);

		XHANDLE** ppSt_AVBuffer = NULL;
		AVHelp_Memory_SetAudioBuffer(&st_MSGBuffer, &st_TimeStamp, &st_AudioFilter.st_AudioInfo, &ppSt_AVBuffer, false);
		//处理
		int nListCount = 0;
		XHANDLE** ppSt_MSGBuffer;
		AVFilter_Audio_Cvt(xhToken, ppSt_AVBuffer[0], &ppSt_MSGBuffer, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			XENGINE_MSGBUFFER st_MSGBuffer = {};
			AVHelp_Memory_GetAudioBuffer(ppSt_MSGBuffer[i], &st_MSGBuffer, false);

			printf("Test_FilterAudio len:%d\n", st_MSGBuffer.nMSGLen[0]);
			fwrite(st_MSGBuffer.unData.ptszMSGArray[0], 1, st_MSGBuffer.nMSGLen[0], pSt_WFile);
			BaseLib_Memory_MSGFree(&st_MSGBuffer);
		}
		AVHelp_Memory_FreeAVList(&ppSt_MSGBuffer, nListCount, false);
	}
	AVFilter_Audio_Destroy(xhToken);
	fclose(pSt_RFile);
	fclose(pSt_WFile);
	return 0;
}

int Test_FilterVideo()
{
	AVFILTER_VIDEO_INFO st_VideoFilter = {};

	st_VideoFilter.st_VideoInfo.nWidth = 720;
	st_VideoFilter.st_VideoInfo.nHeight = 480;
	st_VideoFilter.st_VideoInfo.nFormat = ENUM_AVCODEC_VIDEO_SAMPLEFMT_YUV420P;

	st_VideoFilter.st_VideoTime.nDen = 24;
	st_VideoFilter.st_VideoTime.nNum = 1;

	XHANDLE xhToken = AVFilter_Video_Init(_X("eq=brightness=1.5:contrast=1.2"), &st_VideoFilter);

#ifdef _MSC_BUILD
	FILE* pSt_RBFile = _xtfopen("D:\\h264 file\\480p.yuv", _X("rb"));
	FILE* pSt_WBFile = _xtfopen("D:\\h264 file\\480p_output.yuv", _X("wb"));
#else
	FILE* pSt_RBFile = _xtfopen("480p.yuv", _X("rb"));
	FILE* pSt_WBFile = _xtfopen("480p_output.yuv", _X("wb"));
#endif

	int nSize = st_VideoFilter.st_VideoInfo.nWidth * st_VideoFilter.st_VideoInfo.nHeight * 3 / 2;
	XCHAR* ptszRBBuffer = (XCHAR*)malloc(nSize);
	int nCount = 0;

	while (true)
	{
		int nRet = fread(ptszRBBuffer, 1, nSize, pSt_RBFile);
		if (nRet <= 0)
		{
			break;
		}
		//填充内存
		XENGINE_MSGBUFFER st_MSGBuffer = {};
		AVCODEC_TIMESTAMP st_TimeStamp = {};

		st_MSGBuffer.nMSGLen[0] = nRet;
		BaseLib_Memory_MSGMalloc(&st_MSGBuffer);
		memcpy((XPVOID)st_MSGBuffer.unData.ptszMSGArray[0], ptszRBBuffer, nRet);

		XHANDLE** ppSt_AVBuffer = NULL;
		AVHelp_Memory_SetVideoBuffer(&st_MSGBuffer, &st_TimeStamp, &st_VideoFilter.st_VideoInfo, &ppSt_AVBuffer, false);

		int nListCount = 0;
		XHANDLE** ppSt_MSGBuffer;
		AVFilter_Video_Cvt(xhToken, ppSt_AVBuffer[0], &ppSt_MSGBuffer, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			XENGINE_MSGBUFFER st_MSGBuffer = {};
			AVHelp_Memory_GetVideoBuffer(ppSt_MSGBuffer[i], &st_MSGBuffer, false);

			printf("Test_FilterVideo len:%d\n", st_MSGBuffer.nMSGLen[0]);
			fwrite(st_MSGBuffer.unData.ptszMSGBuffer, 1, st_MSGBuffer.nMSGLen[0], pSt_WBFile);
			BaseLib_Memory_MSGFree(&st_MSGBuffer);
		}
		AVHelp_Memory_FreeAVList(&ppSt_MSGBuffer, nListCount, false);
	}

	AVFilter_Video_Destroy(xhToken);
	fclose(pSt_RBFile);
	fclose(pSt_WBFile);

	free(ptszRBBuffer);
	return 0;
}

int Test_FilterMutliVideo()
{
	int nVideoList = 4;
	AVFILTER_VIDEO_INFO **ppSt_VideoInfo;

	BaseLib_Memory_Malloc((XPPPMEM)&ppSt_VideoInfo, nVideoList, sizeof(AVFILTER_VIDEO_INFO));

	ppSt_VideoInfo[0]->nIndex = 0;
	ppSt_VideoInfo[0]->st_VideoInfo.nWidth = 720;
	ppSt_VideoInfo[0]->st_VideoInfo.nHeight = 480;
	ppSt_VideoInfo[0]->st_VideoInfo.nFormat = ENUM_AVCODEC_VIDEO_SAMPLEFMT_YUV420P;
	ppSt_VideoInfo[0]->st_VideoTime.nDen = 1;
	ppSt_VideoInfo[0]->st_VideoTime.nNum = 24;
	_tcsxcpy(ppSt_VideoInfo[0]->tszFilterName, "in0");

	ppSt_VideoInfo[1]->nIndex = 1;
	ppSt_VideoInfo[1]->st_VideoInfo.nWidth = 720;
	ppSt_VideoInfo[1]->st_VideoInfo.nHeight = 480;
	ppSt_VideoInfo[1]->st_VideoInfo.nFormat = ENUM_AVCODEC_VIDEO_SAMPLEFMT_YUV420P;
	ppSt_VideoInfo[1]->st_VideoTime.nDen = 1;
	ppSt_VideoInfo[1]->st_VideoTime.nNum = 24;
	_tcsxcpy(ppSt_VideoInfo[1]->tszFilterName, "in1");

	ppSt_VideoInfo[2]->nIndex = 2;
	ppSt_VideoInfo[2]->st_VideoInfo.nWidth = 720;
	ppSt_VideoInfo[2]->st_VideoInfo.nHeight = 480;
	ppSt_VideoInfo[2]->st_VideoInfo.nFormat = ENUM_AVCODEC_VIDEO_SAMPLEFMT_YUV420P;
	ppSt_VideoInfo[2]->st_VideoTime.nDen = 1;
	ppSt_VideoInfo[2]->st_VideoTime.nNum = 24;
	_tcsxcpy(ppSt_VideoInfo[2]->tszFilterName, "in2");

	ppSt_VideoInfo[3]->nIndex = 3;
	ppSt_VideoInfo[3]->st_VideoInfo.nWidth = 720;
	ppSt_VideoInfo[3]->st_VideoInfo.nHeight = 480;
	ppSt_VideoInfo[3]->st_VideoInfo.nFormat = ENUM_AVCODEC_VIDEO_SAMPLEFMT_YUV420P;
	ppSt_VideoInfo[3]->st_VideoTime.nDen = 1;
	ppSt_VideoInfo[3]->st_VideoTime.nNum = 24;
	_tcsxcpy(ppSt_VideoInfo[3]->tszFilterName, "in3");
	
	XHANDLE xhToken = AVFilter_Video_MIXInit(&ppSt_VideoInfo, nVideoList, _X("out"), _X("[in0]scale=360:240[in0_scaled];[in1]scale=360:240[in1_scaled];[in2]scale=360:240[in2_scaled];[in3]scale=360:240[in3_scaled];[in0_scaled][in1_scaled][in2_scaled][in3_scaled]xstack=inputs=4:layout=0_0|w0_0|0_h0|w0_h0[out]"));

#ifdef _MSC_BUILD
	FILE* pSt_RBFile = _xtfopen("D:\\h264 file\\480p.yuv", _X("rb"));
	FILE* pSt_WBFile = _xtfopen("D:\\h264 file\\480p_mix.yuv", _X("wb"));
#else
	FILE* pSt_RBFile = _xtfopen("480p.yuv", _X("rb"));
	FILE* pSt_WBFile = _xtfopen("480p_mix.yuv", _X("wb"));
#endif

	int nSize = 720 * 480 * 3 / 2;
	XCHAR* ptszRBBuffer = (XCHAR*)malloc(nSize);

	while (true)
	{
		int nRet = fread(ptszRBBuffer, 1, nSize, pSt_RBFile);
		if (nRet <= 0)
		{
			break;
		}
		for (int i = 0; i < 4; i++)
		{
			//填充内存
			XENGINE_MSGBUFFER st_MSGBuffer = {};
			AVCODEC_TIMESTAMP st_TimeStamp = {};

			st_MSGBuffer.nMSGLen[0] = nRet;
			BaseLib_Memory_MSGMalloc(&st_MSGBuffer);
			memcpy((XPVOID)st_MSGBuffer.unData.ptszMSGArray[0], ptszRBBuffer, nRet);

			XHANDLE** ppSt_AVBuffer = NULL;
			AVHelp_Memory_SetVideoBuffer(&st_MSGBuffer, &st_TimeStamp, &ppSt_VideoInfo[i]->st_VideoInfo, &ppSt_AVBuffer, false);
			AVFilter_Video_MIXSend(xhToken, i, ppSt_AVBuffer[0]);
			BaseLib_Memory_MSGFree(&st_MSGBuffer);
			AVHelp_Memory_FreeAVList(&ppSt_AVBuffer, 1, false);
		}

		int nListCount = 0;
		XHANDLE** ppSt_MSGBuffer;
		bool bRet = AVFilter_Video_MIXRecv(xhToken, &ppSt_MSGBuffer, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			XENGINE_MSGBUFFER st_MSGBuffer = {};
			AVHelp_Memory_GetVideoBuffer(ppSt_MSGBuffer[i], &st_MSGBuffer, false);

			printf("Test_FilterMutliVideo len:%d\n", st_MSGBuffer.nMSGLen[0]);
			fwrite(st_MSGBuffer.unData.ptszMSGBuffer, 1, st_MSGBuffer.nMSGLen[0], pSt_WBFile);
			BaseLib_Memory_MSGFree(&st_MSGBuffer);
		}
		AVHelp_Memory_FreeAVList(&ppSt_MSGBuffer, nListCount, false);
	}

	AVFilter_Video_MIXDestroy(xhToken);
	fclose(pSt_RBFile);
	fclose(pSt_WBFile);

	free(ptszRBBuffer);
	return 0;
}

void Test_FilterMutliAudio()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszAudioFile1 = "d:\\audio\\3_out.pcm";
	LPCXSTR lpszAudioFile2 = "d:\\audio\\4.pcm";
	LPCXSTR lpszDstFile = "d:\\audio\\mix.pcm";
#else
	LPCXSTR lpszAudioFile1 = "3.pcm";
	LPCXSTR lpszAudioFile2 = "4.pcm";
	LPCXSTR lpszDstFile = "mix.pcm";
#endif

	AVFILTER_AUDIO_INFO** ppSt_AudioFile;
	BaseLib_Memory_Malloc((XPPPMEM)&ppSt_AudioFile, 2, sizeof(AVFILTER_AUDIO_INFO));

	ppSt_AudioFile[0]->st_AudioInfo.nSampleFmt = ENUM_AVCODEC_AUDIO_SAMPLEFMT_S16;
	ppSt_AudioFile[0]->st_AudioInfo.nSampleRate = 44100;
	ppSt_AudioFile[0]->st_AudioInfo.nNBSample = 1024;
	ppSt_AudioFile[0]->st_AudioInfo.nChannel = 2;
	ppSt_AudioFile[0]->nIndex = 0;
	ppSt_AudioFile[0]->st_AudioTime.nNum = 1;
	ppSt_AudioFile[0]->st_AudioTime.nDen = 44100;
	_tcsxcpy(ppSt_AudioFile[0]->tszFilterName, "in1");

	ppSt_AudioFile[1]->st_AudioInfo.nSampleFmt = ENUM_AVCODEC_AUDIO_SAMPLEFMT_S16;
	ppSt_AudioFile[1]->st_AudioInfo.nSampleRate = 44100;
	ppSt_AudioFile[1]->st_AudioInfo.nNBSample = 1024;
	ppSt_AudioFile[1]->st_AudioInfo.nChannel = 2;
	ppSt_AudioFile[1]->nIndex = 1;
	ppSt_AudioFile[1]->st_AudioTime.nNum = 1;
	ppSt_AudioFile[1]->st_AudioTime.nDen = 44100;
	_tcsxcpy(ppSt_AudioFile[1]->tszFilterName, "in2");

	XHANDLE xhFilter = AVFilter_Audio_MIXInit(&ppSt_AudioFile, 2, "out", "[in1][in2]amix=inputs=2:duration=longest,aformat=sample_fmts=s16:sample_rates=44100:channel_layouts=stereo[out]");
	if (NULL == xhFilter)
	{
		printf("AudioCodec_Help_FilterInit\n");
		return;
	}
	FILE* pSt_File1 = fopen(lpszAudioFile1, _X("rb"));
	FILE* pSt_File2 = fopen(lpszAudioFile2, _X("rb"));
	FILE* pSt_FileAac = fopen(lpszDstFile, _X("wb"));
	while (1)
	{
		XCHAR tszEnBuffer1[4096];
		XCHAR tszEnBuffer2[4096];
		XCHAR tszFilterBuffer[10240];

		memset(tszEnBuffer1, '\0', sizeof(tszEnBuffer1));
		memset(tszEnBuffer2, '\0', sizeof(tszEnBuffer2));
		memset(tszFilterBuffer, '\0', sizeof(tszFilterBuffer));

		int nRet1 = fread(tszEnBuffer1, 1, sizeof(tszEnBuffer1), pSt_File1);
		if (nRet1 <= 0)
		{
			break;
		}
		int nRet2 = fread(tszEnBuffer2, 1, sizeof(tszEnBuffer2), pSt_File2);
		if (nRet2 <= 0)
		{
			break;
		}
		//填充内存
		XENGINE_MSGBUFFER st_MSGBuffer1 = {};
		AVCODEC_TIMESTAMP st_TimeStamp = {};

		st_MSGBuffer1.nMSGLen[0] = nRet1;
		BaseLib_Memory_MSGMalloc(&st_MSGBuffer1);
		memcpy((XPVOID)st_MSGBuffer1.unData.ptszMSGArray[0], tszEnBuffer1, nRet1);

		XHANDLE** ppSt_AVBuffer1 = NULL;
		AVHelp_Memory_SetAudioBuffer(&st_MSGBuffer1, &st_TimeStamp, &ppSt_AudioFile[0]->st_AudioInfo, &ppSt_AVBuffer1, false);
		//
		XENGINE_MSGBUFFER st_MSGBuffer2 = {};

		st_MSGBuffer2.nMSGLen[0] = nRet2;
		BaseLib_Memory_MSGMalloc(&st_MSGBuffer2);
		memcpy((XPVOID)st_MSGBuffer2.unData.ptszMSGArray[0], tszEnBuffer2, nRet2);

		XHANDLE** ppSt_AVBuffer2 = NULL;
		AVHelp_Memory_SetAudioBuffer(&st_MSGBuffer2, &st_TimeStamp, &ppSt_AudioFile[1]->st_AudioInfo, &ppSt_AVBuffer2, false);

		if (!AVFilter_Audio_MIXSend(xhFilter, 0, ppSt_AVBuffer1[0]))
		{
			printf("errno\n");
		}
		if (!AVFilter_Audio_MIXSend(xhFilter, 1, ppSt_AVBuffer2[0]))
		{
			printf("errno\n");
		}
		int nListCount = 0;
		XHANDLE** ppSt_MSGBuffer;
		AVFilter_Audio_MIXRecv(xhFilter, &ppSt_MSGBuffer, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			XENGINE_MSGBUFFER st_MSGBuffer = {};
			AVHelp_Memory_GetAudioBuffer(ppSt_MSGBuffer[i], &st_MSGBuffer, false);

			printf("Test_FilterMutliAudio len:%d\n", st_MSGBuffer.nMSGLen[0]);
			fwrite(st_MSGBuffer.unData.ptszMSGArray[0], 1, st_MSGBuffer.nMSGLen[0], pSt_FileAac);
			BaseLib_Memory_MSGFree(&st_MSGBuffer);
		}
		BaseLib_Memory_Free((XPPPMEM)&ppSt_MSGBuffer, nListCount);
	}
	AVFilter_Audio_MIXDestroy(xhFilter);
	fclose(pSt_File1);
	fclose(pSt_File2);
	fclose(pSt_FileAac);
}

int main()
{
	Test_FilterAudio();
	Test_FilterVideo();

	Test_FilterMutliAudio();
	Test_FilterMutliVideo();
	return 0;
}