#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#if 1 == _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_HelpComponents/XLog_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/XLog_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_XLog.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_HelpComponents/HelpComponents_XLog/XLog_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_HelpComponents/HelpComponents_XLog/XLog_Error.h"
#ifdef _MSC_BUILD
#ifdef _WIN64
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/HelpComponents_XLog.lib")
#else
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/HelpComponents_XLog.lib")
#endif
#endif
#endif

//Linux::g++ -std=c++20 -Wall -g HelpComponents_APPLog.cpp -o HelpComponents_APPLog.exe -lXEngine_BaseLib -lXEngine_SystemApi -lHelpComponents_XLog 

int main()
{
	HELPCOMPONENTS_XLOG_COLOR st_XLogColor;
	HELPCOMPONENTS_XLOG_CONFIGURE st_XLogConfig;

	memset(&st_XLogColor, '\0', sizeof(HELPCOMPONENTS_XLOG_COLOR));
	memset(&st_XLogConfig, '\0', sizeof(HELPCOMPONENTS_XLOG_CONFIGURE));

	st_XLogConfig.XLog_MaxSize = 10240;
	st_XLogConfig.XLog_MaxBackupFile = 10;

	//st_XLogConfig.st_BackInfo.enBackType = HELPCOMPONENTS_XLOG_BACKTYPE_TIME;./
	strcpy(st_XLogConfig.st_BackInfo.tszBackDir, _X("D:\\xengine_apps\\Debug\\back\\"));
	strcpy(st_XLogConfig.tszFileName, _X("D:\\xengine_apps\\Debug\\log\\test.log"));

	XHANDLE xhLog = HelpComponents_XLog_Init(HELPCOMPONENTS_XLOG_OUTTYPE_FILE | HELPCOMPONENTS_XLOG_OUTTYPE_STD, &st_XLogConfig);
	if (NULL == xhLog)
	{
		printf("HelpComponents_XLog_Init:%lX\n", XLog_GetLastError());
		return -1;
	}
	HelpComponents_XLog_StrongOPen(xhLog, "D:\\xengine_apps\\Debug\\stroage\\info.log", XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO);
	//HelpComponents_XLog_SetLogPriority(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG);

	HelpComponents_XLog_ValueInsert(xhLog, "$(int)", "1988");
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("xlog test 变量测试:$(int).得到值"));
	HelpComponents_XLog_ValueDelete(xhLog, "$(int)");

	HelpComponents_XLog_SetLogInterval(xhLog, 1000);
	for (int i = 0; i < 100; i++)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("xlog test 1=%d XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO中文测试"), i);
		//std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	HelpComponents_XLog_StrongClose(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO);
	HelpComponents_XLog_SetLogInterval(xhLog, 0);

	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("xlog test 1=XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO"));
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("xlog test 2=XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN"));
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("xlog test 3=XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR"));

	st_XLogColor.wError = XENGINE_HELPCOMPONENTS_XLOG_BACK_BLUE | XENGINE_HELPCOMPONENTS_XLOG_TEXT_RED;
	st_XLogColor.wWarn = XENGINE_HELPCOMPONENTS_XLOG_TEXT_GREEN;

	if (!HelpComponents_XLog_SetLogColor(xhLog, &st_XLogColor))
	{
		printf("HelpComponents_XLog_SetLogColor:%lX\n", XLog_GetLastError());
		return -1;
	}

	for (int i = 0; i < 10000; i++)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("xlog test 1=%d XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO"), i);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("xlog test 2=%d XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN"), i);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("xlog test 3=%d XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR"), i);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("xlog test ok"));
	}

	HelpComponents_XLog_SetLogAllow(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO | XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("xlog test"));
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("xlog test"));
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("xlog test"));
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("xlog test ok"));

	HelpComponents_XLog_Destroy(xhLog);
	return 0;
}