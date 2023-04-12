#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/StreamMedia_HLSProtocol.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_HLSProtocol/HLSProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_HLSProtocol/HLSProtocol_Error.h"

//Linux::g++ -std=c++17 -Wall -g StreamMedia_APPHls.cpp -o StreamMedia_APPHls.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia -lXEngine_BaseLib -lStreamMedia_HLSProtocol -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g StreamMedia_APPHls.cpp -o StreamMedia_APPHls.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_StreamMedia -lXEngine_BaseLib -lStreamMedia_HLSProtocol

XNETHANDLE xhRoot;
XNETHANDLE xhLow;
XNETHANDLE xhNormal;
XNETHANDLE xhHigh;

void CALLBACK HLSProtocol_CBNotify(XHANDLE xhToken, LPCXSTR lpszFileName, double dlTime, int nIndex, XPVOID lParam)
{
	printf("File:%s,Index:%d,Time:%lf\n", lpszFileName, nIndex, dlTime);
	HLSProtocol_M3u8File_AddFile(xhRoot, xhNormal, lpszFileName, dlTime, XFALSE);
}

int main()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszVideoFile = _T("D:\\h264 file\\480p.264");
#else
	LPCXSTR lpszVideoFile = _T("480p.264");
#endif
	LPCXSTR lpszDstFile = _T("./live/normal/480p-");
	LPCXSTR lpszRootFile = _T("./live/live.m3u8");
	LPCXSTR lpszLowFile = _T("./live/low/live.m3u8");
	LPCXSTR lpszNormalFile = _T("./live/normal/live.m3u8");
	LPCXSTR lpszHighFile = _T("./live/high/live.m3u8");

	if (!HLSProtocol_M3u8File_Create(&xhRoot, lpszRootFile))
	{
		printf("HLSProtocol_M3u8File_Create:%lX\n", HLSProtocol_GetLastError());
		return -1;
	}

	if (!HLSProtocol_M3u8File_AddStream(xhRoot, &xhLow, lpszLowFile, "www.xyry.org/live/low/live.m3u8", 150000))
	{
		printf("HLSProtocol_M3u8File_AddStream:%lX\n", HLSProtocol_GetLastError());
		return -1;
	}
	if (!HLSProtocol_M3u8File_AddStream(xhRoot, &xhNormal, lpszNormalFile, "www.xyry.org/live/normal/live.m3u8", 500000))
	{
		printf("HLSProtocol_M3u8File_AddStream:%lX\n", HLSProtocol_GetLastError());
		return -1;
	}
	if (!HLSProtocol_M3u8File_AddStream(xhRoot, &xhHigh, lpszHighFile, "www.xyry.org/live/high/live.m3u8", 2000000))
	{
		printf("HLSProtocol_M3u8File_AddStream:%lX\n", HLSProtocol_GetLastError());
		return -1;
	}

	XHANDLE xhToken = HLSProtocol_Section_Init(12, XTRUE, XFALSE, HLSProtocol_CBNotify);
	if (NULL == xhToken)
	{
		printf("HLSProtocol_Section_Init:%lX\n", HLSProtocol_GetLastError());
		return -1;
	}
	if (!HLSProtocol_Section_Input(xhToken, lpszVideoFile))
	{
		printf("HLSProtocol_Section_Input:%lX\n", HLSProtocol_GetLastError());
		return -1;
	}
	if (!HLSProtocol_Section_Output(xhToken, lpszDstFile))
	{
		printf("HLSProtocol_Section_Output:%lX\n", HLSProtocol_GetLastError());
		return -1;
	}
	if (!HLSProtocol_Section_Start(xhToken))
	{
		printf("HLSProtocol_Section_Start:%lX\n", HLSProtocol_GetLastError());
		return -1;
	}

	XBOOL bRun = XTRUE;
	while (bRun)
	{
		if (!HLSProtocol_Section_GetStatus(xhToken, &bRun))
		{
			break;
		}
	}
	HLSProtocol_Section_Close(xhToken);
	HLSProtocol_M3u8File_Delete(xhRoot);
	printf("wandan\n");
	return 0;
}