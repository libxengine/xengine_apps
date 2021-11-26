#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#else

#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Error.h"

//g++ -std=c++17 -Wall -g Lib_APPBaselib.cpp -o Lib_APPBaselib.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -lXEngine_BaseLib -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -lpthread

void EventTest()
{
	XNETEVENT hEvent = BaseLib_OperatorEvent_Create();
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
	LPCTSTR lpszStartTime = _T("2019-11-01 12:31:10");
	LPCTSTR lpszEndTime = _T("2020-10-02 12:31:10");
	XENGINE_LIBTIMER st_LibTimer;
	memset(&st_LibTimer, '\0', sizeof(XENGINE_LIBTIMER));

	BaseLib_OperatorTimeSpan_GetForStr(lpszStartTime, lpszEndTime, &nDayTTime, 0);
	BaseLib_OperatorTimeSpan_GetForStr(lpszStartTime, lpszEndTime, &nHourTTime, 1);
	printf(_T("\n跨了：%lld天 %lld小时\n"), nDayTTime, nHourTTime);

	BaseLib_OperatorTimeSpan_CalForStr(lpszStartTime, lpszEndTime, &st_LibTimer, TRUE);
	printf(_T("%d/%d/%d-%d:%d:%d\n"), st_LibTimer.wYear, st_LibTimer.wMonth, st_LibTimer.wDay, st_LibTimer.wHour, st_LibTimer.wMinute, st_LibTimer.wSecond);

	BaseLib_OperatorTimeSpan_CalForStr(lpszStartTime, lpszEndTime, &st_LibTimer, FALSE);
	printf(_T("%d/%d/%d-%d:%d:%d\n"), st_LibTimer.wYear, st_LibTimer.wMonth, st_LibTimer.wDay, st_LibTimer.wHour, st_LibTimer.wMinute, st_LibTimer.wSecond);
}
void TimeTest()
{
	time_t nTime = 2706010596;
	XENGINE_LIBTIMER st_LibTimer;
	memset(&st_LibTimer, '\0', sizeof(XENGINE_LIBTIMER));

	BaseLib_OperatorTime_GetSysTime(&st_LibTimer);
	BaseLib_OperatorTime_TTimeToStuTime(nTime, &st_LibTimer);
	return;
}
void StringTest()
{
	LPCTSTR lpszFile1 = _T("./adadad/file.txt");
	LPCTSTR lpszFile2 = _T("D:\\adadad\\file.txt");
	LPCTSTR lpszFile3 = _T("./file.txt");
	LPCTSTR lpszFile4 = _T("file.txt");

	TCHAR tszFileDir[MAX_PATH];
	TCHAR tszFileName[MAX_PATH];
	TCHAR tszFileDrive[MAX_PATH];
	TCHAR tszFileExt[MAX_PATH];

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

	XENGINE_LIBADDR st_LibAddr;
	memset(&st_LibAddr, '\0', sizeof(XENGINE_LIBADDR));
	BaseLib_OperatorIPAddr_AddrStruct("192.168.1.5", &st_LibAddr);

	return;
}


void HandleTest()
{
	TCHAR tszMsgBuffer[64];
	memset(tszMsgBuffer, '\0', 64);
	BaseLib_OperatorHandle_CreateGuid(tszMsgBuffer, TRUE, FALSE);
	printf("%s\n", tszMsgBuffer);

	typedef struct
	{
		int a;
		int b;
	}NETENGINE_HANDLETEST;

	XNETHANDLE xhNet = 0;
	NETENGINE_HANDLETEST st_HandleTest;
	st_HandleTest.a = 1;
	st_HandleTest.b = 2;

	BaseLib_OperatorHandle_Add(&xhNet, sizeof(NETENGINE_HANDLETEST));
	memset(&st_HandleTest, '\0', sizeof(NETENGINE_HANDLETEST));
	BaseLib_OperatorHandle_Get(xhNet, &st_HandleTest);
	st_HandleTest.a = 3;
	st_HandleTest.b = 4;
	BaseLib_OperatorHandle_Set(xhNet, &st_HandleTest);
	memset(&st_HandleTest, '\0', sizeof(NETENGINE_HANDLETEST));
	BaseLib_OperatorHandle_Get(xhNet, &st_HandleTest);
	BaseLib_OperatorHandle_Del(xhNet);
}

void GMTTimeTest()
{
	TCHAR tszGMTTime[128];
	memset(tszGMTTime, '\0', sizeof(tszGMTTime));
	BaseLib_OperatorTime_GMTTime(tszGMTTime);

	printf("%s\n", tszGMTTime);
}
void FromCharGetKeyValue()
{
	LPCTSTR lpszStrSource = _T("123: 321");
	TCHAR tszKey[32];
	TCHAR tszValue[32];

	memset(tszKey, '\0', sizeof(tszKey));
	memset(tszValue, '\0', sizeof(tszValue));

	BaseLib_OperatorString_FromCharGetKeyValue(lpszStrSource, ':', tszKey, tszValue);
	printf("%s = %s\n", tszKey, tszValue);


	LPCTSTR lpszStrDest = _T("asd=zxc");
	memset(tszKey, '\0', sizeof(tszKey));
	memset(tszValue, '\0', sizeof(tszValue));

	BaseLib_OperatorString_FromCharGetKeyValue(lpszStrDest, '=', tszKey, tszValue);
	printf("%s = %s\n", tszKey, tszValue);
}

