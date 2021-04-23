#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/NetHelp_APIHelp.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_APIHelp/APIHelp_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_APIHelp/APIHelp_Error.h"

//g++ -std=c++17 -Wall -g NetHelp_APPApi.cpp -o NetHelp_APPApi.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp -lXEngine_BaseLib -lNetHelp_APIHelp -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp,--disable-new-dtags


int Test_HttpRequest()
{
	LPCTSTR lpszUrl = _T("http://www.baidu.com");

	int nResponse = 0;
	int nSourceLen = 102400;

	TCHAR tszSourceBuffer[102400];
	TCHAR tszDestBuffer[102400];
	memset(tszSourceBuffer, '\0', sizeof(tszSourceBuffer));
	memset(tszDestBuffer, '\0', sizeof(tszDestBuffer));

	APIHelp_HttpRequest_Post(lpszUrl, &nResponse, NULL, tszSourceBuffer, &nSourceLen);
	//APIHelp_HttpRequest_Post(lpszUrl, tszSourceBuffer,&nResponse,"Accept-Encoding: gzip, deflate\r\n",NULL,&nSourceLen);
	//HelpCompress_Algorithm_GZipUnCompress((BYTE *)tszSourceBuffer, nSourceLen, (BYTE *)tszDestBuffer, (DWORD *)&nDestLen);
	printf("%s\n", tszSourceBuffer);
	return 0;
}
int Test_NetGetIPAddr()
{
	int nListCount = 0;
	APIHELP_NETCARD** ppSt_NetCard;

	TCHAR tszRemoteBuffer[1024];
	memset(tszRemoteBuffer, '\0', sizeof(tszRemoteBuffer));

	if (!APIHelp_NetWork_GetIPAddr(&ppSt_NetCard, &nListCount, FALSE, tszRemoteBuffer))
	{
		printf("APIHelp_NetWork_GetIPAddr:%lX", APIHelp_GetLastError());
	}
	for (int i = 0; i < nListCount; i++)
	{
		printf("%s %s %s %s %s\n", ppSt_NetCard[i]->tszIFName, ppSt_NetCard[i]->tszIPAddr, ppSt_NetCard[i]->tszBroadAddr, ppSt_NetCard[i]->tszMaskAddr, ppSt_NetCard[i]->tszMacAddr);
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_NetCard, nListCount);
	printf("%s\n", tszRemoteBuffer);
	return 0;
}

void __stdcall NetHelp_HttpGet_Chunked(LPVOID lpszMsgBuffer, int nMsgLen, LPVOID lParam)
{
	printf("%d\n%s\n", nMsgLen, (LPCTSTR)lpszMsgBuffer);
}
int Test_Domain()
{
	APIHELP_DOMAIN st_APIDomain;
	ENUM_APIHELP_DOMAINTYPE enAPIDomain;

	memset(&st_APIDomain, '\0', sizeof(APIHELP_DOMAIN));

	APIHelp_Domain_GetInfo("www.xyry.org", &st_APIDomain, &enAPIDomain);
	printf("APIHelp_Domain_GetInfo:%s,%s,%s,%s,%d\n",st_APIDomain.tszDomainName, st_APIDomain.tszMainDomain, st_APIDomain.tszSubDomain, st_APIDomain.tszTopDomain, enAPIDomain);
	return 0;
}

int main()
{
	Test_Domain();
	Test_NetGetIPAddr();
	Test_HttpRequest();

	return 0;
}

