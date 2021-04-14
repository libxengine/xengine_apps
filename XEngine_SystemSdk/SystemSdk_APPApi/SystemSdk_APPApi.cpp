#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_SystemApi.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_ProcSdk/ProcFile_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_SystemApi/SystemApi_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_SystemApi/SystemApi_Error.h"

//g++ -std=c++17 -Wall -g SystemSdk_APPApi.cpp -o SystemSdk_APPApi.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk -lXEngine_BaseLib -lXEngine_SystemApi -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk,--disable-new-dtags

BOOL __stdcall EnumFile(LPCSTR lpFileOrPath, BOOL bFindPath, LPVOID lParam)
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
	SYSTEMAPI_SERIAL_INFOMATION st_SDKSerial;
	memset(&st_SDKSerial, '\0', sizeof(SYSTEMAPI_SERIAL_INFOMATION));

	DWORD dwOSVersion = 0;
	DWORD dwOSBuild = 0;
	DWORD dwOSProcessor = 0;
	TCHAR tszOSBuffer[MAX_PATH];
	memset(tszOSBuffer, '\0', MAX_PATH);
	SystemApi_System_GetSystemVer(tszOSBuffer, &dwOSVersion, &dwOSBuild, &dwOSProcessor);

#ifndef _WINDOWS
	strcpy(st_SDKSerial.tszMacAddress, "ens33");
	strcpy(st_SDKSerial.tszDiskSerial, "/dev/sda1");
#endif
	if (!SystemApi_HardWare_GetSerial(&st_SDKSerial))
	{
		return -1;
	}
	printf("%s %s %s %s %s\n", st_SDKSerial.tszBaseBoardSerial, st_SDKSerial.tszBiosSerail, st_SDKSerial.tszCpuSerial, st_SDKSerial.tszDiskSerial, st_SDKSerial.tszMacAddress);
	SYSTEMAPI_PROCESS_INFOMATION st_ProcessInfo;
	memset(&st_ProcessInfo, '\0', sizeof(SYSTEMAPI_PROCESS_INFOMATION));

	SystemApi_Process_GetProcessInfo(&st_ProcessInfo);
	printf("%d %s %s %d %d\n", st_ProcessInfo.nPid, st_ProcessInfo.tszAppName, st_ProcessInfo.tszAppUser, st_ProcessInfo.nThreadCount, st_ProcessInfo.st_MemoryInfo.nUsePhysicalMemory);

	SystemApi_File_EnumFile("G:\\", NULL, NULL, EnumFile);
	return 0;
}