int TestAddrLib()
{
	LPCTSTR lpszIPV4Convert = _T("192.0.*.10");
	LPCTSTR lpszIPV6Convert = _T("ACC::1101");

	XENGINE_LIBADDR st_LibAddr;
	XENGINE_LIBADDR st_LibAddrV6;
	memset(&st_LibAddr, '\0', sizeof(XENGINE_LIBADDR));
	memset(&st_LibAddrV6, '\0', sizeof(XENGINE_LIBADDR));

	BaseLib_OperatorIPAddr_AddrStruct(lpszIPV4Convert, &st_LibAddr, TRUE);
	BaseLib_OperatorIPAddr_AddrStruct(lpszIPV6Convert, &st_LibAddrV6, TRUE);
	return 1;
}
int Test_GetLunarCalendar()
{
	XENGINE_LIBTIMER st_LibTime;
	XENGINE_LIBTIMER st_CLTime;

	memset(&st_LibTime, '\0', sizeof(XENGINE_LIBTIMER));
	memset(&st_CLTime, '\0', sizeof(XENGINE_LIBTIMER));

	BaseLib_OperatorTime_GetSysTime(&st_LibTime);
	BaseLib_OperatorTime_GetLunarCalendar(&st_LibTime, &st_CLTime);

	return 1;
}
int test_Mutex()
{
	LPCTSTR lpszName = _T("xyry");
	XNETEVENT xhEvent = BaseLib_OperatorSemaphore_Create(lpszName);
	if (NULL == xhEvent)
	{
		printf("失败\n");
		return 0;
	}

	if (BaseLib_OperatorSemaphore_IsExist(lpszName))
	{
		printf("存在\n");
	}
	else
	{
		printf("不存在\n");
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
		printf("%d\n", *(ppIntArray[i]));
	}
	BaseLib_OperatorMemory_Free((void***)&ppIntArray, 3);

	CHAR** ppszStr;
	if (!BaseLib_OperatorMemory_Malloc((void***)&ppszStr, 3, 6))
	{
		return -1;
	}
	for (int i = 0; i < 3; i++)
	{
		strcpy(ppszStr[i], "hello");
	}
	for (int i = 0; i < 3; i++)
	{
		printf("%s\n", ppszStr[i]);
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
		printf("%d %d\n", ppSt_Memory[i]->a, ppSt_Memory[i]->b);
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
	TCHAR tszMsgTimer[128];

	memset(&st_Timeval, '\0', sizeof(XENGINE_VALTIME));
	memset(&st_LibTime, '\0', sizeof(XENGINE_LIBTIMER));
	memset(tszMsgTimer, '\0', sizeof(tszMsgTimer));

	BaseLib_OperatorTime_GetTimeOfday(&st_Timeval);
	BaseLib_OperatorTime_TTimeToStuTime(st_Timeval.tv_sec, &st_LibTime);
	BaseLib_OperatorTime_ToStringTimer(tszMsgTimer, &st_LibTime);

#ifdef _WINDOWS
	printf("%lld %lld %llu %llu  %s\n", time(NULL), st_Timeval.tv_sec, st_Timeval.tv_value, st_Timeval.tv_usec, tszMsgTimer);
#else
	printf("%ld %ld %llu %llu  %s\n", time(NULL), st_Timeval.tv_sec, st_Timeval.tv_value, st_Timeval.tv_usec, tszMsgTimer);
#endif
}
int profiletest()
{
#ifdef _WINDOWS
	LPCTSTR lpszFile = _T("D:\\xengine_apps\\Debug\\1.txt");
#else
	LPCTSTR lpszFile = _T("./1.txt");
#endif

	BaseLib_OperatorFile_WriteProfileFromFile(lpszFile, "test", "a", "1");
	BaseLib_OperatorFile_WriteProfileFromFile(lpszFile, "test", "b", "2");
	BaseLib_OperatorFile_WriteProfileFromFile(lpszFile, "test", "c", "3");

	BaseLib_OperatorFile_WriteProfileFromFile(lpszFile, "test2", "1", "a");
	BaseLib_OperatorFile_WriteProfileFromFile(lpszFile, "test2", "2", "b");
	BaseLib_OperatorFile_WriteProfileFromFile(lpszFile, "test2", "3", "c");

	BaseLib_OperatorFile_WriteProfileFromFile(lpszFile, "test", "c", "c3");
	BaseLib_OperatorFile_WriteProfileFromFile(lpszFile, "test", "d", "4");
	BaseLib_OperatorFile_WriteProfileFromFile(lpszFile, "test", "b", "b2");
	BaseLib_OperatorFile_WriteProfileFromFile(lpszFile, "test2", "2", "2b");

	TCHAR tszValue[64];
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

int main()
{
	profiletest();
	StringTest();
	Test_GetTimeofday();
	test_TTrigger();
	test_Memory();
	test_Mutex();
	TimeSpanTest();
	Test_GetLunarCalendar();
	TimeTest();
	GMTTimeTest();
	FromCharGetKeyValue();
	TestAddrLib();
	return 0;
}
