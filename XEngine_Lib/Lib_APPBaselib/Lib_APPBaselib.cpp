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

//Linux MacOS:g++ -std=c++17 -Wall -g Lib_APPBaselib.cpp -o Lib_APPBaselib.exe -lXEngine_BaseLib


void EventTest()
{
	XEVENT hEvent = BaseLib_OperatorEvent_Create();
	if (NULL == hEvent)
	{
		return;
	}
	if (!BaseLib_OperatorEvent_Wait(hEvent))
	{
		return;
	}
	BaseLib_OperatorEvent_Delete(hEvent);
}
void TimeSpanTest()
{
	__int64x nDayTTime = 0;
	__int64x nHourTTime = 0;
	LPCXSTR lpszStartTime = _X("2019-11-01 12:31:10");
	LPCXSTR lpszEndTime = _X("2020-10-02 12:31:10");
	XENGINE_LIBTIMER st_LibTimer;
	memset(&st_LibTimer, '\0', sizeof(XENGINE_LIBTIMER));

	BaseLib_OperatorTimeSpan_GetForStr(lpszStartTime, lpszEndTime, &nDayTTime, 0);
	BaseLib_OperatorTimeSpan_GetForStr(lpszStartTime, lpszEndTime, &nHourTTime, 1);
	_xtprintf(_X("\n跨了：%lld天 %lld小时\n"), nDayTTime, nHourTTime);

	BaseLib_OperatorTimeSpan_CalForStr(lpszStartTime, lpszEndTime, &st_LibTimer, true);
	_xtprintf(_X("%d/%d/%d-%d:%d:%d\n"), st_LibTimer.wYear, st_LibTimer.wMonth, st_LibTimer.wDay, st_LibTimer.wHour, st_LibTimer.wMinute, st_LibTimer.wSecond);

	BaseLib_OperatorTimeSpan_CalForStr(lpszStartTime, lpszEndTime, &st_LibTimer, false);
	_xtprintf(_X("%d/%d/%d-%d:%d:%d\n"), st_LibTimer.wYear, st_LibTimer.wMonth, st_LibTimer.wDay, st_LibTimer.wHour, st_LibTimer.wMinute, st_LibTimer.wSecond);
}
void TimeTest()
{
	time_t nTime = 2706010596;
	XENGINE_LIBTIMER st_LibTimer;
	memset(&st_LibTimer, '\0', sizeof(XENGINE_LIBTIMER));

	BaseLib_OperatorTime_GetSysTime(&st_LibTimer);
	BaseLib_OperatorTime_TTimeToStuTime(nTime, &st_LibTimer);

	__int64x nTTime = 0;
	XCHAR tszTimeStr[128] = {0};
	LPCXSTR lpszTimeStr = _X("2022-08-01 10:22:01");
	BaseLib_OperatorTime_StrToInt(lpszTimeStr, &nTTime);
	BaseLib_OperatorTime_IntToStr(nTTime, tszTimeStr);

	BaseLib_OperatorTime_GetSysTime(&st_LibTimer);
	BaseLib_OperatorTime_TimezoneCvt(&st_LibTimer, 4);
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

	BaseLib_OperatorString_DelSub(tszMsgBuffer, _X("9"));

	BaseLib_OperatorString_GetKeyValue(tszMsgBuffer, _X(":"), tszKey, tszValue, true, &nHdrLen, &nBodyLen);
	LPCXSTR lpszFile1 = _X("./adadad/file.txt");
	LPCXSTR lpszFile2 = _X("D:\\adadad\\file.txt");
	LPCXSTR lpszFile3 = _X("./file.txt");
	LPCXSTR lpszFile4 = _X("file.txt");

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	int nLen = _xstprintf(tszMsgBuffer, "123456abc123765cd123ddd321");

	memset(tszValue, '\0', sizeof(tszValue));
	BaseLib_OperatorString_GetStartEnd(tszMsgBuffer, tszValue, NULL, "321", false);
	BaseLib_OperatorString_Change(tszMsgBuffer, &nLen, "ccccc", "123", "321", false);

	XCHAR tszFileDir[MAX_PATH];
	XCHAR tszFileName[MAX_PATH];
	XCHAR tszFileDrive[MAX_PATH];
	XCHAR tszFileExt[MAX_PATH];

	memset(tszFileDir, '\0', MAX_PATH);
	memset(tszFileName, '\0', MAX_PATH);
	memset(tszFileDrive, '\0', MAX_PATH);
	memset(tszFileExt, '\0', MAX_PATH);
	BaseLib_OperatorString_GetFileAndPath(lpszFile1, tszFileDir, tszFileName, tszFileDrive, tszFileExt);

	memset(tszFileDir, '\0', MAX_PATH);
	memset(tszFileName, '\0', MAX_PATH);
	memset(tszFileDrive, '\0', MAX_PATH);
	memset(tszFileExt, '\0', MAX_PATH);
	BaseLib_OperatorString_GetFileAndPath(lpszFile2, tszFileDir, tszFileName, tszFileDrive, tszFileExt);

	memset(tszFileDir, '\0', MAX_PATH);
	memset(tszFileName, '\0', MAX_PATH);
	BaseLib_OperatorString_GetFileAndPath(lpszFile3, tszFileDir, tszFileName);

	memset(tszFileDir, '\0', MAX_PATH);
	memset(tszFileName, '\0', MAX_PATH);
	BaseLib_OperatorString_GetFileAndPath(lpszFile4, tszFileDir, tszFileName);

	XCHAR tszFileStr[MAX_PATH] = {};
	BaseLib_OperatorString_GetSeparatorStr("123456/789/abc/ddd", "/", tszFileStr, 2, true);

	BaseLib_OperatorString_GetSeparatorStr("123456/789/abc/ddd", "/", tszFileStr, 1, false);
	return;
}

