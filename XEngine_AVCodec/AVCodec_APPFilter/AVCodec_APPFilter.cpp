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

//Linux::g++ -std=c++17 -Wall -g AVCodec_APPFilter.cpp -o AVCodec_APPFilter.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCodec -lXEngine_BaseLib -lXEngine_AVFilter -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCodec,--disable-new-dtags
//MacOS::g++ -std=c++17 -Wall -g AVCodec_APPVideo.cpp -o AVCodec_APPVideo.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_AVCodec -lXEngine_BaseLib -lXEngine_AVFilter

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
		XCHAR tszWBBuffer[8192] = {};
		XCHAR tszRDBuffer[8192] = {};
		int nRSize = 1024 * 2 * 2;
		int nWSize = 1024 * 2 * 2;

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

	FILE* pSt_RBFile = _xtfopen("D:\\windows-ffmpeg\\x64\\output.yuv", _X("rb"));
	FILE* pSt_WBFile = _xtfopen("D:\\windows-ffmpeg\\x64\\output_w.yuv", _X("wb"));

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
	AVFILTER_VIDEO_INFO st_VideoDst = {};
	AVFILTER_VIDEO_INFO **ppSt_VideoInfo;

	BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_VideoInfo, nVideoList, sizeof(AVFILTER_VIDEO_INFO));

	ppSt_VideoInfo[0]->nIndex = 0;
	ppSt_VideoInfo[0]->nWidth = 720;
	ppSt_VideoInfo[0]->nHeight = 480;
	ppSt_VideoInfo[0]->nFrame = 24;
	ppSt_VideoInfo[0]->enAVPixForamt = ENUM_AVCOLLECT_VIDEO_FMT_YUV420P;
	
	ppSt_VideoInfo[1]->nIndex = 1;
	ppSt_VideoInfo[1]->nWidth = 720;
	ppSt_VideoInfo[1]->nHeight = 480;
	ppSt_VideoInfo[1]->nFrame = 24;
	ppSt_VideoInfo[1]->enAVPixForamt = ENUM_AVCOLLECT_VIDEO_FMT_YUV420P;

	ppSt_VideoInfo[2]->nIndex = 2;
	ppSt_VideoInfo[2]->nWidth = 720;
	ppSt_VideoInfo[2]->nHeight = 480;
	ppSt_VideoInfo[2]->nFrame = 24;
	ppSt_VideoInfo[2]->enAVPixForamt = ENUM_AVCOLLECT_VIDEO_FMT_YUV420P;

	ppSt_VideoInfo[3]->nIndex = 3;
	ppSt_VideoInfo[3]->nWidth = 720;
	ppSt_VideoInfo[3]->nHeight = 480;
	ppSt_VideoInfo[3]->nFrame = 24;
	ppSt_VideoInfo[3]->enAVPixForamt = ENUM_AVCOLLECT_VIDEO_FMT_YUV420P;

	st_VideoDst.nWidth = 720;
	st_VideoDst.nHeight = 480;
	st_VideoDst.nFrame = 24;
	st_VideoDst.enAVPixForamt = ENUM_AVCOLLECT_VIDEO_FMT_YUV420P;
	
	XCHAR tszFilterStr[512] = {};
	/*
	_xstprintf(tszFilterStr, _X("nullsrc=size=720x480 [base];"
		"[in0] scale=380x480 [left];"
		"[in1] scale=380x480 [right];"
		"[base] overlay=shortest=1[tmp1];"
		"[tmp1] overlay=shortest=1:x=380;"));
	
	_xstprintf(tszFilterStr,
		"buffer=video_size=720x480:pix_fmt=0:time_base=1/24:[main];"
		"buffer=video_size=720x480:pix_fmt=0:time_base=1/24:[logo];"
		"[logo]scale=iw/2:ih/2[small];"
		"[main][small]overlay=W/2:H/2[result];"
		"[result]buffersink");
		
	_xstprintf(tszFilterStr, _X("nullsrc=size=720x480 [base];"
		"[in0] scale=360x240 [upperleft];"
		"[in1] scale=360x240 [upperright];"
		"[in2] scale=360x240 [lowerleft];"
		"[in3] scale=360x240 [lowerright];"
		"[base][upperleft] overlay=shortest=1[tmp1];"
		"[tmp1][upperright] overlay=shortest=1:x=360 [tmp2];"
		"[tmp2][lowerleft] overlay=shortest=1:y=240 [tmp3];"
		"[tmp3][lowerright] overlay=shortest=1:x=360:y=240"));
		*/
	_xstprintf(tszFilterStr, _X("buffer=width=480:height=720:pix_fmt=0:time_base=1/24 [pad_in];"
		"[pad_in] split=4 [in0] [in1] [in2] [in3];"
		"[in0] crop=w=240:h=380:x=0:y=0 [tmp1];"
		"[in1] [tmp1]crop=w=240:h=380:x=380:y=0 [tmp2];"
		"[in2] [tmp2]crop=w=240:h=380:x=0:y=240 [tmp3];"
		"[in3] [tmp3]crop=w=240:h=380:x=380:y=240"));
	AVFilter_Video_MIXInit(&xhToken, tszFilterStr, &ppSt_VideoInfo, nVideoList, &st_VideoDst);

	FILE* pSt_RBFile = _xtfopen("D:\\windows-ffmpeg\\x64\\output_1.yuv", _X("rb"));
	FILE* pSt_WBFile = _xtfopen("D:\\windows-ffmpeg\\x64\\output_w.yuv", _X("wb"));

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
	ppSt_AudioFile[0]->nNBSample = 1152;
	ppSt_AudioFile[0]->nChannle = 2;
	ppSt_AudioFile[0]->nIndex = 0;

	ppSt_AudioFile[1]->nSampleFmt = ENUM_AVCOLLECT_AUDIO_SAMPLE_FMT_S16;
	ppSt_AudioFile[1]->nSampleRate = 44100;
	ppSt_AudioFile[1]->nNBSample = 1152;
	ppSt_AudioFile[1]->nChannle = 2;
	ppSt_AudioFile[1]->nIndex = 1;

	if (!AVFilter_Audio_MIXInit(&xhFilter, "sample_rates=44100:sample_fmts=s16:channel_layouts=stereo", &ppSt_AudioFile, 2))
	{
		printf("AudioCodec_Help_FilterInit\n");
		return;
	}
	FILE* pSt_File1 = fopen(lpszAudioFile1, _X("rb"));
	FILE* pSt_File2 = fopen(lpszAudioFile2, _X("rb"));
	FILE* pSt_FileAac = fopen(lpszDstFile, _X("wb"));
	while (1)
	{
		XCHAR tszEnBuffer1[4608];
		XCHAR tszEnBuffer2[4608];
		XCHAR tszFilterBuffer[8196];

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
	//Test_FilterVideo();

	Test_FilterMutliAudio();
	Test_FilterMutliVideo();
	return 0;
}