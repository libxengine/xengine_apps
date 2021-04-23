#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVHelp.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AVHelp/AVHelp_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AVHelp/AVHelp_Error.h"

//g++ -std=c++17 -Wall -g AVCoder_APPHelp.cpp -o AVCoder_APPHelp.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCoder -lXEngine_BaseLib -lXEngine_AVHelp -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCoder,--disable-new-dtags

void Test_MetaInfo()
{
	int nListCount = 0;
	AVHELP_METADATA st_AVMetaData;
	AVHELP_METAINFO** ppSt_MetaList;

#ifdef _WINDOWS
	LPCTSTR lpszSrcFile = "H:\\h264 file\\1004523.mp3";
#else
	LPCTSTR lpszSrcFile = "1004523.mp3";
#endif

	memset(&st_AVMetaData, '\0', sizeof(AVHELP_METADATA));
	if (!AVHelp_MetaInfo_Get(lpszSrcFile, &st_AVMetaData, &ppSt_MetaList, &nListCount))
	{
		return;
	}
	for (int i = 0; i < nListCount; i++)
	{
#ifdef _WINDOWS
		CHAR tszAStr[1024];
		memset(tszAStr, '\0', sizeof(tszAStr));
		int nLen = strlen(ppSt_MetaList[i]->tszValue);

		BaseLib_OperatorString_UTFToAnsi(ppSt_MetaList[i]->tszValue, tszAStr, &nLen);
		printf("%s %s\n", ppSt_MetaList[i]->tszKey, tszAStr);
#else
		printf("%s %s\n", ppSt_MetaList[i]->tszKey, ppSt_MetaList[i]->tszValue);
#endif
	}
}
void Test_PPS264Info()
{
#ifdef _WINDOWS
	LPCTSTR lpszSrcFile = "H:\\h264 file\\480p.264";
#else
	LPCTSTR lpszSrcFile = "480p.264";
#endif

	int nSPSLen = 0;
	int nPPSLen = 0;
	int nSEILen = 0;
	UCHAR uszSPSBuffer[64];
	UCHAR uszPPSBuffer[64];
	UCHAR uszSEIBuffer[64];
	UCHAR uszIDLeave[64];
	TCHAR uszFileBuffer[2048];

	memset(uszSPSBuffer, '\0', sizeof(uszSPSBuffer));
	memset(uszPPSBuffer, '\0', sizeof(uszPPSBuffer));
	memset(uszSEIBuffer, '\0', sizeof(uszSEIBuffer));
	memset(uszIDLeave, '\0', sizeof(uszIDLeave));
	memset(uszFileBuffer, '\0', sizeof(uszFileBuffer));
	FILE* pSt_File = fopen(lpszSrcFile, "rb");
	int nRet = fread(uszFileBuffer, 1, sizeof(uszFileBuffer), pSt_File);

	if (!AVHelp_MetaInfo_Get264Hdr(uszFileBuffer, nRet, uszSPSBuffer, uszPPSBuffer, uszSEIBuffer, uszIDLeave, &nSPSLen, &nPPSLen, &nSEILen))
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
#ifdef _WINDOWS
	LPCTSTR lpszSrcFile = "H:\\h264 file\\2.hevc";
#else
	LPCTSTR lpszSrcFile = "2.hevc";
#endif

	int nSPSLen = 0;
	int nPPSLen = 0;
	int nVPSLen = 0;
	UCHAR uszSPSBuffer[64];
	UCHAR uszPPSBuffer[64];
	UCHAR uszVPSBuffer[64];
	TCHAR uszFileBuffer[2048];

	memset(uszSPSBuffer, '\0', sizeof(uszSPSBuffer));
	memset(uszPPSBuffer, '\0', sizeof(uszPPSBuffer));
	memset(uszVPSBuffer, '\0', sizeof(uszVPSBuffer));
	memset(uszFileBuffer, '\0', sizeof(uszFileBuffer));
	FILE* pSt_File = fopen(lpszSrcFile, "rb");
	int nRet = fread(uszFileBuffer, 1, sizeof(uszFileBuffer), pSt_File);

	if (!AVHelp_MetaInfo_Get265Hdr(uszFileBuffer, nRet, uszVPSBuffer, uszSPSBuffer, uszPPSBuffer, &nVPSLen, &nSPSLen, &nPPSLen))
	{
		return;
	}

	printf("VPS-%d:", nVPSLen);
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

	TCHAR tszICStr[1024];
	memset(tszICStr, '\0', sizeof(tszICStr));
	AVHelp_MetaInfo_Get265Paraset(uszVPSBuffer, nVPSLen, &nProspace, &nProID, &nFlags, &nLevelID, tszICStr);
	printf("\n%d %d %d %d %s\n", nProspace, nProID, nFlags, nLevelID, tszICStr);
}
void Test_AudioInfo()
{
#ifdef _WINDOWS
	LPCTSTR lpszSrcFile = "H:\\h264 file\\test.aac";
#else
	LPCTSTR lpszSrcFile = "test.aac";
#endif
	int nChannel = 0;
	int nSample = 0;
	int nProfile = 0;
	int nConfig = 0;
	UCHAR uszFileBuffer[2048];
	memset(uszFileBuffer, '\0', sizeof(uszFileBuffer));
	FILE* pSt_File = fopen(lpszSrcFile, "rb");

	int nRet = fread(uszFileBuffer, 1, sizeof(uszFileBuffer), pSt_File);
	if (!AVHelp_MetaInfo_GetAACInfo(uszFileBuffer, nRet, &nChannel, &nSample, &nProfile, &nConfig))
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
		printf("%s %d\n", ppSt_ListAudio[i]->tszName, ppSt_ListAudio[i]->nCardNumber);
	}
	for (int i = 0; i < nVideoCount; i++)
	{
		printf("%s %d\n", ppSt_ListVideo[i]->tszName, ppSt_ListVideo[i]->nCardNumber);
	}

	int nListMeta = 0;
	AVHELP_METAINFO **ppSt_ListMeta;
	AVHelp_Device_DeviceList(&ppSt_ListMeta, &nListMeta);
	for (int i = 0; i < nListMeta; i++)
	{
		printf("%s=%s\n", ppSt_ListMeta[i]->tszKey, ppSt_ListMeta[i]->tszValue);
	}

	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListAudio, nAudioCount);
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListVideo, nVideoCount);
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListMeta, nListMeta);
}

int main()
{
	Test_AVList();
	Test_MetaInfo();
	Test_PPS264Info();
	Test_PPS265Info();
	Test_AudioInfo();
	
	return 0;
}