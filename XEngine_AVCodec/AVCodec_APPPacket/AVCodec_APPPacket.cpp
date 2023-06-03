#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVPacket.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVPacket/AVPacket_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVPacket/AVPacket_Error.h"

//Linux::g++ -std=c++17 -Wall -g AVCodec_APPPacket.cpp -o AVCodec_APPPacket.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCodec -lXEngine_BaseLib -lXEngine_AVPacket -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCodec,--disable-new-dtags
//MacOS::g++ -std=c++17 -Wall -g AVCodec_APPPacket.cpp -o AVCodec_APPPacket.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_AVCodec -lXEngine_BaseLib -lXEngine_AVPacket

void CALLBACK AVPacket_Pack_CBNotify(XHANDLE xhNet, int nCvtType, int nCvtFrame, double dlTime, XPVOID lParam)
{
	printf("AVPacket_Pack_CBConvert:%d %d %lf\n", nCvtType, nCvtFrame, dlTime);
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

	XHANDLE xhAVFile = AVPacket_FileLink_Init(AVPacket_Pack_CBNotify);
	if (NULL == xhAVFile)
	{
		printf("AVPacket_FileConvert_Init:%lX\n", AVPacket_GetLastError());
		return -1;
	}
	if (!AVPacket_FileLink_Output(xhAVFile, lpszDstFile))
	{
		printf("AVPacket_FileConvert_Output:%lX\n", AVPacket_GetLastError());
		return -1;
	}
	if (!AVPacket_FileLink_Input(xhAVFile, lpszSrcFile1))
	{
		printf("AVPacket_FileConvert_Input:%lX\n", AVPacket_GetLastError());
		return -1;
	}
	if (!AVPacket_FileLink_Input(xhAVFile, lpszSrcFile2))
	{
		printf("AVPacket_FileConvert_Input:%lX\n", AVPacket_GetLastError());
		return -1;
	}
	if (!AVPacket_FileLink_Input(xhAVFile, lpszSrcFile3))
	{
		printf("AVPacket_FileConvert_Input:%lX\n", AVPacket_GetLastError());
		return -1;
	}

	
	if (!AVPacket_FileLink_Start(xhAVFile))
	{
		printf("AVPacket_FileConvert_Start:%lX\n", AVPacket_GetLastError());
		return -1;
	}
	while (1)
	{
		bool bIsRun = false;
		if (AVPacket_FileLink_GetStatus(xhAVFile, &bIsRun))
		{
			if (!bIsRun)
			{
				break;
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	return AVPacket_FileLink_Stop(xhAVFile);
}

int AVPacket_Test_FileConvert()
{
	double nTotalAVTime = 0;
#ifdef _MSC_BUILD
	LPCXSTR lpszSrcFile = "D:\\h264 file\\480p.flv";
	LPCXSTR lpszDstFile = "D:\\h264 file\\480p.mp4";
#else
	LPCXSTR lpszSrcFile = "480p.flv";
	LPCXSTR lpszDstFile = "conv.mp4";
#endif

	XHANDLE xhAVFile = AVPacket_FileConvert_Init(AVPacket_Pack_CBNotify);
	if (NULL == xhAVFile)
	{
		printf("AVPacket_FileConvert_Init:%lX\n", AVPacket_GetLastError());
		return -1;
	}
	if (!AVPacket_FileConvert_Output(xhAVFile, lpszDstFile))
	{
		printf("AVPacket_FileConvert_Output:%lX\n", AVPacket_GetLastError());
		return -1;
	}

	if (!AVPacket_FileConvert_Input(xhAVFile, lpszSrcFile, &nTotalAVTime))
	{
		printf("AVPacket_FileConvert_Input:%lX\n", AVPacket_GetLastError());
		return -1;
	}
	printf("AVTime:%lf\n", nTotalAVTime);
	
	if (!AVPacket_FileConvert_Start(xhAVFile))
	{
		printf("AVPacket_FileConvert_Start:%lX\n", AVPacket_GetLastError());
		return -1;
	}
	while (1)
	{
		bool bIsRun = false;
		if (AVPacket_FileConvert_GetStatus(xhAVFile, &bIsRun))
		{
			if (!bIsRun)
			{
				break;
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	return AVPacket_FileConvert_Stop(xhAVFile);
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

	XHANDLE xhAVFile = AVPacket_FilePacket_Init(AVPacket_Pack_CBNotify);
	if (NULL == xhAVFile)
	{
		printf("AVPacket_FileUNPack_Init:%lX\n", AVPacket_GetLastError());
		return -1;
	}

	if (!AVPacket_FilePacket_Output(xhAVFile, lpszDstFile))
	{
		printf("AVPacket_FileUNPack_Output:%lX\n", AVPacket_GetLastError());
		return -1;
	}
	if (!AVPacket_FilePacket_Input(xhAVFile, lpszVideoFile))
	{
		printf("AVPacket_FileUNPack_Input:%lX\n", AVPacket_GetLastError());
		return -1;
	}
	if (!AVPacket_FilePacket_Input(xhAVFile, lpszAudioFile1))
	{
		printf("AVPacket_FileUNPack_Input:%lX\n", AVPacket_GetLastError());
		return -1;
	}
	/*
	if (!AVPacket_FilePacket_Input(xhAVFile, lpszAudioFile2))
	{
		printf("AVPacket_FileUNPack_Input:%lX\n", AVPacket_GetLastError());
		return -1;
	}
	*/
	
	if (!AVPacket_FilePacket_Start(xhAVFile))
	{
		printf("AVPacket_FileUNPack_Start:%lX\n", AVPacket_GetLastError());
		return -1;
	}

	while (1)
	{
		bool bIsRun = false;
		if (AVPacket_FilePacket_GetStatus(xhAVFile, &bIsRun))
		{
			if (!bIsRun)
			{
				break;
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	return AVPacket_FilePacket_Stop(xhAVFile);
}

int AVPacket_Test_UNPacket()
{
	int nListCount = 0;
	AVCODEC_PACKETLIST** ppSt_ListFile;

#ifdef _MSC_BUILD
	LPCXSTR lpszVideoFile = "d:\\h264 file\\480p_1.264";
	LPCXSTR lpszAudioFile1 = "d:\\h264 file\\test_1.aac";
	LPCXSTR lpszAudioFile2 = "d:\\h264 file\\test_2.aac";
	LPCXSTR lpszSrcFile = "d:\\h264 file\\480p.mp4";
#else
	LPCXSTR lpszVideoFile = "480p_1.264";
	LPCXSTR lpszAudioFile1 = "test_1.aac";
	LPCXSTR lpszAudioFile2 = "test_2.aac";
	LPCXSTR lpszSrcFile = "480p.mp4";
#endif

	XHANDLE xhAVFile = AVPacket_FileUNPack_Init(AVPacket_Pack_CBNotify);
	if (NULL == xhAVFile)
	{
		printf("AVPacket_FileUNPack_Init:%lX\n", AVPacket_GetLastError());
		return -1;
	}
	if (!AVPacket_FileUNPack_Input(xhAVFile, lpszSrcFile))
	{
		printf("AVPacket_FileUNPack_Input:%lX\n", AVPacket_GetLastError());
		return -1;
	}
	if (!AVPacket_FileUNPack_GetList(xhAVFile, &ppSt_ListFile, &nListCount))
	{
		printf("AVPacket_FileUNPack_GetList:%lX\n", AVPacket_GetLastError());
		return -1;
	}
	for (int i = 0; i < nListCount; i++)
	{
		printf("%d %d AVTime:%lf\n", ppSt_ListFile[i]->nAVCodecType, ppSt_ListFile[i]->nAVCodecID, ppSt_ListFile[i]->dlAVTime);
	}
	strcpy(ppSt_ListFile[0]->tszFileName, lpszVideoFile);
	strcpy(ppSt_ListFile[1]->tszFileName, lpszAudioFile1);
	strcpy(ppSt_ListFile[2]->tszFileName, lpszAudioFile2);

	if (!AVPacket_FileUNPack_Output(xhAVFile, &ppSt_ListFile, nListCount))
	{
		printf("AVPacket_FileUNPack_Output:%lX\n", AVPacket_GetLastError());
		return -1;
	}
	if (!AVPacket_FileUNPack_Start(xhAVFile))
	{
		printf("AVPacket_FileUNPack_Start:%lX\n", AVPacket_GetLastError());
		return -1;
	}
	while (1)
	{
		bool bIsRun = false;
		if (AVPacket_FileUNPack_GetStatus(xhAVFile, &bIsRun))
		{
			if (!bIsRun)
			{
				break;
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListFile, nListCount);
	return AVPacket_FileUNPack_Stop(xhAVFile);
}
int main()
{
	//AVPacket_Test_FileLink();
	//AVPacket_Test_FileConvert();
	AVPacket_Test_FilePacket();
	//AVPacket_Test_UNPacket();

	return 1;
}