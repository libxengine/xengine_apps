// pch.cpp: 与预编译标头对应的源文件

#include "pch.h"

DWORD dwLastError = 0;
// 当使用预编译的头时，需要使用此源文件，编译才能成功。
extern "C" BOOL FPCall_HttpExe_Process(LPCTSTR lpszClientAddr, RFCCOMPONENTS_HTTP_REQPARAM * pSt_HTTPParam, CHAR * **pppszListHdr, int nListCount, LPCTSTR lpszHttpBody, int nBodyLen, int* pInt_HttpCode, TCHAR * ptszMiniType, TCHAR * ptszMsgBuffer, int* pInt_Len)
{
	_tcscat(ptszMsgBuffer, lpszClientAddr);
	_tcscat(ptszMsgBuffer, " ");

	_tcscat(ptszMsgBuffer, pSt_HTTPParam->tszHttpMethod);
	_tcscat(ptszMsgBuffer, " ");

	_tcscat(ptszMsgBuffer, pSt_HTTPParam->tszHttpUri);
	_tcscat(ptszMsgBuffer, " ");

	_tcscat(ptszMsgBuffer, pSt_HTTPParam->tszHttpVer);
	_tcscat(ptszMsgBuffer, " ");

	_tcscat(ptszMsgBuffer, "\r\n");
	for (int i = 0; i < nListCount; i++)
	{
		_tcscat(ptszMsgBuffer, (*pppszListHdr)[i]);
		_tcscat(ptszMsgBuffer, "\r\n");
	}
	*pInt_Len = _tcslen(ptszMsgBuffer);

	*pInt_HttpCode = 200;
	_tcscpy(ptszMiniType, "html");
	return TRUE;
}
extern "C" DWORD FPCall_HttpExe_GetError()
{
	return dwLastError;
}