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
#include <XEngine_Include/XEngine_AVCodec/AVCollect_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVFilter_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVFilter_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVFilter.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVCollect/AVCollect_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFilter/AVFilter_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFilter/AVFilter_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVFilter.lib")
#endif
#endif

//Linux MacOS:g++ -std=c++17 -Wall -g AVCodec_APPFilter.cpp -o AVCodec_APPFilter.exe -lXEngine_BaseLib -lXEngine_AVFilter 

int Test_FilterAudio()
{
	XNETHANDLE xhToken = 0;
	AVFILTER_AUDIO_INFO st_AudioFilter = {};

	st_AudioFilter.nChannle = 2;
	st_AudioFilter.nNBSample = 1024;
	st_AudioFilter.nSampleFmt = 1;
	st_AudioFilter.nSampleRate = 44100;

	AVFilter_Audio_Init(&xhToken, _X("volume=2.0"), &st_AudioFilter);

	LPCXSTR lpszFile = _X("D:\\audio\\44.1k_2_16.pcm");
	FILE* pSt_File = _xtfopen(lpszFile, _X("rb"));

	while (true)
	{
		int nRSize = 8192;
		int nWSize = 8192;
		XCHAR tszWBBuffer[8192] = {};
		XCHAR tszRDBuffer[8192] = {};
		
		int nRet = fread(tszRDBuffer, 1, nRSize, pSt_File);
		AVFilter_Audio_Cvt(xhToken, (XBYTE*)tszRDBuffer, nRSize, (XBYTE*)tszWBBuffer, &nWSize);
	}

	AVFilter_Audio_Destroy(xhToken);
	return 0;
}

int Test_FilterVideo()
{
	XNETHANDLE xhToken = 0;
	AVFILTER_VIDEO_INFO st_VideoFilter = {};

	st_VideoFilter.nWidth = 720;
	st_VideoFilter.nHeight = 480;
	st_VideoFilter.nFrame = 24;
	st_VideoFilter.enAVPixForamt = ENUM_AVCOLLECT_VIDEO_FMT_YUV420P;

	AVFilter_Video_Init(&xhToken, _X("eq=brightness=1.5:contrast=1.2"), &st_VideoFilter);

	FILE* pSt_RBFile = _xtfopen("D:\\h264 file\\input.yuv", _X("rb"));
	FILE* pSt_WBFile = _xtfopen("D:\\h264 file\\output.yuv", _X("wb"));

	int nSize = st_VideoFilter.nWidth * st_VideoFilter.nHeight * 3 / 2;
	XCHAR* ptszWBBuffer = (XCHAR*)malloc(nSize);
	XCHAR* ptszRBBuffer = (XCHAR*)malloc(nSize);
	int nCount = 0;

	while (true)
	{
		int nRSize = nSize;
		int nWSize = nSize;

		int nRet = fread(ptszRBBuffer, 1, nSize, pSt_RBFile);
		if (nRet <= 0)
		{
			break;
		}
		AVFilter_Video_Cvt(xhToken, (XBYTE*)ptszRBBuffer, NULL, NULL, nRSize, 0, 0, (XBYTE *)ptszWBBuffer, &nWSize);
		if (nWSize > 0)
		{
			fwrite(ptszWBBuffer, 1, nWSize, pSt_WBFile);
			nCount += nWSize;
			printf("Count:%d\n", nCount);
		}
	}

	AVFilter_Video_Destroy(xhToken);
	fclose(pSt_RBFile);
	fclose(pSt_WBFile);

	free(ptszRBBuffer);
	free(ptszWBBuffer);
	return 0;
}
int Test_FilterMutliVideo()
{
	XNETHANDLE xhToken = 0;
	int nVideoList = 4;
	AVFILTER_VIDEO_INFO **ppSt_VideoInfo;

	BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_VideoInfo, nVideoList, sizeof(AVFILTER_VIDEO_INFO));

	ppSt_VideoInfo[0]->nIndex = 0;
	ppSt_VideoInfo[0]->nWidth = 720;
	ppSt_VideoInfo[0]->nHeight = 480;
	ppSt_VideoInfo[0]->nFrame = 24;
	ppSt_VideoInfo[0]->enAVPixForamt = ENUM_AVCOLLECT_VIDEO_FMT_YUV420P;
	_tcsxcpy(ppSt_VideoInfo[0]->tszFilterName, "in0");

	ppSt_VideoInfo[1]->nIndex = 1;
	ppSt_VideoInfo[1]->nWidth = 720;
	ppSt_VideoInfo[1]->nHeight = 480;
	ppSt_VideoInfo[1]->nFrame = 24;
	ppSt_VideoInfo[1]->enAVPixForamt = ENUM_AVCOLLECT_VIDEO_FMT_YUV420P;
	_tcsxcpy(ppSt_VideoInfo[1]->tszFilterName, "in1");

	ppSt_VideoInfo[2]->nIndex = 2;
	ppSt_VideoInfo[2]->nWidth = 720;
	ppSt_VideoInfo[2]->nHeight = 480;
	ppSt_VideoInfo[2]->nFrame = 24;
	ppSt_VideoInfo[2]->enAVPixForamt = ENUM_AVCOLLECT_VIDEO_FMT_YUV420P;
	_tcsxcpy(ppSt_VideoInfo[2]->tszFilterName, "in2");

	ppSt_VideoInfo[3]->nIndex = 3;
	ppSt_VideoInfo[3]->nWidth = 720;
	ppSt_VideoInfo[3]->nHeight = 480;
	ppSt_VideoInfo[3]->nFrame = 24;
	ppSt_VideoInfo[3]->enAVPixForamt = ENUM_AVCOLLECT_VIDEO_FMT_YUV420P;
	_tcsxcpy(ppSt_VideoInfo[3]->tszFilterName, "in3");
	
	AVFilter_Video_MIXInit(&xhToken, &ppSt_VideoInfo, nVideoList, _X("out"), _X("[in0]scale=360:240[in0_scaled]; [in1]scale=360:240[in1_scaled];[in2]scale=360:240[in2_scaled];[in3]scale=360:240[in3_scaled];[in0_scaled][in1_scaled][in2_scaled][in3_scaled]xstack=inputs=4:layout=0_0|w0_0|0_h0|w0_h0[out]"));

	FILE* pSt_RBFile = _xtfopen("D:\\h264 file\\input.yuv", _X("rb"));
	FILE* pSt_WBFile = _xtfopen("D:\\h264 file\\output.yuv", _X("wb"));

	int nSize = 720 * 480 * 3 / 2;
	XCHAR* ptszWBBuffer = (XCHAR*)malloc(nSize);
	XCHAR* ptszRBBuffer = (XCHAR*)malloc(nSize);
	int nCount = 0;

	while (true)
	{
		int nRSize = nSize;
		int nWSize = nSize;

		int nRet = fread(ptszRBBuffer, 1, nSize, pSt_RBFile);
		if (nRet <= 0)
		{
			break;
		}
		for (int i = 0; i < 4; i++)
		{
			AVFilter_Video_MIXSend(xhToken, i, (XBYTE*)ptszRBBuffer, NULL, NULL, nRSize, 0, 0);
		}
		bool bRet = AVFilter_Video_MIXRecv(xhToken, (XBYTE*)ptszWBBuffer, &nWSize);
		if (nWSize > 0 && bRet)
		{
			fwrite(ptszWBBuffer, 1, nWSize, pSt_WBFile);
			nCount += nWSize;
			printf("Count:%d\n", nCount);
		}
	}

	AVFilter_Video_MIXDestroy(xhToken);
	fclose(pSt_RBFile);
	fclose(pSt_WBFile);

	free(ptszRBBuffer);
	free(ptszWBBuffer);
	return 0;
}

