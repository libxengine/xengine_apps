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
#include <XEngine_Include/XEngine_SystemSdk/ProcFile_Error.h>
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
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_ProcFile/ProcFile_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_SystemApi/SystemApi_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_SystemApi/SystemApi_Error.h"
#ifdef _MSC_BUILD
#ifdef _WIN64
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_SystemApi.lib")
#else
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_SystemApi.lib")
#endif
#endif
#endif

//Linux::g++ -std=c++20 -Wall -g SystemSdk_APPApi.cpp -o SystemSdk_APPApi.exe -lXEngine_BaseLib -lXEngine_SystemApi

int Test_SerialInfo()
{
	SYSTEMAPI_SERIAL_INFOMATION st_SDKSerial;
	memset(&st_SDKSerial, '\0', sizeof(SYSTEMAPI_SERIAL_INFOMATION));

	XLONG dwOSProcessor = 0;
	XCHAR tszOSBuffer[XPATH_MAX];
	XCHAR tszOSVersion[XPATH_MAX];
	XCHAR tszOSVBuild[XPATH_MAX];

	memset(tszOSBuffer, '\0', XPATH_MAX);
	memset(tszOSVersion, '\0', XPATH_MAX);
	memset(tszOSVBuild, '\0', XPATH_MAX);

	SystemApi_System_GetSystemVer(tszOSBuffer, tszOSVersion, tszOSVBuild, &dwOSProcessor);
	printf("%s %s %s %lu\n", tszOSBuffer, tszOSVersion, tszOSVBuild, dwOSProcessor);

#ifndef _MSC_BUILD
#ifdef __linux__
	strcpy(st_SDKSerial.tszDiskSerial, "sda");
#else
	strcpy(st_SDKSerial.tszDiskSerial, "macos");
#endif
#endif
	if (!SystemApi_HardWare_GetSerial(&st_SDKSerial))
	{
		return -1;
	}
	printf("Test_SerialInfo:%s %s %s %s\n", st_SDKSerial.tszBoardSerial, st_SDKSerial.tszSystemSerial, st_SDKSerial.tszCPUSerial, st_SDKSerial.tszDiskSerial);
	return 0;
}
int Test_DiskInfo()
{
	int nDiskCount = 0;
	XCHAR** ppszRootName;
	SystemApi_HardWare_GetDiskNumber(&ppszRootName, &nDiskCount);
	for (int i = 0; i < nDiskCount; i++)
	{
		SYSTEMAPI_DISK_INFOMATION st_DiskInfo;
		memset(&st_DiskInfo, '\0', sizeof(SYSTEMAPI_DISK_INFOMATION));

#ifdef _MSC_BUILD
		SystemApi_HardWare_GetDiskInfomation(ppszRootName[i], &st_DiskInfo);
#else
		SystemApi_HardWare_GetDiskInfomation("/", &st_DiskInfo);
#endif
		printf("Test_DiskInfo:%s %llu\n", ppszRootName[i], st_DiskInfo.dwDiskTotal);
	}
	BaseLib_Memory_Free((XPPPMEM)&ppszRootName, nDiskCount);

	return 0;
}
int Test_CPUInfo()
{
	SYSTEMAPI_CPU_INFOMATION st_CPUInfo;
	memset(&st_CPUInfo, '\0', sizeof(SYSTEMAPI_CPU_INFOMATION));
	SystemApi_HardWare_GetCpuInfomation(&st_CPUInfo);

	printf("Test_CPUInfo:%s %d %d %d\n", st_CPUInfo.tszCPUName, st_CPUInfo.nCPUNumber, st_CPUInfo.nCPUThread, st_CPUInfo.nCPUSpeed);
	return 0;
}
int Test_ProcessInfo()
{
	int nUsage = 0;
	XENGINE_LIBTIME st_LibTimer;
	SYSTEMAPI_PROCESS_INFOMATION st_ProcessInfo;

	memset(&st_LibTimer, '\0', sizeof(XENGINE_LIBTIME));
	memset(&st_ProcessInfo, '\0', sizeof(SYSTEMAPI_PROCESS_INFOMATION));

	SystemApi_Process_GetProcessInfo(&st_ProcessInfo);
	SystemApi_Process_GetProcessCpuUsage(&nUsage);
	SystemApi_Process_GetUpTime(&st_LibTimer);

	printf("Test_ProcessInfo:%d %s %s %d %d\n", st_ProcessInfo.nPid, st_ProcessInfo.tszAPPName, st_ProcessInfo.tszAPPUser, st_ProcessInfo.nThreadCount, st_ProcessInfo.st_MemoryInfo.nUsePhysicalMemory);
	printf("Test_ProcessInfo:%d %04d-%02d-%02d %02d:%02d:%02d\n", nUsage, st_LibTimer.wYear, st_LibTimer.wMonth, st_LibTimer.wDay, st_LibTimer.wHour, st_LibTimer.wMinute, st_LibTimer.wSecond);
	return 0;
}
int Test_SystemInfo()
{
	int nUsage = 0;
	int nProcessCount = 0;
	XENGINE_LIBTIME st_LibTimer;
	SYSTEMAPI_MEMORY_INFOMATION st_MemoryInfo;

	memset(&st_LibTimer, '\0', sizeof(XENGINE_LIBTIME));
	memset(&st_MemoryInfo, '\0', sizeof(SYSTEMAPI_MEMORY_INFOMATION));

	SystemApi_System_GetMemoryUsage(&st_MemoryInfo, XENGINE_SYSTEMSDK_API_SYSTEM_SIZE_MB);
	SystemApi_System_GetCpuUsage(&nUsage);
	SystemApi_System_GetProcessCount(&nProcessCount);
	SystemApi_System_GetUpTime(&st_LibTimer);

#ifdef _MSC_BUILD
	XCHAR tszValueStr[128] = {};
	SystemApi_System_WMIQuery("SELECT * FROM Win32_Processor", "MaxClockSpeed", tszValueStr, 1);
#endif
	printf("Test_SystemInfo:%llu %llu %d\n", st_MemoryInfo.dwMemory_Total, st_MemoryInfo.dwMemory_Free, st_MemoryInfo.nMemoryUsage);
	printf("Test_SystemInfo:%d %d %04d-%02d-%02d %02d:%02d:%02d\n", nUsage, nProcessCount, st_LibTimer.wYear, st_LibTimer.wMonth, st_LibTimer.wDay, st_LibTimer.wHour, st_LibTimer.wMinute, st_LibTimer.wSecond);
	return 0;
}
int Test_FileInfo()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszDir = _X("D:\\xengine_apps\\Debug\\stroage\\123123\\abb");
#else
	LPCXSTR lpszDir = _X("./stroage/123123/abb");
