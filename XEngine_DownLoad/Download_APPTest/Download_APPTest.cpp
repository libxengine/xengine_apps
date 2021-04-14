#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_DownLoad.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_DownLoad/XEngine_DownLoad/DownLoad_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_DownLoad/XEngine_DownLoad/DownLoad_Error.h"

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
int main()
{
	XNETHANDLE xhDownCall = 0;;
	LPCTSTR lpszHttpAddr = _T("https://dl.softmgr.qq.com/original/im/QQ9.4.6.27770.exe");
	LPCTSTR lpszFileAddr = _T("QQ.exe");

	if (!DownLoad_Http_Create(&xhDownCall, lpszHttpAddr, lpszFileAddr, TRUE, Download_Progress))
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