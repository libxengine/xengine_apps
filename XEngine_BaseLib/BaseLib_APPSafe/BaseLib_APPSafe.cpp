#include "../../XEngine_ExampleHdr.h"
#define _CRT_SECURE_NO_WARNINGS
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <locale.h>
#if 1 == _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_BaseLib/BaseSafe_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseSafe_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseSafe.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseSafe/BaseSafe_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseSafe/BaseSafe_Error.h"
#ifdef _MSC_BUILD
#ifdef _WIN64
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_BaseSafe.lib")
#else
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseSafe.lib")
#endif
#endif
#endif

//Linux MacOS:g++ -std=c++20 -Wall -g Lib_APPSafe.cpp -o Lib_APPSafe.exe -lXEngine_BaseSafe

// 引入不同平台所需的头文件
#if defined(_WIN32) || defined(_WIN64)
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#endif

void SystemApi_OpenFileSecure(const char* lpszFileName, const char* mode)
{
	FILE* pSt_File = BaseSafe_File_CreateA(lpszFileName, mode);
	fwrite("123", 1, 3, pSt_File);
	fclose(pSt_File);
}

int main()
{
	SystemApi_OpenFileSecure("D:\\xengine_apps\\1.txt", "wb");

	XCHAR tszDestStr[8] = {};
	LPCXSTR lpszSourceStr = _X("123456789");

	_xstrcpy(tszDestStr, lpszSourceStr, sizeof(tszDestStr));
	printf("%s\n", tszDestStr);
	return 0;
}