#endif

	SystemApi_File_CreateMutilFolder(lpszDir);
	SystemApi_File_CreateMutilFolder(lpszDir);
	
#ifdef _MSC_BUILD
	SystemApi_File_DeleteMutilFolder("D:\\xengine_apps\\Debug\\stroage\\");
#else
	SystemApi_File_DeleteMutilFolder("./stroage/");
#endif

	return 0;
}

int Test_EnumFile()
{
	int nCount = 0;
	XCHAR** ppszListDir;
	SYSTEMAPI_FILE_ATTR st_FileAttr = {};
	SystemApi_File_GetFileAttr(_X("D:\\xengine_apps\\x64"), &st_FileAttr);

#ifdef _MSC_BUILD
	SystemApi_File_EnumFile("D:\\xengine_apps\\x64", &ppszListDir, &nCount, true, 1);
#else
	SystemApi_File_EnumFile("/tmp/", &ppszListDir, &nCount);
#endif

	for (int i = 0; i < nCount; i++)
	{
		printf("%s\n", ppszListDir[i]);
	}
	BaseLib_Memory_Free((XPPPMEM)&ppszListDir, nCount);

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
int main()
{
	Test_FileInfo();
	Test_SerialInfo();
	Test_DiskInfo();
	Test_CPUInfo();
	Test_ProcessInfo();
	Test_SystemInfo();
	Test_EnumFile();
	return 0;
}