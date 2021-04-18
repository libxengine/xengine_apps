#include "UPDater_Hdr.h"

BOOL UPDater_Parament(int argc, char **argv, UPDATERSERVICE_CONFIG *pSt_StartlParam)
{
	pSt_StartlParam->nLogType = 2;
	pSt_StartlParam->bIsDown = 0;
	pSt_StartlParam->bIsMake = FALSE;
	_tcscpy(pSt_StartlParam->tszUPUrl, "http://www.xyry.org/UPLoad/XEngine_Release.txt");
#ifdef _WINDOWS
	_tcscpy(pSt_StartlParam->tszLocalList, "H:\\XEngine_Apps\\Debug\\XEngine_Release.list");
	_tcscpy(pSt_StartlParam->tszMakePath, "H:\\XEngine\\XEngine_Release\\");
	_tcscpy(pSt_StartlParam->tszDownPath, "H:\\XEngine_Apps\\Debug\\UPFile\\");
#else
	_tcscpy(pSt_StartlParam->tszLocalList, "./XEngine_Release.list");
	_tcscpy(pSt_StartlParam->tszMakePath, "./Debug");
	_tcscpy(pSt_StartlParam->tszDownPath, "./UPFile/");
#endif

	for (int i = 0;i < argc;i++)
	{
		if ((0 == _tcscmp("-h", argv[i])) || (0 == _tcscmp("-H", argv[i])))
		{
			UPDater_ParamentHelp();
			return FALSE;
		}
		if ((0 == _tcscmp("-v", argv[i])) || (0 == _tcscmp("-V", argv[i])))
		{
			printf("Version：V1.1.0\n");
			return FALSE;
		}
		else if (0 == _tcscmp("-u", argv[i]))
		{
			memset(pSt_StartlParam->tszUPUrl,'\0',sizeof(pSt_StartlParam->tszUPUrl));
			_tcscpy_s(pSt_StartlParam->tszUPUrl, MAX_PATH, argv[i + 1]);
		}
		else if (0 == _tcscmp("-f", argv[i]))
		{
			memset(pSt_StartlParam->tszLocalList, '\0', sizeof(pSt_StartlParam->tszLocalList));
			_tcscpy_s(pSt_StartlParam->tszLocalList, MAX_PATH, argv[i + 1]);
		}
		else if (0 == _tcscmp("-d", argv[i]))
		{
			memset(pSt_StartlParam->tszDownPath, '\0', sizeof(pSt_StartlParam->tszDownPath));
			_tcscpy_s(pSt_StartlParam->tszDownPath, MAX_PATH, argv[i + 1]);
		}
		else if (0 == _tcscmp("-l", argv[i]))
		{
			pSt_StartlParam->nLogType = _ttoi(argv[i + 1]);
		}
		else if (0 == _tcscmp("-down", argv[i]))
		{
			pSt_StartlParam->bIsDown = _ttoi(argv[i + 1]);
		}
	}

	return TRUE;
}

void UPDater_ParamentHelp()
{
	printf(_T("--------------------------启动参数帮助开始--------------------------\n"));
	printf(_T("更新服务启动参数：程序 参数 参数值，参数是区分大小写的\n"));
	printf(_T("-h or -H：启动参数帮助提示信息\n"));
	printf(_T("-v or -V：查看系统版本\n"));
	printf(_T("-u：设置检查更新信息的URL地址,默认:http://www.xyry.org/UPLoad/NetEngine_WINVer.txt\n"));
	printf(_T("-f：设置本地文件列表目录,默认:./LocalFile_List.txt\n"));
	printf(_T("-d：设置更新文件下载的目录,默认:./UPFile/\n"));
	printf(_T("-m：构建本地版本更新列表，需要参数设置要构建软件版本的目录,目录默认需要添加/符号.\n"));
	printf(_T("-l：设置本地日志记录信息,默认为2,使用文件和控制台\n"));
	printf(_T("-d：设置有新版本是否下载文件,默认0不下载,1位下载\n"));
	printf(_T("--------------------------启动参数帮助结束--------------------------\n"));
}