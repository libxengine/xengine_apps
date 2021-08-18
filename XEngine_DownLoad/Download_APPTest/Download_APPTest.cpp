#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_DownLoad.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_SystemApi.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_DownLoad/XEngine_DownLoad/DownLoad_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_DownLoad/XEngine_DownLoad/DownLoad_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_ProcSdk/ProcFile_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_SystemApi/SystemApi_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_SystemApi/SystemApi_Error.h"

//g++ -std=c++17 -Wall -g Download_APPTest.cpp -o Download_APPTest.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_DownLoad -lXEngine_BaseLib -lXEngine_DownLoad -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_DownLoad,--disable-new-dtags

bool bRun = false;
void __stdcall Download_Progress(XNETHANDLE xhDown, double dlTotal, double dlNow, double ulTotal, double ulNow, ENUM_DOWNENGINE_STATUS en_DownHttpStatus, LPVOID lParam)
{
	printf("下载任务：%llu,总大小：%lf，已经下载大小：%lf，下载标识符：%d\n", xhDown, dlTotal, dlNow, en_DownHttpStatus);

	if (ENUM_XENGINE_DOWNLOAD_STATUS_COMPLETE == en_DownHttpStatus)
	{
		bRun = false;
	}
}
int download_http()
{
	XNETHANDLE xhDownCall = 0;;
	LPCTSTR lpszHttpAddr = _T("https://dl.softmgr.qq.com/original/im/QQ9.4.9.27849.exe");
	//LPCTSTR lpszHttpAddr = _T("http://192.168.1.7:5101/QQ.exe");
#ifdef _WINDOWS
	LPCTSTR lpszFileAddr = _T("D:\\xengine_apps\\Debug\\QQ.exe");
#else
	LPCTSTR lpszFileAddr = _T("QQ.exe");
#endif
	/*
	if (!SystemApi_File_CreateSparseFile(lpszFileAddr, 92582984))
	{
		return -1;
	}*/
	if (!DownLoad_Http_Create(&xhDownCall, lpszHttpAddr, lpszFileAddr, _T("0-"), Download_Progress))
	{
		printf("下载失败！");
		return -1;
	}
	bRun = TRUE;

	while (bRun)
	{
		
	}
	DownLoad_Http_Delete(xhDownCall);
	return 0;
}
int upload_http()
{
	XNETHANDLE xhUPLoad = 0;;
	LPCTSTR lpszHttpAddr = _T("http://192.168.1.7:5102/QQ.exe");
#ifdef _WINDOWS
	LPCTSTR lpszFileAddr = _T("D:\\xengine_apps\\Debug\\QQ.exe");
#else
	LPCTSTR lpszFileAddr = _T("QQ.exe");
#endif

	if (!DownLoad_Http_Create(&xhUPLoad, lpszHttpAddr, lpszFileAddr, NULL, NULL, NULL, "PUT"))
	{
		printf("下载失败！");
		return -1;
	}

	while (1)
	{
		NETDOWNLOAD_TASKINFO st_TaskInfo;
		memset(&st_TaskInfo, '\0', sizeof(NETDOWNLOAD_TASKINFO));

		if (!DownLoad_Http_Query(xhUPLoad, &st_TaskInfo))
		{
			break;
		}
		if (ENUM_XENGINE_DOWNLOAD_STATUS_COMPLETE == st_TaskInfo.en_DownStatus)
		{
			break;
		}
		printf("上传任务：%llu,总大小：%lf，已经上传大小：%lf，标识符：%d\n", xhUPLoad, st_TaskInfo.ulTotal, st_TaskInfo.ulNow, st_TaskInfo.en_DownStatus);
		Sleep(100);
	}
	DownLoad_Http_Delete(xhUPLoad);
	return 0;
}

int main()
{
	upload_http();
	return 0;
}