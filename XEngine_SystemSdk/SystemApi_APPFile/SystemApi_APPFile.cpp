#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_SystemApi.lib")
#else

#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_ProcFile/ProcFile_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_SystemApi/SystemApi_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_SystemApi/SystemApi_Error.h"

//Linux::g++ -std=c++17 -Wall -g SystemApi_APPFile.cpp -o SystemApi_APPFile.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk -lXEngine_BaseLib -lXEngine_SystemApi -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g SystemApi_APPFile.cpp -o SystemApi_APPFile.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_SystemSdk -lXEngine_BaseLib -lXEngine_SystemApi

BOOL __stdcall EnumFile(LPCXSTR lpFileOrPath, BOOL bFindPath, XPVOID lParam)
{
	if (bFindPath)
	{
		printf(_T("Path %s\r\n"), lpFileOrPath);
	}
	else
	{
		printf(_T("File %s\r\n"), lpFileOrPath);
	}
	return TRUE;
	}

int main()
{
#ifdef _MSC_BUILD
	SystemApi_File_EnumFile("D:\\XEngine\\XEngine_SourceCode\\Debug\\*", NULL, NULL, EnumFile);
#else
	SystemApi_File_EnumFile("/tmp", NULL, NULL, EnumFile);
#endif

#ifdef _MSC_BUILD
	LPCTSTR lpszFile = _T("D:\\xengine_apps\\Debug\\1.txt");
#else
	LPCTSTR lpszFile = _T("1.txt");
#endif

	if (!SystemApi_File_CreateSparseFile(lpszFile, 10240))
	{
		return -1;
	}

	FILE* pSt_FileStart = fopen(lpszFile, _T("rb+"));
	if (NULL == pSt_FileStart)
	{
		return -1;
	}

	FILE* pSt_FileEnd = fopen(lpszFile, _T("rb+"));
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