void Test_FilterMutliAudio()
{
	XNETHANDLE xhFilter;

#ifdef _MSC_BUILD
	LPCXSTR lpszAudioFile1 = "d:\\audio\\3.pcm";
	LPCXSTR lpszAudioFile2 = "d:\\audio\\4.pcm";
	LPCXSTR lpszDstFile = "d:\\audio\\mix.pcm";
#else
	LPCXSTR lpszAudioFile1 = "3.pcm";
	LPCXSTR lpszAudioFile2 = "4.pcm";
	LPCXSTR lpszDstFile = "mix.pcm";
#endif

	AVFILTER_AUDIO_INFO** ppSt_AudioFile;
	BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_AudioFile, 2, sizeof(AVFILTER_AUDIO_INFO));

	ppSt_AudioFile[0]->nSampleFmt = ENUM_AVCOLLECT_AUDIO_SAMPLE_FMT_S16;
	ppSt_AudioFile[0]->nSampleRate = 44100;
	ppSt_AudioFile[0]->nNBSample = 1024;
	ppSt_AudioFile[0]->nChannle = 2;
	ppSt_AudioFile[0]->nIndex = 0;
	_tcsxcpy(ppSt_AudioFile[0]->tszFilterName, "in1");

	ppSt_AudioFile[1]->nSampleFmt = ENUM_AVCOLLECT_AUDIO_SAMPLE_FMT_S16;
	ppSt_AudioFile[1]->nSampleRate = 44100;
	ppSt_AudioFile[1]->nNBSample = 1024;
	ppSt_AudioFile[1]->nChannle = 2;
	ppSt_AudioFile[1]->nIndex = 1;
	_tcsxcpy(ppSt_AudioFile[1]->tszFilterName, "in2");

	if (!AVFilter_Audio_MIXInit(&xhFilter, &ppSt_AudioFile, 2, "out", "[in1][in2]amix=inputs=2:duration=longest,aformat=sample_fmts=s16:sample_rates=44100:channel_layouts=stereo[out]"))
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
		if (!AVFilter_Audio_MIXSend(xhFilter, 0, (uint8_t*)tszEnBuffer1, nRet1))
		{
			printf("errno\n");
		}
		if (!AVFilter_Audio_MIXSend(xhFilter, 1, (uint8_t*)tszEnBuffer2, nRet2))
		{
			printf("errno\n");
		}

		int nFilterLen = 0;
		if (AVFilter_Audio_MIXRecv(xhFilter, (uint8_t*)tszFilterBuffer, &nFilterLen))
		{
			fwrite(tszFilterBuffer, 1, nFilterLen, pSt_FileAac);
		}
	}
	AVFilter_Audio_MIXDestroy(xhFilter);
	fclose(pSt_File1);
	fclose(pSt_File2);
	fclose(pSt_FileAac);
}
int main()
{
	//Test_FilterAudio();
	Test_FilterVideo();

	Test_FilterMutliAudio();
	//Test_FilterMutliVideo();
	return 0;
}