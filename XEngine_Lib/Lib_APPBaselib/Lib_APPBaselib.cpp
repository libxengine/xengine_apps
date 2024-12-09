#include "../../XEngine_ExampleHdr.h"
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
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#endif
#endif

//Linux MacOS:g++ -std=c++20 -Wall -g Lib_APPBaselib.cpp -o Lib_APPBaselib.exe -lXEngine_BaseLib


void EventTest()
{
	XEVENT hEvent = BaseLib_Event_Create();
	if (NULL == hEvent)
	{
		return;
	}
	if (!BaseLib_Event_Wait(hEvent))
	{
		return;
	}
	BaseLib_Event_Delete(hEvent);
}
void TimeSpanTest()
{
	__int64x nDayTTime = 0;
	__int64x nHourTTime = 0;
	LPCXSTR lpszStartTime = _X("2019-11-01 12:31:10");
	LPCXSTR lpszEndTime = _X("2020-10-02 12:31:10");
	XENGINE_LIBTIMER st_LibTimer;
	memset(&st_LibTimer, '\0', sizeof(XENGINE_LIBTIMER));

	BaseLib_TimeSpan_GetForStr(lpszStartTime, lpszEndTime, &nDayTTime, 0);
	BaseLib_TimeSpan_GetForStr(lpszStartTime, lpszEndTime, &nHourTTime, 1);
	_xtprintf(_X("\n跨了：%lld天 %lld小时\n"), nDayTTime, nHourTTime);

	BaseLib_TimeSpan_CalForStr(lpszStartTime, lpszEndTime, &st_LibTimer, true);
	_xtprintf(_X("%d/%d/%d-%d:%d:%d\n"), st_LibTimer.wYear, st_LibTimer.wMonth, st_LibTimer.wDay, st_LibTimer.wHour, st_LibTimer.wMinute, st_LibTimer.wSecond);

	BaseLib_TimeSpan_CalForStr(lpszStartTime, lpszEndTime, &st_LibTimer, false);
	_xtprintf(_X("%d/%d/%d-%d:%d:%d\n"), st_LibTimer.wYear, st_LibTimer.wMonth, st_LibTimer.wDay, st_LibTimer.wHour, st_LibTimer.wMinute, st_LibTimer.wSecond);
}
void TimeTest()
{
	time_t nTime = 2706010596;
	XENGINE_LIBTIMER st_LibTimer;
	memset(&st_LibTimer, '\0', sizeof(XENGINE_LIBTIMER));

	BaseLib_Time_GetSysTime(&st_LibTimer);
	BaseLib_Time_TTimeToStuTime(nTime, &st_LibTimer);

	__int64x nTTime = 0;
	XCHAR tszTimeStr[128] = {0};
	LPCXSTR lpszTimeStr = _X("2022-08-01 10:22:01");
	BaseLib_Time_StrToInt(lpszTimeStr, &nTTime);
	BaseLib_Time_IntToStr(nTTime, tszTimeStr);

	BaseLib_Time_GetSysTime(&st_LibTimer);
	BaseLib_Time_TimezoneCvt(&st_LibTimer, 4);
	return;
}
void StringTest()
{
	int nHdrLen = 0;
	int nBodyLen = 0;
	XCHAR tszKey[64];
	XCHAR tszValue[64];
	XCHAR tszMsgBuffer[1024];

	memset(tszKey, '\0', sizeof(tszKey));
	memset(tszValue, '\0', sizeof(tszValue));
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	_tcsxcpy(tszMsgBuffer, _X("123456789 : abcd"));

	BaseLib_String_DelSub(tszMsgBuffer, _X("9"));

	BaseLib_String_GetKeyValue(tszMsgBuffer, _X(":"), tszKey, tszValue, true, &nHdrLen, &nBodyLen);
	LPCXSTR lpszFile1 = _X("./adadad/file.txt");
	LPCXSTR lpszFile2 = _X("D:\\adadad\\file.txt");
	LPCXSTR lpszFile3 = _X("./file.txt");
	LPCXSTR lpszFile4 = _X("file.txt");

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	int nLen = _xstprintf(tszMsgBuffer, "123456abc123765cd123ddd321");

	memset(tszValue, '\0', sizeof(tszValue));
	BaseLib_String_GetStartEnd(tszMsgBuffer, tszValue, NULL, "321", false);
	BaseLib_String_Change(tszMsgBuffer, &nLen, "ccccc", "123", "321", false);

	XCHAR tszFileDir[MAX_PATH];
	XCHAR tszFileName[MAX_PATH];
	XCHAR tszFileDrive[MAX_PATH];
	XCHAR tszFileExt[MAX_PATH];

	memset(tszFileDir, '\0', MAX_PATH);
	memset(tszFileName, '\0', MAX_PATH);
	memset(tszFileDrive, '\0', MAX_PATH);
	memset(tszFileExt, '\0', MAX_PATH);
	BaseLib_String_GetFileAndPath(lpszFile1, tszFileDir, tszFileName, tszFileDrive, tszFileExt);

	memset(tszFileDir, '\0', MAX_PATH);
	memset(tszFileName, '\0', MAX_PATH);
	memset(tszFileDrive, '\0', MAX_PATH);
	memset(tszFileExt, '\0', MAX_PATH);
	BaseLib_String_GetFileAndPath(lpszFile2, tszFileDir, tszFileName, tszFileDrive, tszFileExt);

	memset(tszFileDir, '\0', MAX_PATH);
	memset(tszFileName, '\0', MAX_PATH);
	BaseLib_String_GetFileAndPath(lpszFile3, tszFileDir, tszFileName);

	memset(tszFileDir, '\0', MAX_PATH);
	memset(tszFileName, '\0', MAX_PATH);
	BaseLib_String_GetFileAndPath(lpszFile4, tszFileDir, tszFileName);

	XCHAR tszFileStr[MAX_PATH] = {};
	BaseLib_String_GetSeparatorStr("123456/789/abc/ddd", "/", tszFileStr, 2, true);

	BaseLib_String_GetSeparatorStr("123456/789/abc/ddd", "/", tszFileStr, 1, false);
	return;
}

