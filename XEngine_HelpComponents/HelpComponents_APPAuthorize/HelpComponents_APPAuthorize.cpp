#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_OPenSsl.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_SystemApi.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/HelpComponents_Authorize.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <inttypes.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_OPenSsl/OPenSsl_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_OPenSsl/OPenSsl_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_ProcSdk/ProcFile_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_SystemApi/SystemApi_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_SystemApi/SystemApi_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_HelpComponents/HelpComponents_Authorize/Authorize_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_HelpComponents/HelpComponents_Authorize/Authorize_Error.h"

//g++ -std=c++17 -Wall -g HelpComponents_APPAuthorize.cpp -o HelpComponents_APPAuthorize.exe -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_Core -L /usr/local/lib/XEngine_Release/XEngine_SystemSdk -L /usr/local/lib/XEngine_Release/XEngine_HelpComponents -lXEngine_BaseLib -lXEngine_OPenSsl -lXEngine_SystemApi -lHelpComponents_Authorize

int Authorize_APPSerial()
{
	int nCardCount = 5;
	XENGINE_LIBTIMER st_LibTimer;
	memset(&st_LibTimer, '\0', sizeof(XENGINE_LIBTIMER));

	st_LibTimer.wFlags = 10;

	TCHAR** pptszSerial;

	if (!Authorize_Serial_Creator(&pptszSerial, "XAUTH", nCardCount, 8, &st_LibTimer, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_TIME))
	{
		printf("error\n");
		return -1;
	}
	for (int i = 0; i < nCardCount; i++)
	{
		printf("%s Time:%d\n", pptszSerial[i], st_LibTimer.wFlags);
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&pptszSerial, nCardCount);

	memset(&st_LibTimer, '\0', sizeof(XENGINE_LIBTIMER));
	st_LibTimer.wYear = 2022;
	st_LibTimer.wMonth = 2;
	st_LibTimer.wDay = 20;
	st_LibTimer.wHour = 12;
	st_LibTimer.wMinute = 10;
	st_LibTimer.wSecond = 0;

	if (!Authorize_Serial_Creator(&pptszSerial, "XAUTH", nCardCount, 8, &st_LibTimer, ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_CUSTOM))
	{
		printf("error\n");
		return -1;
	}
	for (int i = 0; i < nCardCount; i++)
	{
		ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE enSerialType;
		memset(&st_LibTimer, '\0', sizeof(XENGINE_LIBTIMER));

		Authorize_Serial_GetType(pptszSerial[i], &enSerialType, &st_LibTimer);
		printf("%s:Time:%04d-%02d-%02d %02d:%02d:%02d\n", pptszSerial[i], st_LibTimer.wYear, st_LibTimer.wMonth, st_LibTimer.wDay, st_LibTimer.wHour, st_LibTimer.wMinute, st_LibTimer.wSecond);
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&pptszSerial, nCardCount);
	return 0;
}

