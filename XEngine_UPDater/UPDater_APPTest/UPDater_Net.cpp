#include "UPDater_Hdr.h"

void __stdcall UPDater_Callback_Download(LPCTSTR lpszFileName, double nDownSize, double nFileSize, int nRate, int nAllRate, LPVOID lParam)
{
	XLOG_PRINT(xhLog, NETENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO,_T("���ص��ļ�:%s,���ش�С:%lf,�ļ���С:%lf,����:%d,�ܽ���:%d"),lpszFileName,nDownSize,nFileSize,nRate,nAllRate);

	if (100 == nAllRate)
	{
		bIsRun = FALSE;
		XLOG_PRINT(xhLog, NETENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("�ļ��������"));
	}
}