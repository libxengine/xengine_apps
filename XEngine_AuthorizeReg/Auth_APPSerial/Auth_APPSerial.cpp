#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AuthGenerateSerial.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <inttypes.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AuthorizeReg/XEngine_AuthGenerateSerial/AuthGenerateSerial_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AuthorizeReg/XEngine_AuthGenerateSerial/AuthGenerateSerial_Error.h"

//g++ -std=c++17 -Wall -g Auth_APPSerial.cpp -o Auth_APPSerial.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AuthorizeReg -lXEngine_BaseLib -lXEngine_AuthGenerateSerial -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AuthorizeReg,--disable-new-dtags

int main()
{
	XNETHANDLE xhToken;
	XENGINE_LIBTIMER st_LibTimer;
	memset(&st_LibTimer, '\0', sizeof(XENGINE_LIBTIMER));

	st_LibTimer.wFlags = 10;

	if (!XEngine_AuthGenerateSerial_StartEx(&xhToken, "XAUTH", 3, 8, &st_LibTimer, ENUM_XENGINE_AUTHREG_GENERATESERIAL_TYPE_TIME))
	{
		printf("error\n");
		return -1;
	}
	while (1)
	{
		TCHAR tszSerial[1024];
		memset(tszSerial, '\0', sizeof(tszSerial));
		if (!XEngine_AuthGenerateSerial_GetEx(xhToken, tszSerial))
		{
			break;
		}
		ENUM_AUTHREG_GENERATESERIALTYPE enSerialType;
		memset(&st_LibTimer, '\0', sizeof(XENGINE_LIBTIMER));

		XEngine_AuthGenerateSerial_GetTypeEx(xhToken, tszSerial, &enSerialType, &st_LibTimer);
		printf("%s:Time:%d\n", tszSerial, st_LibTimer.wFlags);
	}
	XEngine_AuthGenerateSerial_CloseEx(xhToken);

	memset(&st_LibTimer, '\0', sizeof(XENGINE_LIBTIMER));
	st_LibTimer.wYear = 2021;
	st_LibTimer.wMonth = 2;
	st_LibTimer.wDay = 20;
	st_LibTimer.wHour = 12;
	st_LibTimer.wMinute = 10;
	st_LibTimer.wSecond = 0;

	if (!XEngine_AuthGenerateSerial_StartEx(&xhToken, "XAUTH", 3, 8, &st_LibTimer, ENUM_XENGINE_AUTHREG_GENERATESERIAL_TYPE_CUSTOM))
	{
		printf("error\n");
		return -1;
	}
	while (1)
	{
		TCHAR tszSerial[1024];
		memset(tszSerial, '\0', sizeof(tszSerial));
		if (!XEngine_AuthGenerateSerial_GetEx(xhToken, tszSerial))
		{
			break;
		}
		ENUM_AUTHREG_GENERATESERIALTYPE enSerialType;
		memset(&st_LibTimer, '\0', sizeof(XENGINE_LIBTIMER));

		XEngine_AuthGenerateSerial_GetTypeEx(xhToken, tszSerial, &enSerialType, &st_LibTimer);
		printf("%s:Time:%04d-%02d-%02d %02d:%02d:%02d\n", tszSerial, st_LibTimer.wYear, st_LibTimer.wMonth, st_LibTimer.wDay, st_LibTimer.wHour, st_LibTimer.wMinute, st_LibTimer.wSecond);
	}
	XEngine_AuthGenerateSerial_CloseEx(xhToken);
	return 0;
}