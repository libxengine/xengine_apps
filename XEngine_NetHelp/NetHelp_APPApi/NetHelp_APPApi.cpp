#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/NetHelp_APIHelp.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_APIHelp/APIHelp_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_APIHelp/APIHelp_Error.h"

//g++ -std=c++17 -Wall -g NetHelp_APPApi.cpp -o NetHelp_APPApi.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp -lXEngine_BaseLib -lNetHelp_APIHelp -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp,--disable-new-dtags

int Test_Http2Request()
{
	LPCTSTR lpszUrl = _T("http://test.xyry.org/");
	LPCTSTR lpszMsgBuffer = _T("12345");
	CHAR* ptszMsgBuffer = NULL;
	int nCode = 0;
	int nLen = 0;
	APIHELP_HTTPPARAMENT st_HTTPParam;
	memset(&st_HTTPParam, '\0', sizeof(APIHELP_HTTPPARAMENT));

	st_HTTPParam.bHTTP2Enable = TRUE;

	APIHelp_HttpRequest_Get(lpszUrl,&ptszMsgBuffer, &nLen, NULL, NULL, NULL, &st_HTTPParam);
	printf("%s\n", ptszMsgBuffer);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	return 0;
}
int Test_HttpRequest()
{
	LPCTSTR lpszUrl = _T("http://www.baidu.com");
	CHAR* ptszMsgBuffer = NULL;

	APIHelp_HttpRequest_Get(lpszUrl, &ptszMsgBuffer);
	printf("%s\n", ptszMsgBuffer);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	return 0;
}
int Test_NetGetIPAddr()
{
	TCHAR tszRemoteBuffer[1024];
	memset(tszRemoteBuffer, '\0', sizeof(tszRemoteBuffer));

	if (!APIHelp_NetWork_GetIPNet(tszRemoteBuffer))
	{
		printf("APIHelp_NetWork_GetIPNet:%lX", APIHelp_GetLastError());
	}
	printf("%s\n", tszRemoteBuffer);
	return 0;
}

void __stdcall NetHelp_HttpGet_Chunked(LPVOID lpszMsgBuffer, int nMsgLen, LPVOID lParam)
{
	printf("%d\n%s\n", nMsgLen, (LPCTSTR)lpszMsgBuffer);
}
int Test_Domain()
{
	TCHAR tszUrlPath[MAX_PATH];
	APIHELP_DOMAIN st_APIDomain;
	ENUM_APIHELP_DOMAINTYPE enAPIDomain;

	memset(tszUrlPath, '\0', MAX_PATH);
	memset(&st_APIDomain, '\0', sizeof(APIHELP_DOMAIN));

	APIHelp_Domain_GetInfo("http://www.xyry.org/Api/Task/Query", &st_APIDomain, &enAPIDomain, tszUrlPath);
	printf("APIHelp_Domain_GetInfo:%s,%s,%s,%s,%d\n",st_APIDomain.tszDomainName, st_APIDomain.tszMainDomain, st_APIDomain.tszSubDomain, st_APIDomain.tszTopDomain, enAPIDomain);
	return 0;
}
int Test_HttpCustom()
{
	XNETHANDLE xhToken = 0;
	CHAR* ptszMsgBuffer = NULL;

	APIHelp_HttpRequest_Create(&xhToken , NetHelp_HttpGet_Chunked);
	APIHelp_HttpRequest_SetUrl(xhToken, "www.xyry.org", "GET");

	APIHelp_HttpRequest_Excute(xhToken, &ptszMsgBuffer);
	APIHelp_HttpRequest_Close(xhToken);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	return 0;
}

int main()
{
	Test_Http2Request();
	Test_Domain();
	Test_NetGetIPAddr();
	Test_HttpRequest();
	Test_HttpCustom();
	return 0;
}

