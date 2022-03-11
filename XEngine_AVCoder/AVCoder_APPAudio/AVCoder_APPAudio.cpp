#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AudioCoder.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVHelp.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AVCollect/AVCollect_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_VideoCoder/VideoCoder_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AVHelp/AVHelp_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AVHelp/AVHelp_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AudioCoder/AudioCoder_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AudioCoder/AudioCoder_Error.h"

//g++ -std=c++17 -Wall -g AVCoder_APPAudio.cpp -o AVCoder_APPAudio.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCoder -lXEngine_BaseLib -lXEngine_AVHelp -lXEngine_AudioCoder -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCoder:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk,--disable-new-dtags


void __stdcall AudioCodec_Stream_Callback(XNETHANDLE xhNet, uint8_t* pszBuffer, int nLen, AVCODEC_AUDIO_INFO* pSt_AudioInfo, LPVOID lParam)
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
	if (!AudioCodec_Stream_EnInit(&xhCoder, ENUM_AVCODEC_AUDIO_TYPE_AAC, 44100, 2, 64000, 0, ENUM_AVCOLLECT_AUDIO_SAMPLE_FMT_FLTP))
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
#ifdef _WINDOWS
	FILE* pSt_File = fopen("d:\\audio\\44.1k_2_16.pcm", "rb");
	FILE* pSt_FileAac = fopen("d:\\audio\\44.1k_2_16.aac", "wb");
#else
	FILE* pSt_File = fopen("44.1k_2_16.pcm", "rb");
	FILE* pSt_FileAac = fopen("44.1k_2_16.aac", "wb");
#endif

	while (1)
	{
		TCHAR tszEnBuffer[40960];
		TCHAR tszPCMBuffer[40960];

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
				BYTE byAACHdr[8];
				AVHelp_Packet_AACHdr(byAACHdr, 44100, 2, ppSt_ListAudio[i]->nMsgLen);

				fwrite(byAACHdr, 1, 7, pSt_FileAac);
				fwrite(ppSt_ListAudio[i]->pbyMsgBuffer, 1, ppSt_ListAudio[i]->nMsgLen, pSt_FileAac);
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

#ifdef _WINDOWS
	FILE* pSt_FileEnCode = fopen("H:\\audio\\44.1k_2_16.aac", "rb");
	FILE* pSt_FileDeCodec = fopen("H:\\audio\\44.1k_2_16.aac.pcm", "wb");
#else
	FILE* pSt_FileEnCode = fopen("44.1k_2_16.aac", "rb");
	FILE* pSt_FileDeCodec = fopen("44.1k_2_16.aac.pcm", "wb");
#endif

	int nLen = 2048;
	AVCODEC_AUDIO_INFO st_AudioInfo;
	memset(&st_AudioInfo, '\0', sizeof(AVCODEC_AUDIO_INFO));

	st_AudioInfo.nSampleRate = 44100;
	st_AudioInfo.nNBSample = 1124;
	st_AudioInfo.nChannle = 2;
	st_AudioInfo.enSampleFmt = ENUM_AVCOLLECT_AUDIO_SAMPLE_FMT_FLTP;

	if (!AudioCodec_Stream_DeInit(&xhCoder, ENUM_AVCODEC_AUDIO_TYPE_AAC, AudioCodec_Stream_Callback, pSt_FileDeCodec, &st_AudioInfo))
	{
		printf("AudioCodec_Stream_DeInit\n");
		return;
	}
	if (!AudioCodec_Stream_SetResample(xhCoder, &nLen, 44100, 44100, ENUM_AVCOLLECT_AUDIO_SAMPLE_FMT_FLTP, ENUM_AVCOLLECT_AUDIO_SAMPLE_FMT_S16, 2, 2))
	{
		printf("AudioCodec_Stream_ResamplerInit\n");
		return;
	}

	while (1)
	{
		TCHAR tszEnBuffer[20480];
		memset(tszEnBuffer, '\0', sizeof(tszEnBuffer));

		int nRet = fread(tszEnBuffer, 1, nLen, pSt_FileEnCode);
		if (nRet <= 0)
		{
			break;
		}
		AudioCodec_Stream_DeCodec(xhCoder, (uint8_t*)tszEnBuffer, nRet);
	}
	fclose(pSt_FileEnCode);
	fclose(pSt_FileDeCodec);
	AudioCodec_Stream_Destroy(xhCoder);
}
void Audio_Mix()
{
	XNETHANDLE xhFilter;

#ifdef _WINDOWS
	LPCTSTR lpszAudioFile1 = "H:\\audio\\3.pcm";
	LPCTSTR lpszAudioFile2 = "H:\\audio\\4.pcm";
	LPCTSTR lpszDstFile = "H:\\audio\\mix.pcm";
#else
	LPCTSTR lpszAudioFile1 = "3.pcm";
	LPCTSTR lpszAudioFile2 = "4.pcm";
	LPCTSTR lpszDstFile = "mix.pcm";
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
	FILE* pSt_File1 = fopen(lpszAudioFile1, _T("rb"));
	FILE* pSt_File2 = fopen(lpszAudioFile2, _T("rb"));
	FILE* pSt_FileAac = fopen(lpszDstFile, _T("wb"));
	while (1)
	{
		TCHAR tszEnBuffer1[4608];
		TCHAR tszEnBuffer2[4608];
		TCHAR tszFilterBuffer[8196];

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