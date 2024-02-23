#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if 1 == _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_NetHelp/NetHelp_APIHelp.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_APIHelp/APIHelp_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_APIHelp/APIHelp_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/NetHelp_APIHelp.lib")
#endif
#endif
//Linux::g++ -std=c++17 -Wall -g NetHelp_APPApi.cpp -o NetHelp_APPApi.exe -lXEngine_BaseLib -lNetHelp_APIHelp


int Test_Domain()
{
	XCHAR tszUrlPath[MAX_PATH];
	APIHELP_DOMAIN st_APIDomain;
	ENUM_APIHELP_DOMAINTYPE enAPIDomain;

	memset(tszUrlPath, '\0', MAX_PATH);
	memset(&st_APIDomain, '\0', sizeof(APIHELP_DOMAIN));

	APIHelp_Domain_GetInfo("http://www.xyry.org/Api/Task/Query", &st_APIDomain, &enAPIDomain, tszUrlPath);
	printf("APIHelp_Domain_GetInfo:%s,%s,%s,%s,%d\n",st_APIDomain.tszDomainName, st_APIDomain.tszMainDomain, st_APIDomain.tszSubDomain, st_APIDomain.tszTopDomain, enAPIDomain);
	return 0;
}
int Test_Email()
{
	LPCXSTR lpszEMailStr = _X("486179@qq.com");
	XCHAR tszNameStr[MAX_PATH];
	XCHAR tszDomainStr[MAX_PATH];

	memset(tszNameStr, '\0', MAX_PATH);
	memset(tszDomainStr, '\0', MAX_PATH);

	if (APIHelp_EMail_IsEMailAddr(lpszEMailStr))
	{
		APIHelp_EMail_GetName(lpszEMailStr, tszNameStr, tszDomainStr);
		printf("Test_Email:%s,%s", tszNameStr, tszDomainStr);
	}
	return 0;
}

int main()
{
	Test_Domain();
	Test_Email();
	return 0;
}

