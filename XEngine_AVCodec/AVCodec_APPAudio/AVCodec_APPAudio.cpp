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
#include <XEngine_Include/XEngine_AVCodec/AVCollect_Define.h>
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
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVCollect/AVCollect_Define.h"
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
	XNETHANDLE xhCoder;
	AVCODEC_AUDIO_INFO st_AudioInfo;
	
	memset(&st_AudioInfo, '\0', sizeof(AVCODEC_AUDIO_INFO));

	st_AudioInfo.nChannel = 2;
	st_AudioInfo.nSampleRate = 48000;
	st_AudioInfo.nBitRate = 128000;
	st_AudioInfo.nSampleFmt = ENUM_AVCODEC_AUDIO_SAMPLEFMT_S16;
	st_AudioInfo.enAVCodec = ENUM_XENGINE_AVCODEC_AUDIO_TYPE_AAC;
	if (!AudioCodec_Stream_EnInit(&xhCoder, &st_AudioInfo))
	{
		printf("AudioCodec_Stream_EnInit\n");
		return;
	}
#ifdef _MSC_BUILD
	FILE* pSt_File = fopen("d:\\audio\\output.pcm", "rb");
	FILE* pSt_FileAac = fopen("d:\\audio\\output.aac", "wb");
#else
	FILE* pSt_File = fopen("output.pcm", "rb");
	FILE* pSt_FileAac = fopen("output.aac", "wb");
#endif

	while (1)
	{
		XCHAR tszEnBuffer[4608];
		XCHAR tszPCMBuffer[40960];

		memset(tszEnBuffer, '\0', sizeof(tszEnBuffer));
		memset(tszPCMBuffer, '\0', sizeof(tszPCMBuffer));

		int nRet = fread(tszPCMBuffer, 1, 4608, pSt_File);
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
				AVHelp_Packet_AACHdr(byAACHdr, 48000, 2, ppSt_ListAudio[i]->nMsgLen);

				fwrite(byAACHdr, 1, 7, pSt_FileAac);
				fwrite(ppSt_ListAudio[i]->ptszMsgBuffer, 1, ppSt_ListAudio[i]->nMsgLen, pSt_FileAac);
				BaseLib_Memory_FreeCStyle((XPPMEM)&ppSt_ListAudio[i]->ptszMsgBuffer);
			}
		}
		AudioCodec_Stream_Free(&ppSt_ListAudio, nListCount);
	}
	int nListCount = 0;
	AVCODEC_AUDIO_MSGBUFFER** ppSt_ListAudio;
	AudioCodec_Stream_EnCodec(xhCoder, NULL, 0, &ppSt_ListAudio, &nListCount);
	AudioCodec_Stream_Free(&ppSt_ListAudio, nListCount);

	fclose(pSt_File);
	fclose(pSt_FileAac);
	AudioCodec_Stream_Destroy(xhCoder);
}

void Audio_DeCodec()
{
	XNETHANDLE xhCoder;

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
	st_AudioInfo.nFrameSize = 1024;
	st_AudioInfo.nChannel = 2;
	st_AudioInfo.nSampleFmt = ENUM_AVCODEC_AUDIO_SAMPLEFMT_S16;

	if (!AudioCodec_Stream_DeInit(&xhCoder, ENUM_XENGINE_AVCODEC_AUDIO_TYPE_AAC, &st_AudioInfo))
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
		AVFRAME_PARSEDATA** ppSt_Frame;
		AVFrame_Frame_ParseGet(xhParse, tszEnBuffer, nRet, &ppSt_Frame, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			int nListCount = 0;
			AVCODEC_AUDIO_MSGBUFFER** ppSt_ListMsgBuffer;
			AudioCodec_Stream_DeCodec(xhCoder, (uint8_t*)ppSt_Frame[i]->ptszMsgBuffer, ppSt_Frame[i]->nMsgLen, &ppSt_ListMsgBuffer, &nListCount);
			for (int i = 0; i < nListCount; i++)
			{
				fwrite(ppSt_ListMsgBuffer[i]->ptszMsgBuffer, 1, ppSt_ListMsgBuffer[i]->nMsgLen, pSt_FileDeCodec);
			}
			BaseLib_Memory_FreeCStyle((XPPMEM)&ppSt_Frame[i]->ptszMsgBuffer);
		}
		BaseLib_Memory_Free((XPPPMEM)&ppSt_Frame, nListCount);
		
	}
	fclose(pSt_FileEnCode);
	fclose(pSt_FileDeCodec);
	AudioCodec_Stream_Destroy(xhCoder);
	AVFrame_Frame_ParseClose(xhParse);
}
void OPUS_Encode()
{
	XNETHANDLE xhCoder;
	AVCODEC_AUDIO_INFO st_AudioInfo;

	memset(&st_AudioInfo, '\0', sizeof(AVCODEC_AUDIO_INFO));

	st_AudioInfo.nChannel = 2;
	st_AudioInfo.nSampleRate = 48000;
	st_AudioInfo.nBitRate = 64000;
	st_AudioInfo.nSampleFmt = ENUM_AVCODEC_AUDIO_SAMPLEFMT_S16;
	st_AudioInfo.enAVCodec = ENUM_XENGINE_AVCODEC_AUDIO_TYPE_OPUS;
	if (!AudioCodec_Stream_EnInit(&xhCoder, &st_AudioInfo))
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
	AudioCodec_Stream_GetSize(xhCoder, &nSize);
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
		if (AudioCodec_Stream_EnCodec(xhCoder, (uint8_t*)tszPCMBuffer, nRet, &ppSt_ListAudio, &nListCount))
		{
			for (int i = 0; i < nListCount; i++)
			{
				fwrite(ppSt_ListAudio[i]->ptszMsgBuffer, 1, ppSt_ListAudio[i]->nMsgLen, pSt_FileAac);
				BaseLib_Memory_FreeCStyle((XPPMEM)&ppSt_ListAudio[i]->ptszMsgBuffer);
			}
		}
		AudioCodec_Stream_Free(&ppSt_ListAudio, nListCount);
	}
	fclose(pSt_File);
	fclose(pSt_FileAac);
	AudioCodec_Stream_Destroy(xhCoder);
}
int main()
{
	Audio_Encode();
	Audio_DeCodec();
	OPUS_Encode();
	Audio_ListCodec();

	return 0;
}