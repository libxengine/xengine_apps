#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVHelp.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVCollect/AVCollect_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_VideoCodec/VideoCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AudioCodec/AudioCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Error.h"

//Linux::g++ -std=c++17 -Wall -g AVCodec_APPHelp.cpp -o AVCodec_APPHelp.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCodec -lXEngine_BaseLib -lXEngine_AVHelp -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCodec,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g AVCodec_APPHelp.cpp -o AVCodec_APPHelp.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_AVCodec -lXEngine_BaseLib -lXEngine_AVHelp

void Test_MetaInfo()
{
	int nListCount = 0;
	AVHELP_METADATA st_AVMetaData;
	AVHELP_METAINFO** ppSt_MetaList;

#ifdef _MSC_BUILD
	LPCXSTR lpszSrcFile = "D:\\audio\\1.mp3";
#else
	LPCXSTR lpszSrcFile = "1.mp3";
#endif

	memset(&st_AVMetaData, '\0', sizeof(AVHELP_METADATA));
	if (!AVHelp_MetaInfo_Get(lpszSrcFile, &st_AVMetaData, &ppSt_MetaList, &nListCount))
	{
		return;
	}
	for (int i = 0; i < nListCount; i++)
	{
#ifdef _MSC_BUILD
		XCHAR tszAStr[1024];
		memset(tszAStr, '\0', sizeof(tszAStr));
		int nLen = strlen(ppSt_MetaList[i]->tszValue);

		BaseLib_OperatorCharset_UTFToAnsi(ppSt_MetaList[i]->tszValue, tszAStr, &nLen);
		printf("%s %s\n", ppSt_MetaList[i]->tszKey, tszAStr);
#else
		printf("%s %s\n", ppSt_MetaList[i]->tszKey, ppSt_MetaList[i]->tszValue);
#endif
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&st_AVMetaData.ppSt_AVList, st_AVMetaData.nNBStream);
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_MetaList, nListCount);
}

void Test_Parse()
{
	XNETHANDLE xhToken = 0;
#ifdef _MSC_BUILD
	FILE* pSt_File = fopen("D:\\h264 file\\480p.264", "rb");
#else
	FILE* pSt_File = fopen("480p.264", "rb");
#endif

	AVHelp_Parse_FrameInit(&xhToken, ENUM_ENTENGINE_AVCODEC_VEDIO_TYPE_H264);

	while (XTRUE)
	{
		XCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		int nRet = fread(tszMsgBuffer, 1, sizeof(tszMsgBuffer), pSt_File);
		if (nRet <= 0)
		{
			break;
		}
		int nListCount = 0;
		AVHELP_FRAMEDATA** ppSt_Frame;
		AVHelp_Parse_FrameGet(xhToken, tszMsgBuffer, nRet, &ppSt_Frame, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			XENGINE_AVCODEC_VIDEOFRAMETYPE enVideoFrame;
			AVHelp_Parse_H264NaluType(ppSt_Frame[i]->ptszMsgBuffer, &enVideoFrame);
		}
	}
	AVHelp_Parse_FrameClose(xhToken);
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
	XBYTE uszIDLeave[64];
	XCHAR uszFileBuffer[2048];

	memset(uszSPSBuffer, '\0', sizeof(uszSPSBuffer));
	memset(uszPPSBuffer, '\0', sizeof(uszPPSBuffer));
	memset(uszSEIBuffer, '\0', sizeof(uszSEIBuffer));
	memset(uszIDLeave, '\0', sizeof(uszIDLeave));
	memset(uszFileBuffer, '\0', sizeof(uszFileBuffer));
	FILE* pSt_File = fopen(lpszSrcFile, "rb");
	int nRet = fread(uszFileBuffer, 1, sizeof(uszFileBuffer), pSt_File);

	if (!AVHelp_Parse_264Hdr(uszFileBuffer, nRet, uszSPSBuffer, uszPPSBuffer, uszSEIBuffer, uszIDLeave, &nSPSLen, &nPPSLen, &nSEILen))
	{
		return;
	}
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

	if (!AVHelp_Parse_265Hdr(puszFileBuffer, nRet, uszVPSBuffer, uszSPSBuffer, uszPPSBuffer, &nVPSLen, &nSPSLen, &nPPSLen))
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
	AVHelp_Parse_265Paraset(uszVPSBuffer, nVPSLen, &nProspace, &nProID, &nFlags, &nLevelID, tszICStr);
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
		printf("%s %s\n", ppSt_ListAudio[i]->st_MetaInfo.tszKey, ppSt_ListAudio[i]->st_MetaInfo.tszValue);
	}
	for (int i = 0; i < nVideoCount; i++)
	{
		printf("%s %s\n", ppSt_ListVideo[i]->st_MetaInfo.tszKey, ppSt_ListVideo[i]->st_MetaInfo.tszValue);
	}

	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListAudio, nAudioCount);
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListVideo, nVideoCount);
}

int main()
{
	Test_MetaInfo();
	Test_Parse();
	Test_PPS264Info();
	Test_PPS265Info();
	Test_AudioInfo();
	Test_AVList();

	return 0;
}