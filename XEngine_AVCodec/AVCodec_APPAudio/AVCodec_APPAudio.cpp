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
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AVFrame_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVFrame_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AudioCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AudioCodec_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AudioCodec.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVHelp.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVFrame.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_VideoCodec/VideoCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFrame/AVFrame_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFrame/AVFrame_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AudioCodec/AudioCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AudioCodec/AudioCodec_Error.h"
#ifdef _MSC_BUILD
#ifdef _WIN64
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_AudioCodec.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_AVHelp.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_AVFrame.lib")
#else
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AudioCodec.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVHelp.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVFrame.lib")
#endif
#endif
#endif
//Linux Macos:g++ -std=c++20 -Wall -g AVCodec_APPAudio.cpp -o AVCodec_APPAudio.exe -lXEngine_BaseLib -lXEngine_AVHelp -lXEngine_AudioCodec -lXEngine_AVFrame


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
	BaseLib_Memory_Free((XPPPMEM)&ppSt_Encoder, nEnCount);
	BaseLib_Memory_Free((XPPPMEM)&ppSt_Decoder, nDeCount);
}

void Audio_Encode()
{
	AVCODEC_AUDIO_INFO st_AudioInfo;
	memset(&st_AudioInfo, '\0', sizeof(AVCODEC_AUDIO_INFO));

	st_AudioInfo.nChannel = 2;
	st_AudioInfo.nSampleRate = 48000;
	st_AudioInfo.nBitRate = 128000;
	st_AudioInfo.nSampleFmt = ENUM_AVCODEC_AUDIO_SAMPLEFMT_S16;
	st_AudioInfo.enAVCodec = ENUM_XENGINE_AVCODEC_AUDIO_TYPE_AAC;
	XHANDLE xhAudio = AudioCodec_Stream_EnInit(&st_AudioInfo, 0, 0, true);
	if (NULL == xhAudio)
	{
		printf("AudioCodec_Stream_EnInit\n");
		return;
	}
	int nSize = 0;
	AudioCodec_Stream_GetSize(xhAudio, &nSize);
#ifdef _MSC_BUILD
	FILE* pSt_File = fopen("d:\\audio\\b.pcm", "rb");
	FILE* pSt_FileAac = fopen("d:\\audio\\output.aac", "wb");
#else
	FILE* pSt_File = fopen("output.pcm", "rb");
	FILE* pSt_FileAac = fopen("output.aac", "wb");
#endif

	while (1)
	{
		XCHAR tszEnBuffer[4096];
		XCHAR tszPCMBuffer[40960];

		memset(tszEnBuffer, '\0', sizeof(tszEnBuffer));
		memset(tszPCMBuffer, '\0', sizeof(tszPCMBuffer));

		int nRet = fread(tszPCMBuffer, 1, 4096, pSt_File);
		if (nRet <= 0)
		{
			break;
		}
		int nListCount = 0;
		AVCODEC_AUDIO_MSGBUFFER** ppSt_ListAudio;
		AVCODEC_AUDIO_MSGBUFFER st_MSGBuffer = {};
		st_MSGBuffer.st_AudioInfo.nChannel = 2;
		st_MSGBuffer.st_AudioInfo.nNBSample = 1024;
		st_MSGBuffer.st_AudioInfo.nSampleFmt = ENUM_AVCODEC_AUDIO_SAMPLEFMT_S16;
		AudioCodec_Help_Malloc(&st_MSGBuffer);

		memcpy(st_MSGBuffer.st_MSGBuffer.unData.ptszMSGArray[0], tszPCMBuffer, nRet);

		if (AudioCodec_Stream_EnCodec(xhAudio, &st_MSGBuffer, &ppSt_ListAudio, &nListCount))
		{
			for (int i = 0; i < nListCount; i++)
			{
				XBYTE byAACHdr[8];
				AVHelp_Packet_AACHdr(byAACHdr, 48000, 2, ppSt_ListAudio[i]->st_MSGBuffer.nMSGLen[0]);

				fwrite(byAACHdr, 1, 7, pSt_FileAac);
				fwrite(ppSt_ListAudio[i]->st_MSGBuffer.unData.ptszMSGArray[0], 1, ppSt_ListAudio[i]->st_MSGBuffer.nMSGLen[0], pSt_FileAac);
				BaseLib_Memory_MSGFree(&ppSt_ListAudio[i]->st_MSGBuffer);
			}
		}
		BaseLib_Memory_Free((XPPPMEM)&ppSt_ListAudio, nListCount);
	}
	int nListCount = 0;
	AVCODEC_AUDIO_MSGBUFFER** ppSt_ListAudio;
	AudioCodec_Stream_EnCodec(xhAudio, NULL, &ppSt_ListAudio, &nListCount);
	BaseLib_Memory_Free((XPPPMEM)&ppSt_ListAudio, nListCount);

	fclose(pSt_File);
	fclose(pSt_FileAac);
	AudioCodec_Stream_Destroy(xhAudio);
}

