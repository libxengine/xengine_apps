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

	tstring m_StrSource;
	tstring m_StrDest;

	APIHelp_HttpRequest_Get(lpszUrl, &m_StrSource);
	//APIHelp_HttpRequest_Post(lpszUrl, tszSourceBuffer,&nResponse,"Accept-Encoding: gzip, deflate\r\n",NULL,&nSourceLen);
	//HelpCompress_Algorithm_GZipUnCompress((BYTE *)tszSourceBuffer, nSourceLen, (BYTE *)tszDestBuffer, (DWORD *)&nDestLen);
	printf("%s\n", m_StrSource.c_str());
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
	
	APIHelp_HttpRequest_Create(&xhToken , NetHelp_HttpGet_Chunked);
	APIHelp_HttpRequest_SetUrl(xhToken, "www.xyry.org", "GET");

	tstring m_Str;
	APIHelp_HttpRequest_Excute(xhToken, &m_Str);
	APIHelp_HttpRequest_Close(xhToken);
	return 0;
}

int main()
{
	Test_Domain();
	Test_NetGetIPAddr();
	Test_HttpRequest();
	Test_HttpCustom();
	return 0;
}

