#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_SystemApi.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_ProcFile/ProcFile_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_ProcFile/ProcFile_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_SystemApi/SystemApi_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_SystemApi/SystemApi_Error.h"

//g++ -std=c++17 -Wall -g SystemSdk_APPApi.cpp -o SystemSdk_APPApi.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk -lXEngine_BaseLib -lXEngine_SystemApi -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk,--disable-new-dtags



int Test_SerialInfo()
{
	SYSTEMAPI_SERIAL_INFOMATION st_SDKSerial;
	memset(&st_SDKSerial, '\0', sizeof(SYSTEMAPI_SERIAL_INFOMATION));

	DWORD dwOSVersion = 0;
	DWORD dwOSBuild = 0;
	DWORD dwOSProcessor = 0;
	TCHAR tszOSBuffer[MAX_PATH];
	memset(tszOSBuffer, '\0', MAX_PATH);
	SystemApi_System_GetSystemVer(tszOSBuffer, &dwOSVersion, &dwOSBuild, &dwOSProcessor);

	int nVerHigh = 0;
	int nVerLow = 0;
#ifdef _WINDOWS
	printf("%s %d %d %d\n", tszOSBuffer, dwOSVersion, dwOSBuild, dwOSProcessor);
#else
	printf("%s %d.%d %ld %ld\n", tszOSBuffer, nVerHigh, nVerLow, dwOSBuild, dwOSProcessor);
#endif

#ifndef _WINDOWS
	strcpy(st_SDKSerial.tszDiskSerial, "sda");
#endif
	if (!SystemApi_HardWare_GetSerial(&st_SDKSerial))
	{
		return -1;
	}
	printf("Test_SerialInfo:%s %s %s %s\n", st_SDKSerial.tszBoardSerial, st_SDKSerial.tszSystemSerail, st_SDKSerial.tszCpuSerial, st_SDKSerial.tszDiskSerial);
	return 0;
}
int Test_DiskInfo()
{
	int nDiskCount = 0;
	TCHAR** ppszRootName;
	SystemApi_HardWare_GetDiskNumber(&ppszRootName, &nDiskCount);
	for (int i = 0; i < nDiskCount; i++)
	{
		SYSTEMAPI_DISK_INFOMATION st_DiskInfo;
		memset(&st_DiskInfo, '\0', sizeof(SYSTEMAPI_DISK_INFOMATION));

#ifdef _WINDOWS
		SystemApi_HardWare_GetDiskInfomation(ppszRootName[i], &st_DiskInfo);
#else
		SystemApi_HardWare_GetDiskInfomation("/", &st_DiskInfo);
#endif
		printf("Test_DiskInfo:%s %llu\n", ppszRootName[i], st_DiskInfo.dwDiskTotal);
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppszRootName, nDiskCount);

	return 0;
}
int Test_CPUInfo()
{
	SYSTEMAPI_CPU_INFOMATION st_CPUInfo;
	memset(&st_CPUInfo, '\0', sizeof(SYSTEMAPI_CPU_INFOMATION));
	SystemApi_HardWare_GetCpuInfomation(&st_CPUInfo);

	printf("Test_CPUInfo:%s %d %d\n", st_CPUInfo.tszCpuName, st_CPUInfo.nCpuNumber, st_CPUInfo.nCpuSpeed);
	return 0;
}
int Test_ProcessInfo()
{
	int nUsage = 0;
	XENGINE_LIBTIMER st_LibTimer;
	SYSTEMAPI_PROCESS_INFOMATION st_ProcessInfo;

	memset(&st_LibTimer, '\0', sizeof(XENGINE_LIBTIMER));
	memset(&st_ProcessInfo, '\0', sizeof(SYSTEMAPI_PROCESS_INFOMATION));

	SystemApi_Process_GetProcessInfo(&st_ProcessInfo);
	SystemApi_Process_GetProcessCpuUsage(&nUsage);
	SystemApi_Process_GetUpTime(&st_LibTimer);

	printf("Test_ProcessInfo:%d %s %s %d %d\n", st_ProcessInfo.nPid, st_ProcessInfo.tszAppName, st_ProcessInfo.tszAppUser, st_ProcessInfo.nThreadCount, st_ProcessInfo.st_MemoryInfo.nUsePhysicalMemory);
	printf("Test_ProcessInfo:%d %04d-%02d-%02d %02d:%02d:%02d\n", nUsage, st_LibTimer.wYear, st_LibTimer.wMonth, st_LibTimer.wDay, st_LibTimer.wHour, st_LibTimer.wMinute, st_LibTimer.wSecond);
	return 0;
}
int Test_SystemInfo()
{
	int nUsage = 0;
	int nCPUCount = 0;
	int nProcessCount = 0;
	XENGINE_LIBTIMER st_LibTimer;
	SYSTEMAPI_MEMORY_INFOMATION st_MemoryInfo;

	memset(&st_LibTimer, '\0', sizeof(XENGINE_LIBTIMER));
	memset(&st_MemoryInfo, '\0', sizeof(SYSTEMAPI_MEMORY_INFOMATION));

	SystemApi_System_GetMemoryUsage(&st_MemoryInfo);
	SystemApi_System_GetCpuUsage(&nUsage);
	SystemApi_System_GetCpuCount(&nCPUCount);
	SystemApi_System_GetProcessCount(&nProcessCount);
	SystemApi_System_GetUpTime(&st_LibTimer);

	printf("Test_SystemInfo:%llu %llu %d\n", st_MemoryInfo.dwMemory_Total, st_MemoryInfo.dwMemory_Free, st_MemoryInfo.nMemoryUsage);
	printf("Test_SystemInfo:%d %d %d %d %04d-%02d-%02d %02d:%02d:%02d\n", nUsage, nCPUCount, nCPUCount, nProcessCount, st_LibTimer.wYear, st_LibTimer.wMonth, st_LibTimer.wDay, st_LibTimer.wHour, st_LibTimer.wMinute, st_LibTimer.wSecond);
	return 0;
}

int main()
{
	Test_SerialInfo();
	Test_DiskInfo();
	Test_CPUInfo();
	Test_ProcessInfo();
	Test_SystemInfo();
	return 0;
}