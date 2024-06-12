#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
using namespace std;
#if 1 == _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AVFormat_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVFormat_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVFormat.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFormat/AVFormat_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFormat/AVFormat_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVFormat.lib")
#endif
#endif

//Linux::g++ -std=c++17 -Wall -g AVCodec_APPPacket.cpp -o AVCodec_APPPacket.exe -lXEngine_BaseLib -lXEngine_AVFormat
FILE* pSt_File;

void CALLBACK AVPacket_Pack_CBNotify(XHANDLE xhNet, int nCvtType, __int64x nCvtFrame, double dlTime, XPVOID lParam)
{
	printf("AVPacket_Pack_CBConvert:%d %lld %lf\n", nCvtType, nCvtFrame, dlTime);
}
int AVFormat_Packet_RW(XPVOID lParam, uint8_t* puszMsgBuffer, int nSize)
{
	static int nCount = 0;
	int nRet = fread(puszMsgBuffer, 1, 256, pSt_File);

	printf("%d\n", nCount += nRet);
	return nRet;
}

int AVPacket_Test_FileLink()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszSrcFile1 = "D:\\h264 file\\1.mp4";
	LPCXSTR lpszSrcFile2 = "D:\\h264 file\\2.mp4";
	LPCXSTR lpszSrcFile3 = "D:\\h264 file\\3.mp4";
	LPCXSTR lpszDstFile = "D:\\h264 file\\out.mp4";
#else
	LPCXSTR lpszSrcFile1 = "480p.flv";
	LPCXSTR lpszSrcFile2 = "480p1.flv";
	LPCXSTR lpszDstFile = "480m.flv";
