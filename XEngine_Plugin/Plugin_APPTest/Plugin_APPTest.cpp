#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_PluginCore.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_PluginInfo.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Plugin/XEngine_PluginCore/PluginCore_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Plugin/XEngine_PluginCore/PluginCore_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Plugin/XEngine_PluginInfo/PluginInfo_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Plugin/XEngine_PluginInfo/PluginInfo_Error.h"

//g++ -std=c++17 -Wall -g Plugin_APPTest.cpp -o Plugin_APPTest.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Plugin -lXEngine_BaseLib -lXEngine_PluginCore -lXEngine_PluginInfo -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Plugin,--disable-new-dtags

BOOL bRun = TRUE;
XNETHANDLE xhToken;
void CALLBACK PluginCore_Search_Callback(LPCSTR lpszFile, LPVOID lParam)
{
	if (PluginCore_FrameWork_Push(&xhToken, lpszFile))
	{
		bRun = FALSE;
		printf(_T("添加插件模块:%s 成功!\n"), lpszFile);
	}
	else
	{
		bRun = FALSE;
		printf(_T("添加插件模块:%s 失败!\n"), lpszFile);
	}
}


int main()
{
	PluginCore_FrameWork_Init();
#ifdef _WINDOWS
	PluginCore_Search_Start("H:\\XEngine_Apps\\Debug\\", PluginCore_Search_Callback);
#else
	PluginCore_Search_Start("../Plugin_Module/", PluginCore_Search_Callback);
#endif
	
	while (bRun)
	{
	}
	PluginCore_FrameWork_Exec(xhToken);
	PluginCore_FrameWork_AddFunc(xhToken, "PluginCore_Print");
	PluginCore_FrameWork_ExecFunc(xhToken, "PluginCore_Print", (LPVOID)"PluginCore_Print");

	PluginCore_FrameWork_Del(xhToken);
	PluginCore_FrameWork_Destroy();
	return 0;
}