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
#ifdef _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AVCollect_Define.h>
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AudioCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AudioCodec_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AudioCodec.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVHelp.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVCollect/AVCollect_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_VideoCodec/VideoCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AudioCodec/AudioCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AudioCodec/AudioCodec_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AudioCodec.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVHelp.lib")
#endif
#endif
//Linux::g++ -std=c++17 -Wall -g AVCodec_APPAudio.cpp -o AVCodec_APPAudio.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCodec -lXEngine_BaseLib -lXEngine_AVHelp -lXEngine_AudioCodec -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCodec:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g AVCodec_APPAudio.cpp -o AVCodec_APPAudio.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_AVCodec -lXEngine_BaseLib -lXEngine_AVHelp -lXEngine_AudioCoder

void CALLBACK AudioCodec_Stream_Callback(XNETHANDLE xhNet, uint8_t* pszBuffer, int nLen, AVCODEC_AUDIO_INFO* pSt_AudioInfo, XPVOID lParam)
{
	FILE* pSt_File = (FILE*)lParam;
	printf("%llu write:%d\n", xhNet, nLen);
	fwrite(pszBuffer, 1, nLen, pSt_File);
}

void Audio_ListCodec()
{
	int nEnCount = 0;
	int nDeCount = 0;
	AVCODEC_AUDIO_CODECLIST** ppSt_Encoder;
	AVCODEC_AUDIO_CODECLIST** ppSt_Decoder;

	AudioCodec_Help_GetList(&ppSt_Encoder, &nEnCount, &ppSt_Decoder, &nDeCount);
	for (int i = 0; i < nEnCount; i++)
	{
		printf("AudioCodec_Help_GetList:%s %d\n", ppSt_Encoder[i]->tszCodecName, ppSt_Encoder[i]->nCodecType);
	}
	for (int i = 0; i < nDeCount; i++)
	{
		printf("AudioCodec_Help_GetList:%s %d\n", ppSt_Decoder[i]->tszCodecName, ppSt_Decoder[i]->nCodecType);
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_Encoder, nEnCount);
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_Decoder, nDeCount);
}

void Audio_Encode()
{
	XNETHANDLE xhCoder;
	AVCODEC_AUDIO_INFO st_AudioInfo;
	
	memset(&st_AudioInfo, '\0', sizeof(AVCODEC_AUDIO_INFO));

	st_AudioInfo.nChannel = 2;
	st_AudioInfo.nSampleRate = 44100;
	st_AudioInfo.nBitRate = 64000;
	st_AudioInfo.nSampleFmt = ENUM_AVCOLLECT_AUDIO_SAMPLE_FMT_FLTP;
	st_AudioInfo.enAVCodec = ENUM_XENGINE_AVCODEC_AUDIO_TYPE_AAC;
	if (!AudioCodec_Stream_EnInit(&xhCoder, &st_AudioInfo))
	{
		printf("AudioCodec_Stream_EnInit\n");
		return;
	}
	int nLen = 0;
	if (!AudioCodec_Stream_SetResample(xhCoder, &nLen, 44100, 44100, ENUM_AVCOLLECT_AUDIO_SAMPLE_FMT_S16, ENUM_AVCOLLECT_AUDIO_SAMPLE_FMT_FLTP, 2, 2))
	{
		printf("AudioCodec_Stream_ResamplerInit\n");
		return;
	}
#ifdef _MSC_BUILD
	FILE* pSt_File = fopen("d:\\audio\\44.1k_2_16.pcm", "rb");
	FILE* pSt_FileAac = fopen("d:\\audio\\44.1k_2_16.aac", "wb");
#else
	FILE* pSt_File = fopen("44.1k_2_16.pcm", "rb");
	FILE* pSt_FileAac = fopen("44.1k_2_16.aac", "wb");
#endif

	while (1)
	{
		XCHAR tszEnBuffer[40960];
		XCHAR tszPCMBuffer[40960];

		memset(tszEnBuffer, '\0', sizeof(tszEnBuffer));
		memset(tszPCMBuffer, '\0', sizeof(tszPCMBuffer));

		int nRet = fread(tszPCMBuffer, 1, 1000, pSt_File);
		if (nRet <= 0)
		{
			break;
		}
		int nListCount = 0;
		AVCODEC_AUDIO_MSGBUFFER** ppSt_ListAudio;
		if (AudioCodec_Stream_EnCodec(xhCoder, (uint8_t*)tszPCMBuffer, nRet, &ppSt_ListAudio, &nListCount))
		{
			for (int i = 0; i < nListCount; i++)
			{
				XBYTE byAACHdr[8];
				AVHelp_Packet_AACHdr(byAACHdr, 44100, 2, ppSt_ListAudio[i]->nMsgLen);

				fwrite(byAACHdr, 1, 7, pSt_FileAac);
				fwrite(ppSt_ListAudio[i]->ptszMsgBuffer, 1, ppSt_ListAudio[i]->nMsgLen, pSt_FileAac);
				BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ppSt_ListAudio[i]->ptszMsgBuffer);
			}
		}
		AudioCodec_Stream_Free(&ppSt_ListAudio, nListCount);
	}
	fclose(pSt_File);
	fclose(pSt_FileAac);
	AudioCodec_Stream_Destroy(xhCoder);
}