void GMTTimeTest()
{
	XCHAR tszGMTTime[128];
	memset(tszGMTTime, '\0', sizeof(tszGMTTime));
	BaseLib_OperatorTime_GMTTime(tszGMTTime);

	_xtprintf(_X("%s\n"), tszGMTTime);
}

int TestAddrLib()
{
	int nPort = 0;
	LPCXSTR lpszIPV4Convert = _X("192.168.*.10");
	
	LPCXSTR lpszIPV61 = _X("2031:0010:1F1F:0200:*:0100:11A0:ADDF");
	LPCXSTR lpszIPV62 = _X("1254:1800:200C::417A:AC11");
	LPCXSTR lpszIPV63 = _X("1080::8:800:200C:417A");
	LPCXSTR lpszIPV64 = _X("ACC::1101");
	LPCXSTR lpszIPV65 = _X("::1");
	LPCXSTR lpszIPV66 = _X("::192:168:1:54");
	XCHAR tszIPV6Addr[] = _X("1080:0:0:0:8:800:200C:5000");

	ENUM_XENGINE_BASELIB_IPADDR_TYPE enIPType;
	XENGINE_LIBADDR st_LibAddr;
	memset(&st_LibAddr, '\0', sizeof(XENGINE_LIBADDR));

	BaseLib_OperatorIPAddr_IsIPV4Addr(lpszIPV4Convert, &st_LibAddr);
	BaseLib_OperatorIPAddr_GetIPV4Type(&st_LibAddr, &enIPType);

	memset(&st_LibAddr, '\0', sizeof(XENGINE_LIBADDR));
	BaseLib_OperatorIPAddr_IsIPV6Addr(lpszIPV61, &st_LibAddr);

	memset(&st_LibAddr, '\0', sizeof(XENGINE_LIBADDR));
	BaseLib_OperatorIPAddr_IsIPV6Addr(lpszIPV62, &st_LibAddr);

	memset(&st_LibAddr, '\0', sizeof(XENGINE_LIBADDR));
	BaseLib_OperatorIPAddr_IsIPV6Addr(lpszIPV63, &st_LibAddr);

	memset(&st_LibAddr, '\0', sizeof(XENGINE_LIBADDR));
	BaseLib_OperatorIPAddr_IsIPV6Addr(lpszIPV64, &st_LibAddr);

	XCHAR tszIPAddr[128];
	memset(tszIPAddr, '\0', sizeof(tszIPAddr));
	BaseLib_OperatorIPAddr_ExpIPV6Addr(&st_LibAddr, tszIPAddr);
	memset(tszIPAddr, '\0', sizeof(tszIPAddr));
	BaseLib_OperatorIPAddr_ComIPV6Addr(&st_LibAddr, tszIPAddr);

	memset(&st_LibAddr, '\0', sizeof(XENGINE_LIBADDR));
	BaseLib_OperatorIPAddr_IsIPV6Addr(lpszIPV65, &st_LibAddr);

	memset(&st_LibAddr, '\0', sizeof(XENGINE_LIBADDR));
	BaseLib_OperatorIPAddr_IsIPV6Addr(lpszIPV66, &st_LibAddr);

	BaseLib_OperatorIPAddr_SegAddr(tszIPV6Addr, &nPort);
	return 1;
}
int test_Mutex()
{
	LPCXSTR lpszName = _X("xyry");
	XEVENT xhEvent = BaseLib_OperatorSemaphore_Create(lpszName);
	if (NULL == xhEvent)
	{
		_xtprintf(_X("失败\n"));
		return 0;
	}

	if (BaseLib_OperatorSemaphore_IsExist(lpszName))
	{
		_xtprintf(_X("存在\n"));
	}
	else
	{
		_xtprintf(_X("不存在\n"));
	}
	BaseLib_OperatorSemaphore_Delete(xhEvent);
	return 0;
}
int test_Memory()
{
	int** ppIntArray;
	if (!BaseLib_OperatorMemory_Malloc((void***)&ppIntArray, 3, sizeof(int)))
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
	BaseLib_OperatorMemory_Free((void***)&ppIntArray, 3);

	XCHAR** ppszStr;
	if (!BaseLib_OperatorMemory_Malloc((void***)&ppszStr, 3, sizeof(XCHAR) * 6))
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
	BaseLib_OperatorMemory_Free((void***)&ppszStr, 3);

	typedef struct
	{
		int a;
		int b;
	}XENGINE_MEMORY;
	XENGINE_MEMORY** ppSt_Memory;
	BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_Memory, 3, sizeof(XENGINE_MEMORY));
	for (int i = 0; i < 3; i++)
	{
		ppSt_Memory[i]->a = 1;
		ppSt_Memory[i]->b = i;
	}
	for (int i = 0; i < 3; i++)
	{
		_xtprintf(_X("%d %d\n"), ppSt_Memory[i]->a, ppSt_Memory[i]->b);
	}
	BaseLib_OperatorMemory_Free((void***)&ppSt_Memory, 3);
	return 0;
}
int test_TTrigger()
{
	XHANDLE xhToken;
	if (!BaseLib_OperatorTTigger_Create(&xhToken))
	{
		return -1;
	}
	if (!BaseLib_OperatorTTigger_Set(xhToken, 1))
	{
		return -2;
	}

	__int64x nTTimer = 0;
	BaseLib_OperatorTTigger_Get(xhToken, 1, &nTTimer);

	BaseLib_OperatorTTigger_Destory(xhToken);
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

	BaseLib_OperatorTime_GetTimeOfday(&st_Timeval);
	BaseLib_OperatorTime_TTimeToStuTime(st_Timeval.tv_sec, &st_LibTime);
	BaseLib_OperatorTime_TimeToStr(tszMsgTimer, NULL, NULL, &st_LibTime);

#ifdef _MSC_BUILD
	_xtprintf(_X("%lld %lld %llu %llu  %s\n"), time(NULL), st_Timeval.tv_sec, st_Timeval.tv_value, st_Timeval.tv_usec, tszMsgTimer);
#else
	_xtprintf("%ld %ld %llu %llu  %s\n", time(NULL), st_Timeval.tv_sec, st_Timeval.tv_value, st_Timeval.tv_usec, tszMsgTimer);
#endif
}
int profiletest()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszFile = "D:\\xengine_apps\\x64\\Debug\\1.txt";
#else
	LPCXSTR lpszFile = "./1.txt";
