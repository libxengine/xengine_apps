#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_DownLoad.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_DownLoad/XEngine_DownLoad/DownLoad_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_DownLoad/XEngine_DownLoad/DownLoad_Error.h"

//Linux::g++ -std=c++17 -Wall -g Download_APPTest.cpp -o Download_APPTest.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_DownLoad -lXEngine_BaseLib -lXEngine_DownLoad -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_DownLoad,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g Download_APPTest.cpp -o Download_APPTest.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_DownLoad -lXEngine_BaseLib -lXEngine_DownLoad 

bool bRun = false;
void __stdcall Download_Progress(XHANDLE xhToken, double dlTotal, double dlNow, double ulTotal, double ulNow, ENUM_DOWNENGINE_STATUS en_DownHttpStatus, LPVOID lParam)
{
	printf("下载任务：%p,总大小：%lf，已经下载大小：%lf，下载标识符：%d\n", xhToken, dlTotal, dlNow, en_DownHttpStatus);

	if (ENUM_XENGINE_DOWNLOAD_STATUS_COMPLETE == en_DownHttpStatus)
	{
		bRun = false;
	}
}
int download_http()
{
	LPCTSTR lpszHttpAddr = _T("https://webcdn.m.qq.com/spcmgr/download/QQ9.6.1.28732.exe");
	//LPCTSTR lpszHttpAddr = _T("http://192.168.1.7:5101/QQ.exe");
#ifdef _WINDOWS
	LPCTSTR lpszFileAddr = _T("D:\\xengine_apps\\Debug\\QQ.exe");
#else
	LPCTSTR lpszFileAddr = _T("QQ.exe");
#endif

	XHANDLE xhDownCall = DownLoad_Http_Create(lpszHttpAddr, lpszFileAddr, NULL, Download_Progress);
	if (NULL == xhDownCall)
	{
		printf("下载失败！");
		return -1;
	}
	bRun = TRUE;

	while (bRun)
	{
		NETDOWNLOAD_TASKINFO st_TaskInfo;
		memset(&st_TaskInfo, '\0', sizeof(NETDOWNLOAD_TASKINFO));

		if (!DownLoad_Http_Query(xhDownCall, &st_TaskInfo))
		{
			break;
		}
		if (ENUM_XENGINE_DOWNLOAD_STATUS_COMPLETE == st_TaskInfo.en_DownStatus)
		{
			break;
		}
	}
	DownLoad_Http_Delete(xhDownCall);
	return 0;
}
int upload_http()
{
	LPCTSTR lpszHttpAddr = _T("http://192.168.1.7:5102/QQ.exe");
#ifdef _WINDOWS
	LPCTSTR lpszFileAddr = _T("D:\\xengine_apps\\Debug\\QQ.exe");
#else
	LPCTSTR lpszFileAddr = _T("QQ.exe");
#endif

	XHANDLE xhUPLoad = DownLoad_Http_Create(lpszHttpAddr, lpszFileAddr, NULL, NULL, NULL, "PUT");
	if (NULL == xhUPLoad)
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
		printf("上传任务：%p,总大小：%lf，已经上传大小：%lf，标识符：%d\n", xhUPLoad, st_TaskInfo.ulTotal, st_TaskInfo.ulNow, st_TaskInfo.en_DownStatus);
	}
	DownLoad_Http_Delete(xhUPLoad);
	return 0;
}

int main()
{
	//upload_http();
	download_http();
	return 0;
}