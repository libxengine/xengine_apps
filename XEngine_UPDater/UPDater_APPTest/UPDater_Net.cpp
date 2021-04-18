#include "UPDater_Hdr.h"

void __stdcall UPDater_Callback_Download(LPCTSTR lpszFileName, double nDownSize, double nFileSize, int nRate, int nAllRate, LPVOID lParam)
{
	XLOG_PRINT(xhLog, NETENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO,_T("下载的文件:%s,下载大小:%lf,文件大小:%lf,进度:%d,总进度:%d"),lpszFileName,nDownSize,nFileSize,nRate,nAllRate);

	if (100 == nAllRate)
	{
		bIsRun = FALSE;
		XLOG_PRINT(xhLog, NETENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("文件下载完成"));
	}
}