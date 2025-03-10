﻿#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
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
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVHelp.lib")
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
#ifdef _MSC_BUILD
#ifdef _WIN64
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_AVHelp.lib")
#else
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVHelp.lib")
#endif
#endif
#endif

//Linux Macos:g++ -std=c++20 -Wall -g AVCodec_APPHelp.cpp -o AVCodec_APPHelp.exe -lXEngine_BaseLib -lXEngine_AVHelp

void Test_MetaInfo()
{
	int nListCount = 0;
	XENGINE_KEYVALUE** ppSt_MetaList;

#ifdef _MSC_BUILD
	LPCXSTR lpszFile = _X("D:\\h264 file\\1.mp4");
#else
	LPCXSTR lpszFile = _X("output.mp4");
#endif

	if (!AVHelp_MetaInfo_Get(lpszFile, &ppSt_MetaList, &nListCount))
	{
		return;
	}
	for (int i = 0; i < nListCount; i++)
	{
#ifdef _MSC_BUILD
		XCHAR tszAStr[1024];
		memset(tszAStr, '\0', sizeof(tszAStr));
		int nLen = strlen(ppSt_MetaList[i]->tszStrKey);

		BaseLib_Charset_UTFToAnsi(ppSt_MetaList[i]->tszStrVlu, tszAStr, &nLen);
		printf("%s %s\n", ppSt_MetaList[i]->tszStrKey, tszAStr);
#else
		printf("%s %s\n", ppSt_MetaList[i]->tszStrKey, ppSt_MetaList[i]->tszStrVlu);
#endif
	}
	BaseLib_Memory_Free((XPPPMEM)&ppSt_MetaList, nListCount);

	nListCount = 0;
	AVHELP_STREAMINFO** ppSt_ListFile;
	AVHelp_MetaInfo_GetStream(lpszFile, &ppSt_ListFile, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		XENGINE_PROTOCOL_AVINFO st_AVInfo = {};
		AVHelp_MetaInfo_GetAVInfo(lpszFile, i, &st_AVInfo);
	}
	BaseLib_Memory_Free((XPPPMEM)&ppSt_ListFile, nListCount);

	double dlAVTime = 0;
	AVHelp_MetaInfo_GetTime(lpszFile, 0, &dlAVTime);

	double dlStartTime = 0;
	AVHelp_MetaInfo_GetStartTime(lpszFile, 0, &dlStartTime);
}

void Test_PPS264Info()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszSrcFile = "D:\\h264 file\\480p.264";
#else
	LPCXSTR lpszSrcFile = "480p.264";
#endif

	int nSPSLen = 0;
	int nPPSLen = 0;
	int nSEILen = 0;
	XBYTE uszSPSBuffer[64];
	XBYTE uszPPSBuffer[64];
	XBYTE uszSEIBuffer[1024];
	XCHAR uszFileBuffer[2048];

	memset(uszSPSBuffer, '\0', sizeof(uszSPSBuffer));
	memset(uszPPSBuffer, '\0', sizeof(uszPPSBuffer));
	memset(uszSEIBuffer, '\0', sizeof(uszSEIBuffer));
	memset(uszFileBuffer, '\0', sizeof(uszFileBuffer));
	FILE* pSt_File = fopen(lpszSrcFile, "rb");
	int nRet = fread(uszFileBuffer, 1, sizeof(uszFileBuffer), pSt_File);

	if (!AVHelp_Parse_VideoHdr(uszFileBuffer, nRet, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264, NULL, uszSPSBuffer, uszPPSBuffer, uszSEIBuffer, NULL, &nSPSLen, &nPPSLen, &nSEILen))
	{
		return;
	}
	AFHELP_FRAMESPS st_SPSInfo;
	memset(&st_SPSInfo, '\0', sizeof(AFHELP_FRAMESPS));

	AVHelp_Parse_SPSInfo((LPCXSTR)uszSPSBuffer + 1, nSPSLen - 1, &st_SPSInfo);

	printf("SPS-%d:", nSPSLen);
	for (int i = 0; i < nSPSLen; i++)
	{
		printf("%02X ", uszSPSBuffer[i]);
	}
	printf("\nPPS-%d:", nPPSLen);
	for (int i = 0; i < nPPSLen; i++)
	{
		printf("%02X ", uszPPSBuffer[i]);
	}
	printf("\nSEI-%d:", nSEILen);
	for (int i = 0; i < nSEILen; i++)
	{
		printf("%02X ", uszSEIBuffer[i]);
	}
}
void Test_PPS265Info()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszSrcFile = "D:\\h264 file\\2.hevc";
#else
	LPCXSTR lpszSrcFile = "2.hevc";
