#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/HelpComponents_BINPack.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_HelpComponents/HelpComponents_BINPack/BINPack_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_HelpComponents/HelpComponents_BINPack/BINPack_Error.h"

//g++ -std=gnu++17 -Wall -g HelpComponents_APPBinPack.cpp -o HelpComponents_APPBinPack.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_HelpComponents -lXEngine_BaseLib -lHelpComponents_BINPack -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_HelpComponents,--disable-new-dtags
#ifdef _MSC_BUILD
LPCTSTR lpszFile = _T("D:\\xengine_apps\\Debug\\Pics.xBin");
LPCTSTR lpszPacket1 = _T("D:\\xengine_apps\\Debug\\1.png");
LPCTSTR lpszPacket2 = _T("D:\\xengine_apps\\Debug\\2.png");
LPCTSTR lpszPacket3 = _T("D:\\xengine_apps\\Debug\\3.png");
LPCTSTR lpszUNPack1 = _T("D:\\xengine_apps\\Debug\\4.png");
LPCTSTR lpszUNPack2 = _T("D:\\xengine_apps\\Debug\\5.png");
LPCTSTR lpszUNPack3 = _T("D:\\xengine_apps\\Debug\\6.png");
#else
LPCTSTR lpszFile = _T("Pics.xBin");
LPCTSTR lpszPacket1 = _T("1.jpg");
LPCTSTR lpszPacket2 = _T("2.jpg");
LPCTSTR lpszPacket3 = _T("3.jpg");
LPCTSTR lpszUNPack1 = _T("4.jpg");
LPCTSTR lpszUNPack2 = _T("5.jpg");
LPCTSTR lpszUNPack3 = _T("6.jpg");
#endif

void BINTest_Packet()
{
	// TODO: 在此添加控件通知处理程序代码
	XNETHANDLE xhFile;
	if (!BINPack_Packet_Init(&xhFile, lpszFile))
	{
		printf(_T("初始化失败"));
		return;
	}
	if (!BINPack_Packet_Push(xhFile, lpszPacket1, NULL, 0, ENUM_XENGINE_PROTOCOLHDR_CRYPTO_TYPE_UNKNOW, ENUM_XENGINE_PROTOCOLHDR_PAYLOAD_TYPE_PIC))
	{
		printf(_T("打包失败1"));
		return;
	}
	if (!BINPack_Packet_Push(xhFile, lpszPacket2, NULL, 0, ENUM_XENGINE_PROTOCOLHDR_CRYPTO_TYPE_UNKNOW, ENUM_XENGINE_PROTOCOLHDR_PAYLOAD_TYPE_PIC))
	{
		printf(_T("打包失败2"));
		return;
	}
	if (!BINPack_Packet_Push(xhFile, lpszPacket3, NULL, 0, ENUM_XENGINE_PROTOCOLHDR_CRYPTO_TYPE_UNKNOW, ENUM_XENGINE_PROTOCOLHDR_PAYLOAD_TYPE_PIC))
	{
		printf(_T("打包失败3"));
		return;
	}
	if (!BINPack_Packet_Push(xhFile, NULL, _T("hello"), 5))
	{
		printf(_T("打包失败4"));
		return;
	}
	BINPack_Packet_Package(xhFile);
	BINPack_Packet_Close(xhFile);
	printf(_T("打包成功"));
}
void BINTest_UNPack()
{
	// TODO: 在此添加控件通知处理程序代码
	XNETHANDLE xhFile;
	if (!BINPack_UnPack_Init(&xhFile, lpszFile))
	{
		printf(_T("初始化失败"));
		return;
	}
	ENUM_XENGINE_PROTOCOLHDR_CRYPTO_TYPE enEncrypto;
	ENUM_XENGINE_PROTOCOLHDR_PAYLOAD_TYPE enPAYLoad;

	BINPack_UnPack_GetType(xhFile, &enEncrypto, &enPAYLoad);
	if (!BINPack_UnPack_Get(xhFile, lpszUNPack1))
	{
		printf(_T("解包失败1"));
		return;
	}
	if (!BINPack_UnPack_Get(xhFile, lpszUNPack2))
	{
		printf(_T("解包失败2"));
		return;
	}
	if (!BINPack_UnPack_Get(xhFile, lpszUNPack3))
	{
		printf(_T("解包失败3"));
		return;
	}
	int nLen = 4095;
	TCHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	if (!BINPack_UnPack_Get(xhFile, NULL, tszMsgBuffer, &nLen))
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
