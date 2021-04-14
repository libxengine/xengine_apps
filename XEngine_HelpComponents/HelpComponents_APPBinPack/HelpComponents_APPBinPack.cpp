#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/HelpComponents_BINPack.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_HelpComponents/HelpComponents_BINPack/BINPack_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_HelpComponents/HelpComponents_BINPack/BINPack_Error.h"

//g++ -std=gnu++17 -Wall -g HelpComponents_APPBinPack.cpp -o HelpComponents_APPBinPack.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_HelpComponents -lXEngine_BaseLib -lHelpComponents_BINPack -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_HelpComponents,--disable-new-dtags

typedef struct
{
	TCHAR tszFile[10];
	int nType;
}HELP_BINPACK;
BOOL bIsDis = FALSE;

void BINTest_Packet()
{
	// TODO: 在此添加控件通知处理程序代码
	XNETHANDLE xhFile;
	HELP_BINPACK st_BINPack;
	LPCTSTR lpszFile = _T("Pics.xBin");

	if (!BINPack_Packet_Init(&xhFile, lpszFile))
	{
		printf(_T("初始化失败"));
		return;
	}
	memset(&st_BINPack, '\0', sizeof(HELP_BINPACK));
	st_BINPack.nType = 1;
	strcpy(st_BINPack.tszFile, "1.jpg");
	if (!BINPack_Packet_Push(xhFile, _T("1.jpg"), &st_BINPack, sizeof(st_BINPack), NULL, 0, bIsDis))
	{
		printf(_T("打包失败1"));
		return;
	}
	memset(&st_BINPack, '\0', sizeof(HELP_BINPACK));
	st_BINPack.nType = 2;
	strcpy(st_BINPack.tszFile, "2.jpg");
	if (!BINPack_Packet_Push(xhFile, _T("2.jpg"), &st_BINPack, sizeof(st_BINPack), NULL, 0, bIsDis))
	{
		printf(_T("打包失败2"));
		return;
	}
	memset(&st_BINPack, '\0', sizeof(HELP_BINPACK));
	st_BINPack.nType = 3;
	strcpy(st_BINPack.tszFile, "3.jpg");
	if (!BINPack_Packet_Push(xhFile, _T("3.jpg"), &st_BINPack, sizeof(st_BINPack), NULL, 0, bIsDis))
	{
		printf(_T("打包失败3"));
		return;
	}

	LPCTSTR lpszMsg = _T("hello world");
	memset(&st_BINPack, '\0', sizeof(HELP_BINPACK));
	st_BINPack.nType = 4;
	strcpy(st_BINPack.tszFile, "Memory");
	if (!BINPack_Packet_Push(xhFile, NULL, &st_BINPack, sizeof(st_BINPack), lpszMsg, strlen(lpszMsg), TRUE))
	{
		printf(_T("打包失败4"));
		return;
	}
	BINPack_Packet_Close(xhFile);
	printf(_T("打包成功"));
}
void BINTest_UNPack()
{
	// TODO: 在此添加控件通知处理程序代码
	XNETHANDLE xhFile;
	int nCusLen = 4096;

	LPCTSTR lpszFile = _T("Pics.xBin");
	HELP_BINPACK st_BINPack;

	if (!BINPack_UnPack_Init(&xhFile, lpszFile))
	{
		printf(_T("初始化失败"));
		return;
	}
	memset(&st_BINPack, '\0', sizeof(st_BINPack));
	if (!BINPack_UnPack_Get(xhFile, &st_BINPack, &nCusLen, _T("4.jpg"), NULL, 0, bIsDis))
	{
		printf(_T("解包失败1"));
		return;
	}
	nCusLen = 4096;
	memset(&st_BINPack, '\0', sizeof(st_BINPack));
	if (!BINPack_UnPack_Get(xhFile, &st_BINPack, &nCusLen, _T("5.jpg"), NULL, 0, bIsDis))
	{
		printf(_T("解包失败2"));
		return;
	}
	nCusLen = 4096;
	memset(&st_BINPack, '\0', sizeof(st_BINPack));
	if (!BINPack_UnPack_Get(xhFile, &st_BINPack, &nCusLen, _T("6.jpg"), NULL, 0, bIsDis))
	{
		printf(_T("解包失败3"));
		return;
	}
	TCHAR tszMsgBuffer[2048];
	nCusLen = 4096;
	int nLen = 2048;

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_BINPack, '\0', sizeof(st_BINPack));
	if (!BINPack_UnPack_Get(xhFile, &st_BINPack, &nCusLen, NULL, tszMsgBuffer, &nLen, TRUE))
	{
		printf(_T("解包失败4"));
		return;
	}
	BINPack_UnPack_Close(xhFile);
	printf(_T("解包成功"));
}


int main()
{
	BINTest_Packet();
	BINTest_UNPack();
	return 0;
}