#endif
	int nSPSLen = 0;
	int nPPSLen = 0;
	int nVPSLen = 0;
	XBYTE uszSPSBuffer[128];
	XBYTE uszPPSBuffer[128];
	XBYTE uszVPSBuffer[128];
	XCHAR* puszFileBuffer = (XCHAR*)malloc(1024 * 1024 * 10);

	memset(uszSPSBuffer, '\0', sizeof(uszSPSBuffer));
	memset(uszPPSBuffer, '\0', sizeof(uszPPSBuffer));
	memset(uszVPSBuffer, '\0', sizeof(uszVPSBuffer));
	memset(puszFileBuffer, '\0', 1024 * 1024 * 10);
	FILE* pSt_File = fopen(lpszSrcFile, "rb");
	int nRet = fread(puszFileBuffer, 1, 1024 * 1024 * 10, pSt_File);

	if (!AVHelp_Parse_VideoHdr(puszFileBuffer, nRet, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H265, uszVPSBuffer, uszSPSBuffer, uszPPSBuffer, NULL, &nVPSLen, &nSPSLen, &nPPSLen))
	{
		return;
	}
	free(puszFileBuffer);

	printf("\nVPS-%d:", nVPSLen);
	for (int i = 0; i < nVPSLen; i++)
	{
		printf("%02X ", uszVPSBuffer[i]);
	}
	printf("\nSPS-%d:", nSPSLen);
	for (int i = 0; i < nSPSLen; i++)
	{
		printf("%02X ", uszSPSBuffer[i]);
	}
	printf("\nPPS-%d:", nPPSLen);
	for (int i = 0; i < nPPSLen; i++)
	{
		printf("%02X ", uszPPSBuffer[i]);
	}

	int nProspace = 0;
	int nProID = 0;
	int nFlags = 0;
	int nLevelID = 0;

	XCHAR tszICStr[1024];
	memset(tszICStr, '\0', sizeof(tszICStr));
	AVHelp_Parse_VPSInfo(uszVPSBuffer, nVPSLen, &nProspace, &nProID, &nFlags, &nLevelID, tszICStr);
	printf("\n%d %d %d %d %s\n", nProspace, nProID, nFlags, nLevelID, tszICStr);
}
void Test_AudioInfo()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszSrcFile = "D:\\h264 file\\test.aac";
#else
	LPCXSTR lpszSrcFile = "test.aac";
#endif
	int nChannel = 0;
	int nSample = 0;
	int nProfile = 0;
	int nConfig = 0;
	XBYTE uszFileBuffer[2048];
	memset(uszFileBuffer, '\0', sizeof(uszFileBuffer));
	FILE* pSt_File = fopen(lpszSrcFile, "rb");

	int nRet = fread(uszFileBuffer, 1, sizeof(uszFileBuffer), pSt_File);
	if (!AVHelp_Parse_AACInfo(uszFileBuffer, nRet, &nChannel, &nSample, &nProfile, &nConfig))
	{
		return;
	}
	memset(uszFileBuffer, '\0', sizeof(uszFileBuffer));
	AVHelp_Packet_AACHdr(uszFileBuffer, nSample, nChannel, 0);
	if (!AVHelp_Parse_AACInfo(uszFileBuffer, 7, &nChannel, &nSample, &nProfile, &nConfig))
	{
		return;
	}

	printf("Test_AudioInfo:%d %d %d %d\n", nChannel, nSample, nProfile, nConfig);
}
void Test_AVList()
{
	int nAudioCount = 0;
	int nVideoCount = 0;
	AVHELP_DEVICEINFO** ppSt_ListAudio;
	AVHELP_DEVICEINFO** ppSt_ListVideo;

	AVHelp_Device_EnumDevice(&ppSt_ListAudio, &ppSt_ListVideo, &nAudioCount, &nVideoCount);
	for (int i = 0; i < nAudioCount; i++)
	{
		printf("%s %s\n", ppSt_ListAudio[i]->st_MetaInfo.tszStrKey, ppSt_ListAudio[i]->st_MetaInfo.tszStrVlu);
	}
	for (int i = 0; i < nVideoCount; i++)
	{
		printf("%s %s\n", ppSt_ListVideo[i]->st_MetaInfo.tszStrKey, ppSt_ListVideo[i]->st_MetaInfo.tszStrVlu);
	}

	BaseLib_Memory_Free((XPPPMEM)&ppSt_ListAudio, nAudioCount);
	BaseLib_Memory_Free((XPPPMEM)&ppSt_ListVideo, nVideoCount);
}

int main()
{
	Test_MetaInfo();
	Test_PPS264Info();
	Test_PPS265Info();
	Test_AudioInfo();
	Test_AVList();

	return 0;
}