void Audio_DeCodec()
{
	XNETHANDLE xhCoder;

#ifdef _MSC_BUILD
	FILE* pSt_FileEnCode = fopen("d:\\audio\\44.1k_2_16.aac", "rb");
	FILE* pSt_FileDeCodec = fopen("d:\\audio\\44.1k_2_16.aac.pcm", "wb");
#else
	FILE* pSt_FileEnCode = fopen("44.1k_2_16.aac", "rb");
	FILE* pSt_FileDeCodec = fopen("44.1k_2_16.aac.pcm", "wb");
#endif

	int nLen = 2048;
	XNETHANDLE xhParse = 0;
	AVCODEC_AUDIO_INFO st_AudioInfo;
	memset(&st_AudioInfo, '\0', sizeof(AVCODEC_AUDIO_INFO));

	st_AudioInfo.nSampleRate = 44100;
	st_AudioInfo.nFrameSize = 1124;
	st_AudioInfo.nChannel = 2;
	st_AudioInfo.nSampleFmt = ENUM_AVCOLLECT_AUDIO_SAMPLE_FMT_FLTP;

	if (!AudioCodec_Stream_DeInit(&xhCoder, ENUM_XENGINE_AVCODEC_AUDIO_TYPE_AAC, AudioCodec_Stream_Callback, pSt_FileDeCodec, &st_AudioInfo))
	{
		printf("AudioCodec_Stream_DeInit\n");
		return;
	}
	if (!AudioCodec_Stream_SetResample(xhCoder, &nLen, 44100, 44100, ENUM_AVCOLLECT_AUDIO_SAMPLE_FMT_FLTP, ENUM_AVCOLLECT_AUDIO_SAMPLE_FMT_S16, 2, 2))
	{
		printf("AudioCodec_Stream_ResamplerInit\n");
		return;
	}
	AVHelp_Parse_FrameInit(&xhParse, ENUM_XENGINE_AVCODEC_AUDIO_TYPE_AAC);

	while (1)
	{
		XCHAR tszEnBuffer[20480];
		memset(tszEnBuffer, '\0', sizeof(tszEnBuffer));

		int nRet = fread(tszEnBuffer, 1, nLen, pSt_FileEnCode);
		if (nRet <= 0)
		{
			break;
		}
		int nListCount = 0;
		AVHELP_FRAMEDATA** ppSt_Frame;
		AVHelp_Parse_FrameGet(xhParse, tszEnBuffer, nRet, &ppSt_Frame, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			AudioCodec_Stream_DeCodec(xhCoder, (uint8_t*)ppSt_Frame[i]->ptszMsgBuffer, ppSt_Frame[i]->nMsgLen);
			BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ppSt_Frame[i]->ptszMsgBuffer);
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_Frame, nListCount);
		
	}
	fclose(pSt_FileEnCode);
	fclose(pSt_FileDeCodec);
	AudioCodec_Stream_Destroy(xhCoder);
	AVHelp_Parse_FrameClose(xhParse);
}
void Audio_Mix()
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


	AVCODEC_AUDIO_FILTER** ppSt_AudioFile;
	BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_AudioFile, 2, sizeof(AVCODEC_AUDIO_FILTER));

	ppSt_AudioFile[0]->nSampleFmt = ENUM_AVCOLLECT_AUDIO_SAMPLE_FMT_S16;
	ppSt_AudioFile[0]->nSampleRate = 44100;
	ppSt_AudioFile[0]->nNBSample = 1152;
	ppSt_AudioFile[0]->nChannle = 2;
	ppSt_AudioFile[0]->nIndex = 0;
	strcpy(ppSt_AudioFile[0]->tszArgsName, lpszAudioFile1);

	ppSt_AudioFile[1]->nSampleFmt = ENUM_AVCOLLECT_AUDIO_SAMPLE_FMT_S16;
	ppSt_AudioFile[1]->nSampleRate = 44100;
	ppSt_AudioFile[1]->nNBSample = 1152;
	ppSt_AudioFile[1]->nChannle = 2;
	ppSt_AudioFile[1]->nIndex = 1;
	strcpy(ppSt_AudioFile[1]->tszArgsName, lpszAudioFile2);

	if (!AudioCodec_Help_MixInit(&xhFilter, "sample_rates=44100:sample_fmts=s16:channel_layouts=stereo", &ppSt_AudioFile, 2))
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
		if (!AudioCodec_Help_MixInput(xhFilter, 0, (uint8_t*)tszEnBuffer1, nRet1))
		{
			printf("errno\n");
		}
		if (!AudioCodec_Help_MixInput(xhFilter, 1, (uint8_t*)tszEnBuffer2, nRet2))
		{
			printf("errno\n");
		}

		int nFilterLen = 0;
		if (AudioCodec_Help_MixOutput(xhFilter, (uint8_t*)tszFilterBuffer, &nFilterLen))
		{
			fwrite(tszFilterBuffer, 1, nFilterLen, pSt_FileAac);
		}
	}
	AudioCodec_Help_FilterDestroy(xhFilter);
	fclose(pSt_File1);
	fclose(pSt_File2);
	fclose(pSt_FileAac);
}
int main()
{
	Audio_ListCodec();
	Audio_Encode();
	Audio_DeCodec();
	Audio_Mix();
	return 0;
}