#ifdef _WINDOWS
LPCTSTR lpszFile = _T("D:\\XEngine_Apps\\Debug\\XEngine_Authorize.CDKey");
#else
LPCTSTR lpszFile = _T("XEngine_Authorize.CDKey");
#endif
int Authorize_APPLocal()
{
	//////////////////////////////////////////////////////////////////////////生成CDKEY
	__int64x nLeftTime = 0;
	UCHAR tszEnBuffer[2048];
	TCHAR tszDeBuffer[2048];
	XENGINE_AUTHORIZE_LOCAL st_AuthLocal;
	SYSTEMAPI_SERIAL_INFOMATION st_SDKSerial;

	memset(tszDeBuffer, '\0', sizeof(tszDeBuffer));
	memset(tszEnBuffer, '\0', sizeof(tszEnBuffer));
	memset(&st_AuthLocal, '\0', sizeof(XENGINE_AUTHORIZE_LOCAL));
	memset(&st_SDKSerial, '\0', sizeof(SYSTEMAPI_SERIAL_INFOMATION));

	strcpy(st_AuthLocal.tszAddr, _T("127.0.0.1"));
	st_AuthLocal.nPort = 5000;

	strcpy(st_AuthLocal.st_AuthAppInfo.tszAppName, "XEngine");
	strcpy(st_AuthLocal.st_AuthAppInfo.tszAppVer, "7.17.0.1001");

	st_AuthLocal.st_AuthRegInfo.enSerialType = ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_TIME;
	st_AuthLocal.st_AuthRegInfo.enRegType = ENUM_HELPCOMPONENTS_AUTHORIZE_REG_TYPE_TRY;
	st_AuthLocal.st_AuthRegInfo.enHWType = ENUM_HELPCOMPONENTS_AUTHORIZE_HW_TYPE_CPU;
	SystemApi_HardWare_GetSerial(&st_SDKSerial);
	strcpy(st_AuthLocal.st_AuthRegInfo.tszHardware, st_SDKSerial.tszCpuSerial);

	strcpy(st_AuthLocal.st_AuthUserInfo.tszUserName, "XEngine");
	strcpy(st_AuthLocal.st_AuthUserInfo.tszUserContact, "486179@qq.com");

	Authorize_Local_BuildKeyTime(&st_AuthLocal, 10);
	if (!Authorize_Local_WriteKey(lpszFile, &st_AuthLocal))
	{
		return -1;
	}
	//需要加密?
	FILE* pSt_File = fopen(lpszFile, "rb");
	int nRet = fread(tszDeBuffer, 1, sizeof(tszDeBuffer), pSt_File);
	fclose(pSt_File);
	if (!OPenSsl_Api_CryptEncodec(tszDeBuffer, tszEnBuffer, &nRet, "123123"))
	{
		return -1;
	}
	pSt_File = fopen(lpszFile, "wb");
	fwrite(tszEnBuffer, 1, nRet, pSt_File);
	fclose(pSt_File);
	//////////////////////////////////////////////////////////////////////////验证CDKEY
	memset(tszDeBuffer, '\0', sizeof(tszDeBuffer));
	memset(tszEnBuffer, '\0', sizeof(tszEnBuffer));
	memset(&st_AuthLocal, '\0', sizeof(XENGINE_AUTHORIZE_LOCAL));
	memset(&st_SDKSerial, '\0', sizeof(SYSTEMAPI_SERIAL_INFOMATION));
	//需要解密?
	pSt_File = fopen(lpszFile, "rb");
	nRet = fread(tszEnBuffer, 1, sizeof(tszEnBuffer), pSt_File);
	fclose(pSt_File);
	if (!OPenSsl_Api_CryptDecodec(tszEnBuffer, tszDeBuffer, &nRet, "123123"))
	{
		return -1;
	}
	pSt_File = fopen(lpszFile, "wb");
	fwrite(tszDeBuffer, 1, nRet, pSt_File);
	fclose(pSt_File);

	if (!Authorize_Local_ReadKey(lpszFile, &st_AuthLocal))
	{
		return -1;
	}
	SystemApi_HardWare_GetSerial(&st_SDKSerial);
	if (0 != strncmp(st_SDKSerial.tszCpuSerial, st_AuthLocal.st_AuthRegInfo.tszHardware, strlen(st_SDKSerial.tszCpuSerial)))
	{
		printf(_T("序列号不匹配"));
		return -1;
	}
	if (ENUM_HELPCOMPONENTS_AUTHORIZE_HW_TYPE_CPU != st_AuthLocal.st_AuthRegInfo.enHWType)
	{
		printf(_T("注册硬件序列不匹配"));
		return -1;
	}
	Authorize_Local_GetLeftTimer(&nLeftTime, &st_AuthLocal);
	printf("%s = %lld\n", st_AuthLocal.st_AuthRegInfo.tszHardware, nLeftTime);

	if (ENUM_HELPCOMPONENTS_AUTHORIZE_SERIAL_TYPE_TIME == st_AuthLocal.st_AuthRegInfo.enSerialType)
	{
		__int64x nTime = atoll(st_AuthLocal.st_AuthRegInfo.tszLeftTime) - 1;
		sprintf(st_AuthLocal.st_AuthRegInfo.tszLeftTime, _T("%lld"), nTime);
		if (!Authorize_Local_WriteKey(lpszFile, &st_AuthLocal))
		{
			return -1;
		}
	}
	return 0;
}


int main()
{
	Authorize_APPSerial();
	Authorize_APPLocal();
	return 0;
}