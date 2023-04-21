#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/HelpComponents_XLog.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_HelpComponents/HelpComponents_XLog/XLog_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_HelpComponents/HelpComponents_XLog/XLog_Error.h"

//Linux::g++ -std=gnu++17 -Wall -g HelpComponents_APPLog.cpp -o HelpComponents_APPLog.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_HelpComponents -lXEngine_BaseLib -lXEngine_SystemApi -lHelpComponents_XLog -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_HelpComponents,--disable-new-dtags
//Macos;:g++ -std=gnu++17 -Wall -g HelpComponents_APPLog.cpp -o HelpComponents_APPLog.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_SystemSdk -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_HelpComponents -lXEngine_BaseLib -lXEngine_SystemApi -lHelpComponents_XLog 


int main()
{
	HELPCOMPONENTS_XLOG_COLOR st_XLogColor;
	HELPCOMPONENTS_XLOG_CONFIGURE st_XLogConfig;

	memset(&st_XLogColor, '\0', sizeof(HELPCOMPONENTS_XLOG_COLOR));
	memset(&st_XLogConfig, '\0', sizeof(HELPCOMPONENTS_XLOG_CONFIGURE));

	st_XLogConfig.XLog_MaxSize = 10240;
	st_XLogConfig.XLog_MaxBackupFile = 10;

	//st_XLogConfig.st_BackInfo.enBackType = HELPCOMPONENTS_XLOG_BACKTYPE_TIME;./
	strcpy(st_XLogConfig.st_BackInfo.tszBackDir, _X("./back/"));
	strcpy(st_XLogConfig.tszFileName, _X("./log/test.log"));
	XHANDLE xhLog = HelpComponents_XLog_Init(HELPCOMPONENTS_XLOG_OUTTYPE_FILE | HELPCOMPONENTS_XLOG_OUTTYPE_STD, &st_XLogConfig);
	if (NULL == xhLog)
	{
		printf("HelpComponents_XLog_Init:%lX\n", XLog_GetLastError());
		return -1;
	}
	HelpComponents_XLog_SetLogPriority(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG);

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