#endif

	XHANDLE xhAVFile = AVFormat_Link_Init(AVPacket_Pack_CBNotify);
	if (NULL == xhAVFile)
	{
		printf("AVFormat_Convert_Init:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	if (!AVFormat_Link_Output(xhAVFile, lpszDstFile))
	{
		printf("AVFormat_Convert_Output:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	if (!AVFormat_Link_Input(xhAVFile, lpszSrcFile1))
	{
		printf("AVFormat_Convert_Input:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	if (!AVFormat_Link_Input(xhAVFile, lpszSrcFile2))
	{
		printf("AVFormat_Convert_Input:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	if (!AVFormat_Link_Input(xhAVFile, lpszSrcFile3))
	{
		printf("AVFormat_Convert_Input:%lX\n", AVFormat_GetLastError());
		return -1;
	}


	if (!AVFormat_Link_Start(xhAVFile))
	{
		printf("AVFormat_Convert_Start:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	while (1)
	{
		bool bIsRun = false;
		if (AVFormat_Link_GetStatus(xhAVFile, &bIsRun))
		{
			if (!bIsRun)
			{
				break;
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	return AVFormat_Link_Stop(xhAVFile);
}

int AVPacket_Test_FileConvert()
{
	double nTotalAVTime = 0;
#ifdef _MSC_BUILD
	LPCXSTR lpszSrcFile = "D:\\h264 file\\720x480.264";
	LPCXSTR lpszDstFile = "D:\\windows-ffmpeg\\x64\\1.mp4";
#else
	LPCXSTR lpszSrcFile = "480p.flv";
	LPCXSTR lpszDstFile = "conv.mp4";
#endif

	XHANDLE xhAVFile = AVFormat_Convert_Init(AVPacket_Pack_CBNotify);
	if (NULL == xhAVFile)
	{
		printf("AVFormat_Convert_Init:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	if (!AVFormat_Convert_Output(xhAVFile, lpszDstFile))
	{
		printf("AVFormat_Convert_Output:%lX\n", AVFormat_GetLastError());
		return -1;
	}

	pSt_File = fopen(lpszSrcFile, "rb");
	if (!AVFormat_Convert_Input(xhAVFile, NULL, &nTotalAVTime, AVFormat_Packet_RW))
	{
		printf("AVFormat_Convert_Input:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	printf("AVTime:%lf\n", nTotalAVTime);

	if (!AVFormat_Convert_Start(xhAVFile))
	{
		printf("AVFormat_Convert_Start:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	while (1)
	{
		bool bIsRun = false;
		if (AVFormat_Convert_GetStatus(xhAVFile, &bIsRun))
		{
			if (!bIsRun)
			{
				break;
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	return AVFormat_Convert_Stop(xhAVFile);
}

int AVPacket_Test_FilePacket()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszVideoFile = "D:\\h264 file\\480p.264";
	LPCXSTR lpszAudioFile1 = "D:\\h264 file\\1.aac";
	LPCXSTR lpszAudioFile2 = "D:\\h264 file\\test.aac";
	LPCXSTR lpszDstFile = "D:\\h264 file\\480p.flv";
#else
	LPCXSTR lpszVideoFile = "480p.264";
	LPCXSTR lpszAudioFile1 = "1.aac";
	LPCXSTR lpszAudioFile2 = "test.aac";
	LPCXSTR lpszDstFile = "480p.mp4";
#endif

	XHANDLE xhAVFile = AVFormat_INPacket_Init(AVPacket_Pack_CBNotify);
	if (NULL == xhAVFile)
	{
		printf("AVFormat_UNPacket_Init:%lX\n", AVFormat_GetLastError());
		return -1;
	}

	if (!AVFormat_INPacket_Output(xhAVFile, lpszDstFile))
	{
		printf("AVFormat_UNPacket_Output:%lX\n", AVFormat_GetLastError());
		return -1;
	}

	if (!AVFormat_INPacket_Input(xhAVFile, lpszAudioFile1))
	{
		printf("AVFormat_UNPacket_Input:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	if (!AVFormat_INPacket_Input(xhAVFile, lpszVideoFile))
	{
		printf("AVFormat_UNPacket_Input:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	/*
	if (!AVFormat_INPacket_Input(xhAVFile, lpszAudioFile2))
	{
		printf("AVFormat_UNPacket_Input:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	*/

	if (!AVFormat_INPacket_Start(xhAVFile))
	{
		printf("AVFormat_UNPacket_Start:%lX\n", AVFormat_GetLastError());
		return -1;
	}

	while (1)
	{
		bool bIsRun = false;
		if (AVFormat_INPacket_GetStatus(xhAVFile, &bIsRun))
		{
			if (!bIsRun)
			{
				break;
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	return AVFormat_INPacket_Stop(xhAVFile);
}

int AVPacket_Test_UNPacket()
{
	int nListCount = 0;
	AVCODEC_FORMATINFO** ppSt_ListFile;

#ifdef _MSC_BUILD
	LPCXSTR lpszVideoFile = "d:\\h264 file\\480p_1.264";
	LPCXSTR lpszAudioFile1 = "d:\\h264 file\\test_1.aac";
	LPCXSTR lpszAudioFile2 = "d:\\h264 file\\test_2.aac";
	LPCXSTR lpszSrcFile = "D:\\windows-ffmpeg\\x64\\1.ts";
#else
	LPCXSTR lpszVideoFile = "480p_1.264";
	LPCXSTR lpszAudioFile1 = "test_1.aac";
	LPCXSTR lpszAudioFile2 = "test_2.aac";
	LPCXSTR lpszSrcFile = "480p.mp4";
#endif

	XHANDLE xhAVFile = AVFormat_UNPacket_Init(AVPacket_Pack_CBNotify);
	if (NULL == xhAVFile)
	{
		printf("AVFormat_UNPacket_Init:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	if (!AVFormat_UNPacket_Input(xhAVFile, lpszSrcFile))
	{
		printf("AVFormat_UNPacket_Input:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	if (!AVFormat_UNPacket_GetList(xhAVFile, &ppSt_ListFile, &nListCount))
	{
		printf("AVFormat_UNPacket_GetList:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	for (int i = 0; i < nListCount; i++)
	{
		printf("%d %d AVTime:%lf\n", ppSt_ListFile[i]->nAVCodecType, ppSt_ListFile[i]->nAVCodecID, ppSt_ListFile[i]->dlAVTime);
	}
	strcpy(ppSt_ListFile[0]->tszFileName, lpszVideoFile);
	//strcpy(ppSt_ListFile[1]->tszFileName, lpszAudioFile1);
	//strcpy(ppSt_ListFile[2]->tszFileName, lpszAudioFile2);

	if (!AVFormat_UNPacket_Output(xhAVFile, &ppSt_ListFile, nListCount))
	{
		printf("AVFormat_UNPacket_Output:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	if (!AVFormat_UNPacket_Start(xhAVFile))
	{
		printf("AVFormat_UNPacket_Start:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	while (1)
	{
		bool bIsRun = false;
		if (AVFormat_UNPacket_GetStatus(xhAVFile, &bIsRun))
		{
			if (!bIsRun)
			{
				break;
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListFile, nListCount);
	return AVFormat_UNPacket_Stop(xhAVFile);
}
int main()
{
	//AVPacket_Test_FileLink();
	AVPacket_Test_FileConvert();
	//AVPacket_Test_FilePacket();
	//AVPacket_Test_UNPacket();

	return 1;
}