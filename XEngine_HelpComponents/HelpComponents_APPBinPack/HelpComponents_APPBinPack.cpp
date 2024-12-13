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
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_HelpComponents/BINPack_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/BINPack_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_BINPack.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_HelpComponents/HelpComponents_BINPack/BINPack_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_HelpComponents/HelpComponents_BINPack/BINPack_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/HelpComponents_BINPack.lib")
#endif
#endif

//Linux::g++ -std=c++20 -Wall -g HelpComponents_APPBinPack.cpp -o HelpComponents_APPBinPack.exe -lXEngine_BaseLib -lHelpComponents_BINPack 

#ifdef _MSC_BUILD
LPCXSTR lpszFile = _X("D:\\xengine_apps\\Debug\\Pics.xBin");
LPCXSTR lpszPacket1 = _X("D:\\xengine_apps\\Debug\\1.png");
LPCXSTR lpszPacket2 = _X("D:\\xengine_apps\\Debug\\2.png");
LPCXSTR lpszPacket3 = _X("D:\\xengine_apps\\Debug\\3.png");
LPCXSTR lpszUNPack1 = _X("D:\\xengine_apps\\Debug\\4.png");
LPCXSTR lpszUNPack2 = _X("D:\\xengine_apps\\Debug\\5.png");
LPCXSTR lpszUNPack3 = _X("D:\\xengine_apps\\Debug\\6.png");
#else
LPCXSTR lpszFile = _X("./Pics.xBin");
LPCXSTR lpszPacket1 = _X("./1.png");
LPCXSTR lpszPacket2 = _X("./2.png");
LPCXSTR lpszPacket3 = _X("./3.png");
LPCXSTR lpszUNPack1 = _X("./4.png");
LPCXSTR lpszUNPack2 = _X("./5.png");
LPCXSTR lpszUNPack3 = _X("./6.png");
#endif

void BINTest_Packet()
{
	// TODO: 在此添加控件通知处理程序代码
	XNETHANDLE xhFile;
	if (!BINPack_Packet_Init(&xhFile, lpszFile))
	{
		printf(_X("初始化失败"));
		return;
	}
	if (!BINPack_Packet_Push(xhFile, lpszPacket1, NULL, 0, ENUM_XENGINE_PROTOCOLHDR_CRYPTO_TYPE_UNKNOW, ENUM_XENGINE_PROTOCOLHDR_PAYLOAD_TYPE_PIC))
	{
		printf(_X("打包失败1"));
		return;
	}
	if (!BINPack_Packet_Push(xhFile, lpszPacket2, NULL, 0, ENUM_XENGINE_PROTOCOLHDR_CRYPTO_TYPE_UNKNOW, ENUM_XENGINE_PROTOCOLHDR_PAYLOAD_TYPE_PIC))
	{
		printf(_X("打包失败2"));
		return;
	}
	if (!BINPack_Packet_Push(xhFile, lpszPacket3, NULL, 0, ENUM_XENGINE_PROTOCOLHDR_CRYPTO_TYPE_UNKNOW, ENUM_XENGINE_PROTOCOLHDR_PAYLOAD_TYPE_PIC))
	{
		printf(_X("打包失败3"));
		return;
	}
	if (!BINPack_Packet_Push(xhFile, NULL, _X("hello"), 5))
	{
		printf(_X("打包失败4"));
		return;
	}
	BINPack_Packet_Package(xhFile);
	BINPack_Packet_Close(xhFile);
	printf(_X("打包成功"));
}
void BINTest_UNPack()
{
	// TODO: 在此添加控件通知处理程序代码
	XNETHANDLE xhFile;
	if (!BINPack_UnPack_Init(&xhFile, lpszFile))
	{
		printf(_X("初始化失败"));
		return;
	}
	ENUM_XENGINE_PROTOCOLHDR_CRYPTO_TYPE enEncrypto;
	ENUM_XENGINE_PROTOCOLHDR_PAYLOAD_TYPE enPAYLoad;

	BINPack_UnPack_GetType(xhFile, &enEncrypto, &enPAYLoad);
	if (!BINPack_UnPack_Get(xhFile, lpszUNPack1))
	{
		printf(_X("解包失败1"));
		return;
	}
	if (!BINPack_UnPack_Get(xhFile, lpszUNPack2))
	{
		printf(_X("解包失败2"));
		return;
	}
	if (!BINPack_UnPack_Get(xhFile, lpszUNPack3))
	{
		printf(_X("解包失败3"));
		return;
	}
	int nLen = 4095;
	XCHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	if (!BINPack_UnPack_Get(xhFile, NULL, tszMsgBuffer, &nLen))
	{
		printf(_X("解包失败4"));
		return;
	}
	BINPack_UnPack_Close(xhFile);
	printf(_X("解包成功"));
}

int main()
{
	BINTest_Packet();
	BINTest_UNPack();
	return 0;
}