void Audio_DeCodec()
{
#ifdef _MSC_BUILD
	FILE* pSt_FileEnCode = fopen("d:\\audio\\output.aac", "rb");
	FILE* pSt_FileDeCodec = fopen("d:\\audio\\output_de.pcm", "wb");
#else
	FILE* pSt_FileEnCode = fopen("output.aac", "rb");
	FILE* pSt_FileDeCodec = fopen("output_de.pcm", "wb");
#endif

	int nLen = 2048;
	XNETHANDLE xhParse = 0;
	AVCODEC_AUDIO_INFO st_AudioInfo;
	memset(&st_AudioInfo, '\0', sizeof(AVCODEC_AUDIO_INFO));

	st_AudioInfo.nSampleRate = 48000;
	st_AudioInfo.nNBSample = 1024;
	st_AudioInfo.nChannel = 2;
	st_AudioInfo.nSampleFmt = ENUM_AVCODEC_AUDIO_SAMPLEFMT_S16;

	XHANDLE xhAudio = AudioCodec_Stream_DeInit(ENUM_XENGINE_AVCODEC_AUDIO_TYPE_AAC);
	if (NULL == xhAudio)
	{
		printf("AudioCodec_Stream_DeInit\n");
		return;
	}

	if (nLen <= 0)
	{
		nLen = 10240;
	}
	AVFrame_Frame_ParseInit(&xhParse, ENUM_XENGINE_AVCODEC_AUDIO_TYPE_AAC);

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
		XENGINE_MSGBUFFER** ppSt_Frame;
		AVFrame_Frame_ParseGet(xhParse, tszEnBuffer, nRet, &ppSt_Frame, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			AVCODEC_AUDIO_MSGBUFFER** ppSt_ListMsgBuffer;
			AVCODEC_AUDIO_MSGBUFFER st_MSGBuffer = {};
			st_MSGBuffer.st_MSGBuffer.nMSGLen[0] = ppSt_Frame[i]->nMSGLen[0];
			BaseLib_Memory_MSGMalloc(&st_MSGBuffer.st_MSGBuffer);
			st_MSGBuffer.st_MSGBuffer.unData.ptszMSGArray[0] = (uint8_t*)ppSt_Frame[i]->unData.ptszMSGBuffer;
			
			int nAVCount = 0;
			AudioCodec_Stream_DeCodec(xhAudio, &st_MSGBuffer, &ppSt_ListMsgBuffer, &nAVCount);
			for (int i = 0; i < nAVCount; i++)
			{
				fwrite(ppSt_ListMsgBuffer[i]->st_MSGBuffer.unData.ptszMSGArray[0], 1, ppSt_ListMsgBuffer[i]->st_MSGBuffer.nMSGLen[0], pSt_FileDeCodec);
				BaseLib_Memory_MSGFree(&ppSt_ListMsgBuffer[i]->st_MSGBuffer);
			}
			BaseLib_Memory_Free((XPPPMEM)&ppSt_ListMsgBuffer, nAVCount);
		}
		BaseLib_Memory_Free((XPPPMEM)&ppSt_Frame, nListCount);
	}
	fclose(pSt_FileEnCode);
	fclose(pSt_FileDeCodec);
	AudioCodec_Stream_Destroy(xhAudio);
	AVFrame_Frame_ParseClose(xhParse);
}
void OPUS_Encode()
{
	AVCODEC_AUDIO_INFO st_AudioInfo;
	memset(&st_AudioInfo, '\0', sizeof(AVCODEC_AUDIO_INFO));

	st_AudioInfo.nChannel = 2;
	st_AudioInfo.nSampleRate = 48000;
	st_AudioInfo.nBitRate = 64000;
	st_AudioInfo.nSampleFmt = ENUM_AVCODEC_AUDIO_SAMPLEFMT_S16;
	st_AudioInfo.enAVCodec = ENUM_XENGINE_AVCODEC_AUDIO_TYPE_OPUS;

	XHANDLE xhAudio = AudioCodec_Stream_EnInit(&st_AudioInfo);
	if (NULL == xhAudio)
	{
		printf("AudioCodec_Stream_EnInit\n");
		return;
	}
#ifdef _MSC_BUILD
	FILE* pSt_File = fopen("d:\\audio\\mix.pcm", "rb");
	FILE* pSt_FileAac = fopen("d:\\audio\\output1.opus", "wb");
#else
	FILE* pSt_File = fopen("mix.pcm", "rb");
	FILE* pSt_FileAac = fopen("output1.opus", "wb");
#endif

	int nSize = 0;
	AudioCodec_Stream_GetSize(xhAudio, &nSize);
	while (1)
	{
		XCHAR tszEnBuffer[4608];
		XCHAR tszPCMBuffer[3840];  // 3840 == nSize

		memset(tszEnBuffer, '\0', sizeof(tszEnBuffer));
		memset(tszPCMBuffer, '\0', sizeof(tszPCMBuffer));

		int nRet = fread(tszPCMBuffer, 1, sizeof(tszPCMBuffer), pSt_File);
		if (nRet <= 0)
		{
			break;
		}
		int nListCount = 0;
		AVCODEC_AUDIO_MSGBUFFER** ppSt_ListAudio;

		AVCODEC_AUDIO_MSGBUFFER st_MSGBuffer = {};
		st_MSGBuffer.st_AudioInfo.nChannel = 2;
		st_MSGBuffer.st_AudioInfo.nNBSample = 1024;
		st_MSGBuffer.st_AudioInfo.nSampleFmt = ENUM_AVCODEC_AUDIO_SAMPLEFMT_S16;

		st_MSGBuffer.st_MSGBuffer.nMSGLen[0] = nRet;
		BaseLib_Memory_MSGMalloc(&st_MSGBuffer.st_MSGBuffer);
		memcpy(st_MSGBuffer.st_MSGBuffer.unData.ptszMSGArray[0], tszPCMBuffer, nRet);

		if (AudioCodec_Stream_EnCodec(xhAudio, &st_MSGBuffer, &ppSt_ListAudio, &nListCount))
		{
			for (int i = 0; i < nListCount; i++)
			{
				fwrite(ppSt_ListAudio[i]->st_MSGBuffer.unData.ptszMSGArray[0], 1, ppSt_ListAudio[i]->st_MSGBuffer.nMSGLen[0], pSt_FileAac);
				BaseLib_Memory_MSGFree(&ppSt_ListAudio[i]->st_MSGBuffer);
			}
		}
		BaseLib_Memory_Free((XPPPMEM)&ppSt_ListAudio, nListCount);
	}
	fclose(pSt_File);
	fclose(pSt_FileAac);
	AudioCodec_Stream_Destroy(xhAudio);
}
int main()
{
	int nListCount = 0;
	ENUM_AVCODEC_AUDIO_SAMPLEFMT** ppenListSamples;
	AudioCodec_Help_GetFmtList(ENUM_XENGINE_AVCODEC_AUDIO_TYPE_MP2, &ppenListSamples, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		printf("%d\n", *ppenListSamples[i]);
	}
	BaseLib_Memory_Free((XPPPMEM)&ppenListSamples, nListCount);

	int** ppInt_RateList;
	nListCount = 0;
	AudioCodec_Help_GetRateList(ENUM_XENGINE_AVCODEC_AUDIO_TYPE_MP2, &ppInt_RateList, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		printf("%d\n", *ppInt_RateList[i]);
	}
	BaseLib_Memory_Free((XPPPMEM)&ppInt_RateList, nListCount);

	XCHAR** pptszChList;
	nListCount = 0;
	AudioCodec_Help_GetChList(ENUM_XENGINE_AVCODEC_AUDIO_TYPE_MP3, &pptszChList, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		printf("%s\n", pptszChList[i]);
	}
	BaseLib_Memory_Free((XPPPMEM)&pptszChList, nListCount);

	Audio_Encode();
	Audio_DeCodec();
	OPUS_Encode();
	Audio_ListCodec();

	return 0;
}