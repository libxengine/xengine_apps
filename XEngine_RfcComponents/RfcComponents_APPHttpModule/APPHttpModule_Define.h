#pragma once

extern "C" BOOL FPCall_HttpExe_Process(LPCTSTR lpszClientAddr, RFCCOMPONENTS_HTTP_REQPARAM * pSt_HTTPParam, CHAR * **pppszListHdr, int nListCount, LPCTSTR lpszHttpBody, int nBodyLen, int* pInt_HttpCode, TCHAR * ptszMiniType, TCHAR * ptszMsgBuffer, int* pInt_Len);
extern "C" DWORD FPCall_HttpExe_GetError();