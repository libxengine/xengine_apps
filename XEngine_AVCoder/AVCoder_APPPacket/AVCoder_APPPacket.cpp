#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVPacket.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AVPacket/AVPacket_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AVPacket/AVPacket_Error.h"

//g++ -std=c++17 -Wall -g AVCoder_APPPacket.cpp -o AVCoder_APPPacket.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCoder -lXEngine_BaseLib -lXEngine_AVPacket -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCoder,--disable-new-dtags

void __stdcall AVPacket_Pack_CBNotify(XNETHANDLE xhNet, int nCvtType, int nCvtFrame, double dlTime, LPVOID lParam)
{
	printf("AVPacket_Pack_CBConvert:%d %d %lf\n", nCvtType, nCvtFrame, dlTime);
}

int AVPacket_Test_FileConvert()
{
	XNETHANDLE xhAVFile = 0;
	double nTotalAVTime = 0;
#ifdef _WINDOWS
	LPCTSTR lpszSrcFile = "H:\\h264 file\\480p.264";
	LPCTSTR lpszDstFile = "H:\\h264 file\\480p.flv";
#else
	LPCTSTR lpszSrcFile = "480p.264";
	LPCTSTR lpszDstFile = "480p.flv";
#endif

	if (!AVPacket_FileConvert_Init(&xhAVFile, AVPacket_Pack_CBNotify))
	{
		printf("AVPacket_FileConvert_Init:%lX\n", AVPacket_GetLastError());
		return -1;
	}
	if (!AVPacket_FileConvert_Input(xhAVFile, lpszSrcFile, &nTotalAVTime))
	{
		printf("AVPacket_FileConvert_Input:%lX\n", AVPacket_GetLastError());
		return -1;
	}
	printf("AVTime:%lf\n", nTotalAVTime);
	if (!AVPacket_FileConvert_Output(xhAVFile, lpszDstFile))
	{
		printf("AVPacket_FileConvert_Output:%lX\n", AVPacket_GetLastError());
		return -1;
	}
	if (!AVPacket_FileConvert_Start(xhAVFile))
	{
		printf("AVPacket_FileConvert_Start:%lX\n", AVPacket_GetLastError());
		return -1;
	}
	while (1)
	{
		BOOL bIsRun = FALSE;
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
	XNETHANDLE xhAVFile = 0;
#ifdef _WINDOWS
	LPCTSTR lpszVideoFile = "H:\\h264 file\\480p.264";
	LPCTSTR lpszAudioFile1 = "H:\\h264 file\\1.aac";
	LPCTSTR lpszAudioFile2 = "H:\\h264 file\\test.aac";
	LPCTSTR lpszDstFile = "H:\\h264 file\\480p.mp4";
#else
	LPCTSTR lpszVideoFile = "480p.264";
	LPCTSTR lpszAudioFile1 = "1.aac";
	LPCTSTR lpszAudioFile2 = "test.aac";
	LPCTSTR lpszDstFile = "480p.mp4";
#endif

	if (!AVPacket_FilePacket_Init(&xhAVFile, AVPacket_Pack_CBNotify))
	{
		printf("AVPacket_FileUNPack_Init:%lX\n", AVPacket_GetLastError());
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
	if (!AVPacket_FilePacket_Input(xhAVFile, lpszAudioFile2))
	{
		printf("AVPacket_FileUNPack_Input:%lX\n", AVPacket_GetLastError());
		return -1;
	}

	if (!AVPacket_FilePacket_Output(xhAVFile, lpszDstFile))
	{
		printf("AVPacket_FileUNPack_Output:%lX\n", AVPacket_GetLastError());
		return -1;
	}
	if (!AVPacket_FilePacket_Start(xhAVFile))
	{
		printf("AVPacket_FileUNPack_Start:%lX\n", AVPacket_GetLastError());
		return -1;
	}

	while (1)
	{
		BOOL bIsRun = FALSE;
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
	XNETHANDLE xhAVFile = 0;
	BOOL bAudio = FALSE;
	BOOL bVideo = FALSE;

	LPCTSTR lpszVideoFile = "H:\\h264 file\\480p.264";
	LPCTSTR lpszAudioFile = "H:\\h264 file\\test.aac";
	LPCTSTR lpszSrcFile = "H:\\h264 file\\480p.mp4";

	if (!AVPacket_FileUNPack_Init(&xhAVFile, AVPacket_Pack_CBNotify))
	{
		printf("AVPacket_FileUNPack_Init:%lX\n", AVPacket_GetLastError());
		return -1;
	}
	double dlAVTime = 0;
	if (!AVPacket_FileUNPack_Input(xhAVFile, lpszSrcFile, &bVideo, &bAudio, &dlAVTime))
	{
		printf("AVPacket_FileUNPack_Input:%lX\n", AVPacket_GetLastError());
		return -1;
	}
	printf("AVTime:%lf\n", dlAVTime);
	if (!AVPacket_FileUNPack_Output(xhAVFile, lpszVideoFile, lpszAudioFile))
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
		BOOL bIsRun = FALSE;
		if (AVPacket_FileUNPack_GetStatus(xhAVFile, &bIsRun))
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
int main()
{
	AVPacket_Test_FileConvert();
	AVPacket_Test_FilePacket();
	//AVPacket_Test_UNPacket();
	
	return 1;
}