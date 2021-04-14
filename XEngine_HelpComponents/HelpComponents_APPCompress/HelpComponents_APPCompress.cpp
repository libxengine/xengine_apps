#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/HelpComponents_Compress.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_SystemApi.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_ProcSdk/ProcFile_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_SystemApi/SystemApi_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_SystemApi/SystemApi_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_HelpComponents/HelpComponents_Compress/HelpCompress_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_HelpComponents/HelpComponents_Compress/HelpCompress_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_HelpComponents/HelpComponents_Compress/HelpCompress_Error.h"

//g++ -std=gnu++17 -Wall -g HelpComponents_APPCompress.cpp -o HelpComponents_APPCompress.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_HelpComponents -lXEngine_BaseLib -lXEngine_SystemApi -lHelpComponents_Compress -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_HelpComponents,--disable-new-dtags

void CompressBuffer_Test()
{
	LPCTSTR lpszBuffer = _T("12345678901234567890123456789012345678901234567890");
	TCHAR tszEnBuffer[2048];
	TCHAR tszDeBuffer[2048];

	memset(tszEnBuffer, '\0', sizeof(tszEnBuffer));
	memset(tszDeBuffer, '\0', sizeof(tszDeBuffer));

	int nLen = strlen(lpszBuffer);
	DWORD dwEnLen = 2048;
	DWORD dwDeLen = 2048;

	if (!HelpCompress_Memory_Compress(lpszBuffer, nLen, tszEnBuffer, &dwEnLen, 4))
	{
		printf("%lX\n", HelpCompress_GetLastError());
		return;
	}
	printf("HelpCompress_Memory_Compress:%ld = %s\n", dwEnLen,tszEnBuffer);
	if (!HelpCompress_Memory_UnCompress(tszEnBuffer, dwEnLen, tszDeBuffer, &dwDeLen))
	{
		printf("%lX\n", HelpCompress_GetLastError());
		return;
	}
	printf("HelpCompress_Memory_UnCompress:%ld = %s\n", dwDeLen, tszDeBuffer);
}
void CompressFile_Test()
{
	XZIP xhFile = HelpCompress_File_Create(_T("H:\\XEngine_Apps\\Debug\\1.zip"));
	HelpCompress_File_Add(xhFile, "XClient_Socket.dll", _T("H:\\XEngine_Apps\\Debug\\XClient_Socket.dll"));
	HelpCompress_File_Add(xhFile, "zlibwapi.dll", _T("H:\\XEngine_Apps\\Debug\\zlibwapi.dll"));
	HelpCompress_File_Add(xhFile, "123\\XEngine_WBlackList.dll", _T("H:\\XEngine_Apps\\Debug\\XEngine_WBlackList.dll"));
	HelpCompress_File_Add(xhFile, "123\\XClient_OPenSsl.dll", _T("H:\\XEngine_Apps\\Debug\\XClient_OPenSsl.dll"));
	HelpCompress_File_Close(xhFile);
}
void UNCompressTest()
{
	XZIP xhFile = HelpCompress_File_OPen(_T("H:\\XEngine_Apps\\Debug\\1.zip"));
	if (NULL == xhFile)
	{
		return;
	}
	int nCount = 0;
	HelpCompress_File_GetCount(xhFile, &nCount);

	BOOL bRet = HelpCompress_File_GoFirstFile(xhFile);
	while (bRet)
	{
		TCHAR tszExtDir[1024];
		HELPCOMPRESS_ZLIBINFO st_ZLibInfo;

		memset(tszExtDir, '\0', 1024);
		memset(&st_ZLibInfo, '\0', sizeof(HELPCOMPRESS_ZLIBINFO));
		HelpCompress_File_GetInfo(xhFile, &st_ZLibInfo);

		if (st_ZLibInfo.bIsFile)
		{
			sprintf(tszExtDir, _T("H:\\XEngine_Apps\\Debug\\%s"), st_ZLibInfo.tszFileName);
			HelpCompress_File_WriteFile(xhFile, tszExtDir);
		}
		else
		{
			sprintf(tszExtDir, _T("H:\\XEngine_Apps\\Debug\\%s"), st_ZLibInfo.tszFileName);
			SystemApi_File_CreateMutilFolder(tszExtDir);
		}
		bRet = HelpCompress_File_GoNextFile(xhFile);
	}
	HelpCompress_File_UNClose(xhFile);
}


int main()
{
	CompressBuffer_Test();
	CompressFile_Test();
	UNCompressTest();
	return 0;
}
