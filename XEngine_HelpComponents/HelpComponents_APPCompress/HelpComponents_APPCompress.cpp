#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/HelpComponents_Compress.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_SystemApi.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_ProcFile/ProcFile_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_SystemApi/SystemApi_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_SystemApi/SystemApi_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_HelpComponents/HelpComponents_Compress/HelpCompress_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_HelpComponents/HelpComponents_Compress/HelpCompress_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_HelpComponents/HelpComponents_Compress/HelpCompress_Error.h"

//linux::g++ -std=gnu++17 -Wall -g HelpComponents_APPCompress.cpp -o HelpComponents_APPCompress.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_HelpComponents -lXEngine_BaseLib -lXEngine_SystemApi -lHelpComponents_Compress -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_HelpComponents,--disable-new-dtags
//Macos::g++ -std=gnu++17 -Wall -g HelpComponents_APPCompress.cpp -o HelpComponents_APPCompress.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_SystemSdk -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_HelpComponents -lXEngine_BaseLib -lXEngine_SystemApi -lHelpComponents_Compress

void CompressBuffer_Test()
{
	LPCXSTR lpszBuffer = _X("12345678901234567890123456789012345678901234567890");
	XCHAR tszEnBuffer[2048];
	XCHAR tszDeBuffer[2048];

	memset(tszEnBuffer, '\0', sizeof(tszEnBuffer));
	memset(tszDeBuffer, '\0', sizeof(tszDeBuffer));

	int nLen = strlen(lpszBuffer);
	XLONG dwEnLen = 2048;
	XLONG dwDeLen = 2048;

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
#ifdef _MSC_BUILD
	XZIP xhFile = HelpCompress_File_Create(_X("d:\\xengine_apps\\Debug\\1.zip"));
	HelpCompress_File_Add(xhFile, "XClient_Socket.dll", _X("d:\\xengine_apps\\Debug\\XClient_Socket.dll"));
	HelpCompress_File_Add(xhFile, "XEngine_Authorize.txt", _X("d:\\xengine_apps\\Debug\\XEngine_Authorize.txt"));
	HelpCompress_File_Add(xhFile, "123\\XEngine_WBlackList.dll", _X("d:\\xengine_apps\\Debug\\XEngine_WBlackList.dll"));
	HelpCompress_File_Add(xhFile, "123\\XClient_OPenSsl.dll", _X("d:\\xengine_apps\\Debug\\XClient_OPenSsl.dll"));
	HelpCompress_File_Close(xhFile);
#else
	XZIP xhFile = HelpCompress_File_Create(_X("./1.zip"));
	HelpCompress_File_Add(xhFile, "1.png", _X("./1.png"));
	HelpCompress_File_Add(xhFile, "2.png", _X("./2.png"));
	HelpCompress_File_Add(xhFile, "123\\3.png", _X("./3.png"));
	HelpCompress_File_Close(xhFile);
#endif
}
void UNCompressTest()
{
#ifdef _MSC_BUILD
	XZIP xhFile = HelpCompress_File_OPen(_X("d:\\xengine_apps\\Debug\\1.zip"));
#else
	XZIP xhFile = HelpCompress_File_OPen(_X("./1.zip"));
#endif
	if (NULL == xhFile)
	{
		return;
	}
	int nCount = 0;
	HelpCompress_File_GetCount(xhFile, &nCount);

	bool bRet = HelpCompress_File_GoFirstFile(xhFile);
	while (bRet)
	{
		XCHAR tszExtDir[1024];
		HELPCOMPRESS_ZLIBINFO st_ZLibInfo;

		memset(tszExtDir, '\0', 1024);
		memset(&st_ZLibInfo, '\0', sizeof(HELPCOMPRESS_ZLIBINFO));
		HelpCompress_File_GetInfo(xhFile, &st_ZLibInfo);

		if (st_ZLibInfo.bIsFile)
		{
#ifdef _MSC_BUILD
			sprintf(tszExtDir, _X("d:\\xengine_apps\\Debug\\%s"), st_ZLibInfo.tszFileName);
#else
			sprintf(tszExtDir, _X("./dezip/%s"), st_ZLibInfo.tszFileName);
#endif
			HelpCompress_File_WriteFile(xhFile, tszExtDir);
		}
		else
		{
#ifdef _MSC_BUILD
			sprintf(tszExtDir, _X("d:\\xengine_apps\\Debug\\%s"), st_ZLibInfo.tszFileName);
#else
			sprintf(tszExtDir, _X("./dezip/%s"), st_ZLibInfo.tszFileName);
#endif
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