void GMTTimeTest()
{
	XCHAR tszGMTTime[128];
	memset(tszGMTTime, '\0', sizeof(tszGMTTime));
	BaseLib_Time_GMTTime(tszGMTTime);

	_xtprintf(_X("%s\n"), tszGMTTime);
}


int test_Mutex()
{
	LPCXSTR lpszName = _X("xyry");
	XEVENT xhEvent = BaseLib_Semaphore_Create(lpszName);
	if (NULL == xhEvent)
	{
		_xtprintf(_X("失败\n"));
		return 0;
	}

	if (BaseLib_Semaphore_IsExist(lpszName))
	{
		_xtprintf(_X("存在\n"));
	}
	else
	{
		_xtprintf(_X("不存在\n"));
	}
	BaseLib_Semaphore_Delete(xhEvent);
	return 0;
}
int test_Memory()
{
	int** ppIntArray;
	if (!BaseLib_Memory_Malloc((void***)&ppIntArray, 3, sizeof(int)))
	{
		return -1;
	}
	for (int i = 0; i < 3; i++)
	{
		*(ppIntArray[i]) = i;
	}
	for (int i = 0; i < 3; i++)
	{
		_xtprintf(_X("%d\n"), *(ppIntArray[i]));
	}
	BaseLib_Memory_Free((void***)&ppIntArray, 3);

	XCHAR** ppszStr;
	if (!BaseLib_Memory_Malloc((void***)&ppszStr, 3, sizeof(XCHAR) * 6))
	{
		return -1;
	}
	for (int i = 0; i < 3; i++)
	{
		_tcsxcpy(ppszStr[i], _X("hello"));
	}
	for (int i = 0; i < 3; i++)
	{
		_xtprintf(_X("%s\n"), ppszStr[i]);
	}
	BaseLib_Memory_Free((void***)&ppszStr, 3);

	typedef struct
	{
		int a;
		int b;
	}XENGINE_MEMORY;
	XENGINE_MEMORY** ppSt_Memory;
	BaseLib_Memory_Malloc((XPPPMEM)&ppSt_Memory, 3, sizeof(XENGINE_MEMORY));
	for (int i = 0; i < 3; i++)
	{
		ppSt_Memory[i]->a = 1;
		ppSt_Memory[i]->b = i;
	}
	for (int i = 0; i < 3; i++)
	{
		_xtprintf(_X("%d %d\n"), ppSt_Memory[i]->a, ppSt_Memory[i]->b);
	}
	BaseLib_Memory_Free((void***)&ppSt_Memory, 3);
	return 0;
}
int test_TTrigger()
{
	XHANDLE xhToken;
	if (!BaseLib_TTigger_Create(&xhToken))
	{
		return -1;
	}
	if (!BaseLib_TTigger_Set(xhToken, 1))
	{
		return -2;
	}

	__int64x nTTimer = 0;
	BaseLib_TTigger_Get(xhToken, 1, &nTTimer);

	BaseLib_TTigger_Destory(xhToken);
	return 0;
}

void Test_GetTimeofday()
{
	XENGINE_VALTIME st_Timeval;
	XENGINE_LIBTIMER st_LibTime;
	XCHAR tszMsgTimer[128];

	memset(&st_Timeval, '\0', sizeof(XENGINE_VALTIME));
	memset(&st_LibTime, '\0', sizeof(XENGINE_LIBTIMER));
	memset(tszMsgTimer, '\0', sizeof(tszMsgTimer));

	BaseLib_Time_GetTimeOfday(&st_Timeval);
	BaseLib_Time_TTimeToStuTime(st_Timeval.tv_sec, &st_LibTime);
	BaseLib_Time_TimeToStr(tszMsgTimer, NULL, NULL, &st_LibTime);

#ifdef _MSC_BUILD
	_xtprintf(_X("%lld %lld %llu %llu  %s\n"), time(NULL), st_Timeval.tv_sec, st_Timeval.tv_value, st_Timeval.tv_usec, tszMsgTimer);
#else
	_xtprintf("%ld %ld %llu %llu  %s\n", time(NULL), st_Timeval.tv_sec, st_Timeval.tv_value, st_Timeval.tv_usec, tszMsgTimer);
#endif
}
int test_handle()
{
	for (int i = 0; i < 10; i++)
	{
		XNETHANDLE xhToken = 0;
		BaseLib_Handle_Create(&xhToken);
		_xtprintf(_X("%lld\n"), xhToken);
	}
	for (int i = 0; i < 4; i++)
	{
		XCHAR tszKeyStr[64] = {};
		BaseLib_Handle_CreateStr(tszKeyStr, 6, 0, 1);
		printf("%s\n", tszKeyStr);
	}

	return 0;
}

int Test_Ver()
{
	printf("%lld\n%s\n", BaseLib_Version_XNumberInt(), BaseLib_Version_XNumberStr());
	printf("%s\n", BaseLib_Version_BuildTime());
	return 0;
}
int main()
{
	setlocale(LC_ALL, "");

	Test_Ver();
	test_handle();
	StringTest();
	Test_GetTimeofday();
	test_TTrigger();
	test_Memory();
	test_Mutex();
	TimeSpanTest();
	TimeTest();
	GMTTimeTest();
	
	return 0;
}