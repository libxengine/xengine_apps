#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if 1 == _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemConfig_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemConfig_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_SystemSdk/XEngine_SystemConfig.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_SystemConfig/SystemConfig_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_SystemConfig/SystemConfig_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_SystemConfig.lib")
#endif
#endif

//Linux::g++ -std=c++20 -Wall -g SystemApi_APPFile.cpp -o SystemApi_APPFile.exe -lXEngine_BaseLib -lXEngine_SystemConfig

int profiletest()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszFile = "D:\\xengine_apps\\Debug\\1.txt";
#else
	LPCXSTR lpszFile = "./1.txt";
#endif
	SystemConfig_File_WriteProfileFromFile(lpszFile, "test", "a", "10");
	SystemConfig_File_WriteProfileFromFile(lpszFile, "test", "b", "20");
	SystemConfig_File_WriteProfileFromFile(lpszFile, "test", "c", "30");

	SystemConfig_File_WriteProfileFromFile(lpszFile, "test2", "1", "a");
	SystemConfig_File_WriteProfileFromFile(lpszFile, "test2", "2", "b");
	SystemConfig_File_WriteProfileFromFile(lpszFile, "test2", "3", "c");

	SystemConfig_File_WriteProfileFromFile(lpszFile, "test", "c", "中文");
	SystemConfig_File_WriteProfileFromFile(lpszFile, "test", "d", "4");
	SystemConfig_File_WriteProfileFromFile(lpszFile, "test", "b", "b2");
	SystemConfig_File_WriteProfileFromFile(lpszFile, "test2", "2", "2b");

	XCHAR tszValue[64];
	memset(tszValue, '\0', sizeof(tszValue));

	SystemConfig_File_ReadProfileFromFile(lpszFile, "test", "a", tszValue);
	printf("%s\n", tszValue);
	memset(tszValue, '\0', sizeof(tszValue));

	SystemConfig_File_ReadProfileFromFile(lpszFile, "test", "b", tszValue);
	printf("%s\n", tszValue);
	memset(tszValue, '\0', sizeof(tszValue));

	SystemConfig_File_ReadProfileFromFile(lpszFile, "test", "c", tszValue);
	printf("%s\n", tszValue);
	memset(tszValue, '\0', sizeof(tszValue));

	SystemConfig_File_ReadProfileFromFile(lpszFile, "test", "d", tszValue);
	printf("%s\n", tszValue);
	memset(tszValue, '\0', sizeof(tszValue));

	SystemConfig_File_ReadProfileFromFile(lpszFile, "test2", "1", tszValue);
	printf("%s\n", tszValue);
	memset(tszValue, '\0', sizeof(tszValue));

	SystemConfig_File_ReadProfileFromFile(lpszFile, "test2", "2", tszValue);
	printf("%s\n", tszValue);
	memset(tszValue, '\0', sizeof(tszValue));

	SystemConfig_File_ReadProfileFromFile(lpszFile, "test2", "3", tszValue);
	printf("%s\n", tszValue);
	memset(tszValue, '\0', sizeof(tszValue));
	return 0;
}
int main()
{
	profiletest();
	return 0;
}