#endif
	BaseLib_OperatorFile_WriteProfileFromFile(lpszFile, "test", "a", "1");
	BaseLib_OperatorFile_WriteProfileFromFile(lpszFile, "test", "b", "2");
	BaseLib_OperatorFile_WriteProfileFromFile(lpszFile, "test", "c", "3");

	BaseLib_OperatorFile_WriteProfileFromFile(lpszFile, "test2", "1", "a");
	BaseLib_OperatorFile_WriteProfileFromFile(lpszFile, "test2", "2", "b");
	BaseLib_OperatorFile_WriteProfileFromFile(lpszFile, "test2", "3", "c");

	BaseLib_OperatorFile_WriteProfileFromFile(lpszFile, "test", "c", "中文");
	BaseLib_OperatorFile_WriteProfileFromFile(lpszFile, "test", "d", "4");
	BaseLib_OperatorFile_WriteProfileFromFile(lpszFile, "test", "b", "b2");
	BaseLib_OperatorFile_WriteProfileFromFile(lpszFile, "test2", "2", "2b");

	XCHAR tszValue[64];
	memset(tszValue, '\0', sizeof(tszValue));

	BaseLib_OperatorFile_ReadProfileFromFile(lpszFile, "test", "a", tszValue);
	printf("%s\n", tszValue);
	memset(tszValue, '\0', sizeof(tszValue));

	BaseLib_OperatorFile_ReadProfileFromFile(lpszFile, "test", "b", tszValue);
	printf("%s\n", tszValue);
	memset(tszValue, '\0', sizeof(tszValue));

	BaseLib_OperatorFile_ReadProfileFromFile(lpszFile, "test", "c", tszValue);
	printf("%s\n", tszValue);
	memset(tszValue, '\0', sizeof(tszValue));

	BaseLib_OperatorFile_ReadProfileFromFile(lpszFile, "test", "d", tszValue);
	printf("%s\n", tszValue);
	memset(tszValue, '\0', sizeof(tszValue));

	BaseLib_OperatorFile_ReadProfileFromFile(lpszFile, "test2", "1", tszValue);
	printf("%s\n", tszValue);
	memset(tszValue, '\0', sizeof(tszValue));

	BaseLib_OperatorFile_ReadProfileFromFile(lpszFile, "test2", "2", tszValue);
	printf("%s\n", tszValue);
	memset(tszValue, '\0', sizeof(tszValue));

	BaseLib_OperatorFile_ReadProfileFromFile(lpszFile, "test2", "3", tszValue);
	printf("%s\n", tszValue);
	memset(tszValue, '\0', sizeof(tszValue));
	return 0;
}
int test_handle()
{
	for (int i = 0; i < 10; i++)
	{
		XNETHANDLE xhToken = 0;
		BaseLib_OperatorHandle_Create(&xhToken);
		_xtprintf(_X("%lld\n"), xhToken);
	}
	for (int i = 0; i < 4; i++)
	{
		XCHAR tszKeyStr[64] = {};
		BaseLib_OperatorHandle_CreateStr(tszKeyStr, 6, 0, 1);
		printf("%s\n", tszKeyStr);
	}

	return 0;
}
int test_endain()
{
	double nValue64 = 1920;
	XCHAR tszMsgBuffer[MAX_PATH];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	BaseLib_OperatorEndain_ToHexW64((XBYTE*)tszMsgBuffer, *(__int64u*)&nValue64, true);
	return 0;
}
int test_ipaddr()
{
	XUINT nIPAddr = 0;
	LPCXSTR lpszIPAddr = _X("192.168.0.1");
	XCHAR tszIPAddr[128] = {};

	BaseLib_OperatorIPAddr_IPConvertInt(lpszIPAddr, &nIPAddr);
	BaseLib_OperatorIPAddr_IPConvertStr(&nIPAddr, tszIPAddr);

	printf("IPConvert:%d %s\n", nIPAddr, tszIPAddr);
	return 0;
}

int Test_Ver()
{
	printf("%lld\n%s\n", BaseLib_OperatorVer_XNumberInt(), BaseLib_OperatorVer_XNumberStr());
	printf("%s\n", BaseLib_OperatorVer_BuildTime());
	return 0;
}
int main()
{
	setlocale(LC_ALL, "");

	Test_Ver();
	test_endain();
	test_handle();
	TestAddrLib();
	StringTest();
	Test_GetTimeofday();
	test_TTrigger();
	test_Memory();
	test_Mutex();
	TimeSpanTest();
	TimeTest();
	GMTTimeTest();
	profiletest();
	test_ipaddr();
	return 0;
}