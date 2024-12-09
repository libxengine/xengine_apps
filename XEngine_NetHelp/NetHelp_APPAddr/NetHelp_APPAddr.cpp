#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#else
#include <sys/socket.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
using namespace std;
#if 1 == _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIAddr_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIAddr_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_NetHelp/NetHelp_APIAddr.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_APIAddr/APIAddr_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_APIAddr/APIAddr_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/NetHelp_APIAddr.lib")
#endif
#endif

//Linux MACOS:g++ -std=c++20 -Wall -g NetHelp_APPAddr.cpp -o NetHelp_APPAddr.exe -lXEngine_BaseLib  -lNetHelp_APIAddr


int TestAddrLib()
{
	int nPort = 0;
	LPCXSTR lpszIPV4Convert = _X("192.168.*.10");

	LPCXSTR lpszIPV61 = _X("2031:0010:1F1F:0200:*:0100:11A0:ADDF");
	LPCXSTR lpszIPV62 = _X("1254:1800:200C::417A:AC11");
	LPCXSTR lpszIPV63 = _X("1080::8:800:200C:417A");
	LPCXSTR lpszIPV64 = _X("ACC::1101");
	LPCXSTR lpszIPV65 = _X("::1");
	LPCXSTR lpszIPV66 = _X("::192:168:1:54");
	XCHAR tszIPV6Addr[] = _X("1080:0:0:0:8:800:200C:5000");

	ENUM_NETHELP_APIADDR_IP_TYPE enIPType;
	APIADDR_IPADDR st_LibAddr;
	memset(&st_LibAddr, '\0', sizeof(APIADDR_IPADDR));

    APIAddr_IPAddr_IsIPV4Addr(lpszIPV4Convert, &st_LibAddr);
	APIAddr_IPAddr_GetIPV4Type(&st_LibAddr, &enIPType);

	memset(&st_LibAddr, '\0', sizeof(APIADDR_IPADDR));
	APIAddr_IPAddr_IsIPV6Addr(lpszIPV61, &st_LibAddr);

	memset(&st_LibAddr, '\0', sizeof(APIADDR_IPADDR));
	APIAddr_IPAddr_IsIPV6Addr(lpszIPV62, &st_LibAddr);

	memset(&st_LibAddr, '\0', sizeof(APIADDR_IPADDR));
	APIAddr_IPAddr_IsIPV6Addr(lpszIPV63, &st_LibAddr);

	memset(&st_LibAddr, '\0', sizeof(APIADDR_IPADDR));
	APIAddr_IPAddr_IsIPV6Addr(lpszIPV64, &st_LibAddr);

	XCHAR tszIPAddr[128];
	memset(tszIPAddr, '\0', sizeof(tszIPAddr));
	APIAddr_IPAddr_ExpIPV6Addr(&st_LibAddr, tszIPAddr);
	memset(tszIPAddr, '\0', sizeof(tszIPAddr));
	APIAddr_IPAddr_ComIPV6Addr(&st_LibAddr, tszIPAddr);

	memset(&st_LibAddr, '\0', sizeof(APIADDR_IPADDR));
	APIAddr_IPAddr_IsIPV6Addr(lpszIPV65, &st_LibAddr);

	memset(&st_LibAddr, '\0', sizeof(APIADDR_IPADDR));
	APIAddr_IPAddr_IsIPV6Addr(lpszIPV66, &st_LibAddr);

	APIAddr_IPAddr_SegAddr(tszIPV6Addr, &nPort);
	return 1;
}
int test_ipaddr()
{
	XUINT nIPAddr = 0;
	LPCXSTR lpszIPAddr = _X("192.168.0.1");
	XCHAR tszIPAddr[128] = {};

	APIAddr_IPAddr_IPConvertInt(lpszIPAddr, &nIPAddr);
	APIAddr_IPAddr_IPConvertStr(&nIPAddr, tszIPAddr);

	printf("IPConvert:%d %s\n", nIPAddr, tszIPAddr);
	return 0;
}
int Test_Domain()
{
	XCHAR tszUrlPath[MAX_PATH];
	APIADDR_DOMAIN st_APIDomain;
	ENUM_NETHELP_APIADDR_DOMAIN_TYPE enAPIDomain;

	memset(tszUrlPath, '\0', MAX_PATH);
	memset(&st_APIDomain, '\0', sizeof(APIADDR_DOMAIN));

	APIAddr_Domain_GetInfo("http://www.xyry.org/Api/Task/Query", &st_APIDomain, &enAPIDomain, tszUrlPath);
	printf("APIHelp_Domain_GetInfo:%s,%s,%s,%s,%d\n", st_APIDomain.tszDomainName, st_APIDomain.tszMainDomain, st_APIDomain.tszSubDomain, st_APIDomain.tszTopDomain, enAPIDomain);
	return 0;
}
int Test_Email()
{
	LPCXSTR lpszEMailStr = _X("486179@qq.com");
	XCHAR tszNameStr[MAX_PATH];
	XCHAR tszDomainStr[MAX_PATH];

	memset(tszNameStr, '\0', MAX_PATH);
	memset(tszDomainStr, '\0', MAX_PATH);

	if (APIAddr_EMail_IsEMailAddr(lpszEMailStr))
	{
		APIAddr_EMail_GetName(lpszEMailStr, tszNameStr, tszDomainStr);
		printf("Test_Email:%s,%s", tszNameStr, tszDomainStr);
	}
	return 0;
}

int main()
{
	TestAddrLib();
	test_ipaddr();
	Test_Domain();
	Test_Email();
	return 0;
}