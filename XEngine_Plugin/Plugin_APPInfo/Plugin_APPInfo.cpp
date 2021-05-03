#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_PluginInfo.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Plugin/XEngine_PluginInfo/PluginInfo_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Plugin/XEngine_PluginInfo/PluginInfo_Error.h"

//g++ -std=c++17 -Wall -g Plugin_APPTest.cpp -o Plugin_APPTest.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Plugin -lXEngine_BaseLib -lXEngine_PluginCore -lXEngine_PluginInfo -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Plugin,--disable-new-dtags


int main()
{
#ifdef _WINDOWS
	LPCTSTR lpszFile = _T("H:\\XEngine_Apps\\Debug\\list.txt");
#else
	LPCTSTR lpszFile = _T("list.txt");
#endif

	XNETHANDLE xhToken;
	PluginInfo_ListCreator_Init(&xhToken);

	PluginInfo_ListCreator_AddModule(xhToken, "./FileModule1.dll");
	PluginInfo_ListCreator_AddModule(xhToken, "./FileModule2.dll");

	PluginInfo_ListCreator_AddFunction(xhToken, "./FileModule1.dll", "funadd");
	PluginInfo_ListCreator_AddFunction(xhToken, "./FileModule2.dll", "funadd");
	PluginInfo_ListCreator_AddFunction(xhToken, "./FileModule2.dll", "fundel");

	int nSize = 0;
	CHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	PluginInfo_ListCreator_Get(xhToken, tszMsgBuffer, &nSize);
	PluginInfo_ListCreator_WriteFile(xhToken, lpszFile);
	printf("%s\n", tszMsgBuffer);
	PluginInfo_ListCreator_Destroy(xhToken);

	int nListCount = 0;
	PluginInfo_ListParse_Init(&xhToken, lpszFile);
	PluginInfo_ListParse_ModuleCount(xhToken, &nListCount);
	printf("PluginInfo_ListParse_ModuleCount:%d\n", nListCount);

	nListCount = 0;
	PluginInfo_ListParse_FunctionCount(xhToken, "./FileModule2.dll", &nListCount);
	printf("PluginInfo_ListParse_FunctionCount:%d\n", nListCount);

	nListCount = 0;
	TCHAR** ppszListFile;
	PluginInfo_ListParse_GetModule(xhToken, &ppszListFile, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		printf("%s\n", ppszListFile[i]);
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppszListFile, nListCount);

	nListCount = 0;
	PluginInfo_ListParse_GetFunction(xhToken, "./FileModule2.dll", &ppszListFile, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		printf("%s\n", ppszListFile[i]);
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppszListFile, nListCount);
	PluginInfo_ListParse_Destroy(xhToken);
	return 0;
}