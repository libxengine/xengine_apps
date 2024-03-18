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
#include <XEngine_Include/XEngine_SystemSdk/ProcFile_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Define.h>
#include <XEngine_Include/XEngine_SystemSdk/SystemApi_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_SystemSdk/XEngine_SystemApi.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_ProcFile/ProcFile_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_SystemApi/SystemApi_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_SystemApi/SystemApi_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_SystemApi.lib")
#endif
#endif

//Linux::g++ -std=c++17 -Wall -g SystemApi_APPFile.cpp -o SystemApi_APPFile.exe -lXEngine_BaseLib -lXEngine_SystemApi

bool CALLBACK EnumFile(LPCXSTR lpFileOrPath, bool bFindPath, XPVOID lParam)
{
	if (bFindPath)
	{
		printf(_X("Path %s\r\n"), lpFileOrPath);
	}
	else
	{
		printf(_X("File %s\r\n"), lpFileOrPath);
	}
	return true;
	}

int main()
{
	int nCount = 0;
	XCHAR** ppszListDir;
	SYSTEMAPI_FILE_ATTR st_FileAttr = {};
	SystemApi_File_GetFileAttr(_X("D:\\xengine_apps"), &st_FileAttr);

#ifdef _MSC_BUILD
	SystemApi_File_EnumFile("D:\\test\\*", &ppszListDir, &nCount, true, 1);
#else
	SystemApi_File_EnumFile("/tmp", &ppszListDir, &nCount);
#endif

	for (int i = 0; i < nCount; i++)
	{
		printf("%s\n", ppszListDir[i]);
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppszListDir, nCount);

#ifdef _MSC_BUILD
	LPCXSTR lpszFile = _X("D:\\xengine_apps\\Debug\\1.txt");
#else
	LPCXSTR lpszFile = _X("1.txt");
#endif

	if (!SystemApi_File_CreateSparseFile(lpszFile, 10240))
	{
		return -1;
	}

	FILE* pSt_FileStart = fopen(lpszFile, _X("rb+"));
	if (NULL == pSt_FileStart)
	{
		return -1;
	}

	FILE* pSt_FileEnd = fopen(lpszFile, _X("rb+"));
	if (NULL == pSt_FileEnd)
	{
		return -1;
	}
	fseek(pSt_FileEnd, 5, SEEK_CUR);

	int nRet = fwrite("01234", 1, 5, pSt_FileStart);
	nRet = fwrite("56789", 1, 5, pSt_FileEnd);

	fclose(pSt_FileStart);
	fclose(pSt_FileEnd);
	return 0;
}