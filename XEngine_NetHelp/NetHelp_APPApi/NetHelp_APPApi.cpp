#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _XENGINE_USER_DIR_SYSTEM
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
//Linux::g++ -std=c++17 -Wall -g NetHelp_APPApi.cpp -o NetHelp_APPApi.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp -lXEngine_BaseLib -lNetHelp_APIHelp -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g NetHelp_APPApi.cpp -o NetHelp_APPApi.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_NetHelp -lXEngine_BaseLib -lNetHelp_APIHelp


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

int main()
{
	Test_Domain();

	double nValue64 = 1920;
	XCHAR tszMsgBuffer[MAX_PATH];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	APIHelp_NetWork_ToHexW64((XBYTE*)tszMsgBuffer, *(__int64u*)&nValue64, true);

	return 0;
}

