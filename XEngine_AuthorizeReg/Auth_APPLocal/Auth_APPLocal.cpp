#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_SystemApi.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AuthRegCrypt.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <inttypes.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_ProcSdk/ProcFile_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_SystemApi/SystemApi_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_SystemApi/SystemApi_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AuthorizeReg/XEngine_AuthRegCrypt/AuthRegCrypt_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AuthorizeReg/XEngine_AuthRegCrypt/AuthRegCrypt_Error.h"

//g++ -std=c++17 -Wall -g Auth_APPLocal.cpp -o Auth_APPLocal.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AuthorizeReg -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk -lXEngine_BaseLib -lXEngine_SystemApi -lXEngine_AuthRegCrypt -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AuthorizeReg,--disable-new-dtags

#ifdef _WINDOWS
LPCTSTR lpszFile = _T("H:\\XEngine_Apps\\Debug\\XEngine_AuthorizeReg.CDKey");
#else
LPCTSTR lpszFile = _T("XEngine_AuthorizeReg.CDKey");
#endif

void AuthTest_SetKey()
{
	// TODO: 在此添加控件通知处理程序代码
	XENGINE_AUTHREGCRYPT_LOCAL st_AuthRegLocal;
	SYSTEMAPI_SERIAL_INFOMATION st_SDKSerial;

	memset(&st_AuthRegLocal, '\0', sizeof(XENGINE_AUTHREGCRYPT_LOCAL));
	memset(&st_SDKSerial, '\0', sizeof(SYSTEMAPI_SERIAL_INFOMATION));

	strcpy(st_AuthRegLocal.tszAddr, _T("127.0.0.1"));
	strcpy(st_AuthRegLocal.tszPort, "1111");
	strcpy(st_AuthRegLocal.st_AuthAppInfo.tszAppName, "血与荣誉网络通信引擎");
	strcpy(st_AuthRegLocal.st_AuthAppInfo.tszAppVer, "7.2.0.1001");

	st_AuthRegLocal.st_AuthRegInfo.nRegType = 1;
	st_AuthRegLocal.st_AuthRegInfo.nHardwareType = 1;

	SystemApi_HardWare_GetSerial(&st_SDKSerial);
	AuthRegCrypt_Local_BuildKeyTime(&st_AuthRegLocal, 10);

	strcpy(st_AuthRegLocal.st_AuthRegInfo.tszHardware, st_SDKSerial.tszCpuSerial);

	if (!AuthRegCrypt_Local_WriteDatFile(lpszFile, &st_AuthRegLocal, "123123"))
	{
		return;
	}
}
void AuthTest_GetKey()
{
	// TODO: 在此添加控件通知处理程序代码
	__int64 nLeftTime = 0;
	XENGINE_AUTHREGCRYPT_LOCAL st_AuthRegLocal;
	SYSTEMAPI_SERIAL_INFOMATION st_SDKSerial;

	memset(&st_AuthRegLocal, '\0', sizeof(XENGINE_AUTHREGCRYPT_LOCAL));
	memset(&st_SDKSerial, '\0', sizeof(SYSTEMAPI_SERIAL_INFOMATION));

	SystemApi_HardWare_GetSerial(&st_SDKSerial);

	if (!AuthRegCrypt_Local_ReadDatFile(lpszFile, &st_AuthRegLocal, "123123"))
	{
		return;
	}
	if (0 != _tcsncmp(st_SDKSerial.tszCpuSerial, st_AuthRegLocal.st_AuthRegInfo.tszHardware, strlen(st_SDKSerial.tszCpuSerial)))
	{
		printf(_T("序列号不匹配"));
		return;
	}
	if (1 != st_AuthRegLocal.st_AuthRegInfo.nHardwareType)
	{
		printf(_T("注册硬件序列不匹配"));
		return;
	}
	AuthRegCrypt_Local_GetLeftTimer(&nLeftTime, &st_AuthRegLocal, 2);
	printf("%s = %" PRId64"\n", st_AuthRegLocal.st_AuthRegInfo.tszHardware, nLeftTime);
	return;
}

int main(int argc, char** argv)
{
	AuthTest_SetKey();
	AuthTest_GetKey();
	return 0;
}
