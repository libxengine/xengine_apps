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
LPCTSTR lpszFile = _T("D:\\XEngine_Apps\\Debug\\XEngine_AuthorizeReg.CDKey");
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

	SystemApi_HardWare_GetSerial(&st_SDKSerial);

	strcpy(st_AuthRegLocal.tszAddr, _T("127.0.0.1"));
	st_AuthRegLocal.nPort = 5000;

	strcpy(st_AuthRegLocal.st_AuthAppInfo.tszAppName, "XEngine");
	strcpy(st_AuthRegLocal.st_AuthAppInfo.tszAppVer, "7.11.0.1001");

	st_AuthRegLocal.st_AuthRegInfo.nLeftType = 3;
	st_AuthRegLocal.st_AuthRegInfo.nRegType = 1;
	st_AuthRegLocal.st_AuthRegInfo.nHWType = 1;
	strcpy(st_AuthRegLocal.st_AuthRegInfo.tszHardware, st_SDKSerial.tszCpuSerial);

	AuthRegCrypt_Local_BuildKeyTime(&st_AuthRegLocal, 10);

	strcpy(st_AuthRegLocal.st_AuthUserInfo.tszUserName, "XEngine");
	strcpy(st_AuthRegLocal.st_AuthUserInfo.tszUserContact, "486179@qq.com");

	if (!AuthRegCrypt_Local_WriteDatFile(lpszFile, &st_AuthRegLocal, "123123"))
	{
		return;
	}
}
void AuthTest_GetKey()
{
	// TODO: 在此添加控件通知处理程序代码
	__int64x nLeftTime = 0;
	XENGINE_AUTHREGCRYPT_LOCAL st_AuthRegLocal;
	SYSTEMAPI_SERIAL_INFOMATION st_SDKSerial;

	memset(&st_AuthRegLocal, '\0', sizeof(XENGINE_AUTHREGCRYPT_LOCAL));
	memset(&st_SDKSerial, '\0', sizeof(SYSTEMAPI_SERIAL_INFOMATION));

	if (!AuthRegCrypt_Local_ReadDatFile(lpszFile, &st_AuthRegLocal, "123123"))
	{
		return;
	}
	SystemApi_HardWare_GetSerial(&st_SDKSerial);
	if (0 != _tcsncmp(st_SDKSerial.tszCpuSerial, st_AuthRegLocal.st_AuthRegInfo.tszHardware, strlen(st_SDKSerial.tszCpuSerial)))
	{
		printf(_T("序列号不匹配"));
		return;
	}
	if (1 != st_AuthRegLocal.st_AuthRegInfo.nHWType)
	{
		printf(_T("注册硬件序列不匹配"));
		return;
	}
	AuthRegCrypt_Local_GetLeftTimer(&nLeftTime, &st_AuthRegLocal);
	printf("%s = %lld\n", st_AuthRegLocal.st_AuthRegInfo.tszHardware, nLeftTime);

	if (3 == st_AuthRegLocal.st_AuthRegInfo.nLeftType)
	{
		__int64x nTime = _ttoi64(st_AuthRegLocal.st_AuthRegInfo.tszLeftTime) - 1;
		_stprintf(st_AuthRegLocal.st_AuthRegInfo.tszLeftTime, _T("%lld"), nTime);
		if (!AuthRegCrypt_Local_WriteDatFile(lpszFile, &st_AuthRegLocal, "123123"))
		{
			return;
		}
	}

	return;
}

int main(int argc, char** argv)
{
	AuthTest_SetKey();
	AuthTest_GetKey();
	return 0;
}
