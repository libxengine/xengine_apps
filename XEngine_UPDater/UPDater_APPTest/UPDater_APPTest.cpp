#include "UPDater_Hdr.h"

BOOL bIsRun = FALSE;
XLOG xhLog = NULL;
UPDATERSERVICE_CONFIG st_ServiceConfig;

void Signale_Handler(int sig)
{
	XLOG_PRINT(xhLog, NETENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("服务器退出"));
	bIsRun = FALSE;

	UPData_DlParser_Close();
	HelpComponents_XLog_Destroy(xhLog);
#ifdef _WINDOWS
	WSACleanup();
#endif
	exit(0);
}

int main(int argc, char** argv)
{
	bIsRun = TRUE;
	HELPCOMPONENTS_XLOG_CONFIGURE st_XLogConfig;

	bIsRun = TRUE;
	int nDesLen = 1024000;
	int nUPLen = 1024000;
	int nListCount = 0;
	
	TCHAR* ptszUPDesp = NULL;
	TCHAR* ptszJsonMsg = NULL;
	FILEPARSER_VERSIONINFO** ppSt_ListVer;

	ptszUPDesp = new TCHAR[nDesLen];
	ptszJsonMsg = new TCHAR[nUPLen];

	if ((NULL == ptszUPDesp) || (NULL == ptszJsonMsg))
	{
		printf("申请内存失败，无法继续！\n");
		return 0;
	}
	memset(ptszUPDesp, '\0', nDesLen);
	memset(ptszJsonMsg, '\0', nUPLen);
	memset(&st_XLogConfig, '\0', sizeof(HELPCOMPONENTS_XLOG_CONFIGURE));
	memset(&st_ServiceConfig, '\0', sizeof(UPDATERSERVICE_CONFIG));

	st_XLogConfig.XLog_MaxBackupFile = 10;
	st_XLogConfig.XLog_MaxSize = 102400;

#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	if (!UPDater_Parament(argc, argv, &st_ServiceConfig))
	{
		printf("参数解析失败,无法继续\n");
		return FALSE;
	}
	xhLog = HelpComponents_XLog_Init(HELPCOMPONENTS_XLOG_OUTTYPE_STD, &st_XLogConfig);
	if (NULL == xhLog)
	{
		printf("初始化日志服务失败,无法继续!\n");
		goto NETSERVICE_APPEXIT;
	}
	HelpComponents_XLog_SetLogPriority(xhLog, NETENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO);
	XLOG_PRINT(xhLog, NETENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化日志系统成功"));

	XLOG_PRINT(xhLog, NETENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("启动服务中，初始化配置成功,配置URL:%s,配置本地列表:%s,下载路径:%s,文件下载选项:%d"), st_ServiceConfig.tszUPUrl, st_ServiceConfig.tszLocalList, st_ServiceConfig.tszDownPath, st_ServiceConfig.bIsDown);
	signal(SIGINT, Signale_Handler);
	signal(SIGTERM, Signale_Handler);
	signal(SIGABRT, Signale_Handler);

	if (st_ServiceConfig.bIsMake)
	{
		LPCSTR lpszUrl = _T("http://www.xyry.org/UPLoad/XEngine_Release/");

		XLOG_PRINT(xhLog, NETENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("开始构建版本列表"));
		if (!HelpModule_Api_BuildVer(st_ServiceConfig.tszMakePath, st_ServiceConfig.tszLocalList, 0, TRUE, "XEngine_Release.txt", lpszUrl))
		{
			XLOG_PRINT(xhLog, NETENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("构建标准版本列表失败！错误：%lX"), UPHelpModule_GetLastError());
			goto NETSERVICE_APPEXIT;
		}
		XLOG_PRINT(xhLog, NETENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("构建本地版本列表成功！本地自定义列表名：%s，更新列表名：%s"), st_ServiceConfig.tszLocalList, "XEngine_Release.txt");
		goto NETSERVICE_APPEXIT;
	}

	if (!FileParser_ReadVer_GetUPVersion(st_ServiceConfig.tszUPUrl, ptszJsonMsg, &nUPLen))
	{
		XLOG_PRINT(xhLog, NETENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("获取更新失败,获取新版本信息失败,错误:%lX!"), UPFileParser_GetLastError());
		goto NETSERVICE_APPEXIT;
	}
	XLOG_PRINT(xhLog, NETENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("获取网络引擎服务更新信息成功,开始获取是否存在新版本..."));

	if (!FileParser_Match_Start(st_ServiceConfig.tszLocalList, ptszJsonMsg, nUPLen, &ppSt_ListVer, &nListCount))
	{
		DWORD dwRet = UPFileParser_GetLastError();
		if (ERROR_XENGINE_UPDATA_FILEPARSER_MATCH_START_NOTNEWVER == dwRet)
		{
			XLOG_PRINT(xhLog, NETENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("获取更新失败,没有新版本可以使用!"));
		}
		else
		{
			XLOG_PRINT(xhLog, NETENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("获取更新失败,错误:%lX!"), dwRet);
		}
		goto NETSERVICE_APPEXIT;
	}
	XLOG_PRINT(xhLog, NETENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("获取新版本成功,开始获取更新描述..."));

	if (!FileParser_ReadVer_GetDescription(ptszJsonMsg, nUPLen, ptszUPDesp, &nDesLen))
	{
		XLOG_PRINT(xhLog, NETENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("获取更新失败,获取更新描述信息失败,错误:%lX"), UPFileParser_GetLastError());
		goto NETSERVICE_APPEXIT;
	}
	XLOG_PRINT(xhLog, NETENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("新版本描述信息\r\n%s"), ptszUPDesp);

	if (!st_ServiceConfig.bIsDown)
	{
		XLOG_PRINT(xhLog, NETENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("获取更新成功,但是配置为不下载更新,请手动检测下载更新并且修改本地配置版本,将自动退出"));
		goto NETSERVICE_APPEXIT;
	}

	{
		for (int i = 0; i < nListCount; i++)
		{
			if (0 == ppSt_ListVer[i]->st_LocalVersion.nModuleVersion)
			{
				XLOG_PRINT(xhLog, NETENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("增加一个模块，模块名称：%s，老模块版本:%ld,新模块版本:%ld"), ppSt_ListVer[i]->tszModuleName, ppSt_ListVer[i]->st_LocalVersion.nModuleVersion, ppSt_ListVer[i]->nModuleVersion);
			}
			else
			{
				XLOG_PRINT(xhLog, NETENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("更新一个模块，模块名称：%s，老模块版本:%ld,新模块版本:%ld"), ppSt_ListVer[i]->tszModuleName, ppSt_ListVer[i]->st_LocalVersion.nModuleVersion, ppSt_ListVer[i]->nModuleVersion);
			}
		}
		XLOG_PRINT(xhLog, NETENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("开始进行下载更新..."));
	}

	system("rd /s /q \"./UPFile/\" 2>nul");
	system("md \"./UPFile\"");
	XLOG_PRINT(xhLog, NETENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("清理临时文件成功！"));

	UPData_DlParser_CBQuery(UPDater_Callback_Download);
	if (!UPData_DlParser_Init(st_ServiceConfig.tszDownPath, &ppSt_ListVer,nListCount, FALSE))
	{
		XLOG_PRINT(xhLog, NETENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("更新失败,初始化更新器失败,错误:%lX"), UPDataDown_GetLastError());
		goto NETSERVICE_APPEXIT;
	}

	XLOG_PRINT(xhLog, NETENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("开始进行文件下载..."));
	while (bIsRun)
	{
	}

	XLOG_PRINT(xhLog, NETENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("开始安装更新文件..."));
	UPData_DlParser_Close();
	if (!HelpModule_Api_Install(st_ServiceConfig.tszLocalList, ptszJsonMsg, nUPLen, &ppSt_ListVer,nListCount, st_ServiceConfig.tszDownPath))
	{
		XLOG_PRINT(xhLog, NETENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("更新失败,安装文件失败,错误:%lX"), UPHelpModule_GetLastError());
		goto NETSERVICE_APPEXIT;
	}
	XLOG_PRINT(xhLog, NETENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("更新成功,退出更新服务!"));
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListVer, nListCount);

NETSERVICE_APPEXIT:
	bIsRun = FALSE;

	if (NULL != ptszUPDesp)
	{
		delete[]ptszUPDesp;
		ptszUPDesp = NULL;
	}
	if (NULL != ptszJsonMsg)
	{
		delete[]ptszJsonMsg;
		ptszJsonMsg = NULL;
	}

	UPData_DlParser_Close();
	HelpComponents_XLog_Destroy(xhLog);

#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}
