#include "../../XEngine_ExampleHdr.h"
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
#include <locale.h>

#if 1 == _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_Core/NetCore_Define.h>
#include <XEngine_Include/XEngine_Core/NetCore_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_Core/XEngine_Core.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Core/NetCore_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Core/NetCore_Error.h"
#ifdef _MSC_BUILD
#ifdef _WIN64
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_Core.lib")
#else
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Core.lib")
#endif
#endif
#endif
//Linux::g++ -std=gnu++17 -Wall -g XCore_APPTest.cpp -o XCore_APPTest.exe -lXEngine_BaseLib -lXEngine_Algorithm -lXEngine_ManagePool -lXEngine_Core 


int Test_Anonymous()
{
	LPCXSTR lpszPIPName = _X("d:\\xengine_apps\\Debug\\Lib_APPAlgorithm.exe");
	LPCXSTR lpszMsgBuffer = _X("hello");
	if (!NetCore_PIPAnonymous_Create(lpszPIPName))
	{
		printf("%lX", NetCore_GetLastError());
		return -1;
	}
	NetCore_PIPAnonymous_Write(lpszPIPName, lpszMsgBuffer, _tcslen(lpszMsgBuffer));
	NetCore_PIPAnonymous_Write(lpszPIPName, lpszMsgBuffer, _tcslen(lpszMsgBuffer));

	int nLen = 1024;
	XCHAR tszMsgBuffer[1024];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	NetCore_PIPAnonymous_Read(lpszPIPName, tszMsgBuffer, &nLen);
	printf("Test_Anonymous:%s\n", tszMsgBuffer);
	NetCore_PIPAnonymous_Close(lpszPIPName);
	return 0;
}
int Test_Named()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszPIPName = _X("\\\\.\\pipe\\MyNamedPipeOne");
#else
	LPCXSTR lpszPIPName = _X("MyNamedPipeOne");
#endif
	if (!NetCore_PIPNamed_Create(lpszPIPName))
	{
		printf("%lX", NetCore_GetLastError());
		return -1;
	}
#ifdef _MSC_BUILD
	NetCore_PIPNamed_WaitConnect(lpszPIPName);
#endif

	int nLen = 1024;
	XCHAR tszMsgBuffer[1024];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	NetCore_PIPNamed_Read(lpszPIPName, tszMsgBuffer, &nLen);
	printf("Test_Named:%s\n", tszMsgBuffer);
	NetCore_PIPNamed_Close(lpszPIPName);
	return 0;
}
int Test_MailSlot()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszPIPName = _X("\\\\.\\mailslot\\MyMailSlot");
#else
	LPCXSTR lpszPIPName = _X("/MyMailSlot");
#endif
	
#ifndef __APPLE__
	if (!NetCore_PIPMailSlot_Create(lpszPIPName))
	{
		printf("%lX", NetCore_GetLastError());
		return -1;
	}

	int nLen = 1024;
	XCHAR tszMsgBuffer[1024];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	while (1)
	{
		if (NetCore_PIPMailSlot_Read(lpszPIPName, tszMsgBuffer, &nLen))
		{
			printf("Test_MailSlot:%s\n", tszMsgBuffer);
			break;
		}
	}

	NetCore_PIPMailSlot_Close(lpszPIPName);
#endif
	return 0;
}
int Test_MMap()
{
	LPCXSTR lpszPIPName = _X("xyry");
	LPCXSTR lpszMsgBuffer = _X("hello");
#ifdef _MSC_BUILD
	LPCXSTR lpszFileName = _X("D:\\xengine_apps\\Debug\\1.txt");
#else
	LPCXSTR lpszFileName = _X("1.txt");
#endif

	if (!NetCore_PIPMMap_Create(lpszPIPName, lpszFileName))
	{
		printf("%lX", NetCore_GetLastError());
		return -1;
	}
	NetCore_PIPMMap_Write(lpszPIPName, (XCHAR*)lpszMsgBuffer, _tcslen(lpszMsgBuffer));
	NetCore_PIPMMap_Write(lpszPIPName, (XCHAR*)lpszMsgBuffer, _tcslen(lpszMsgBuffer), 5);

#ifdef _MSC_BUILD
	XPVOID lPBuffer = NetCore_PIPMMap_GetPointer(lpszPIPName);
	memcpy(lPBuffer, _X("123123"), 6);
	NetCore_PIPMMap_FreePointer(lpszPIPName);
#endif

	XCHAR tszMsgBuffer[1024];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	NetCore_PIPMMap_Read(lpszPIPName, tszMsgBuffer, 10, 0);
	printf("Test_MMap:%s\n", tszMsgBuffer);
	NetCore_PIPMMap_Close(lpszPIPName);
	return 0;
}


int Test_MIPC()
{
#ifndef _MSC_BUILD
	int nKey = 0;
	if (!NetCore_PIPIpc_Create(&nKey, 4096))
	{
		printf("%lX", NetCore_GetLastError());
		return -1;
	}
	printf("Key:%d\n", nKey);

	XCHAR* ptszMsgBuffer = (XCHAR*)NetCore_PIPIpc_Get(nKey);
	_tcsxcpy(ptszMsgBuffer, "123213");
	printf("Test_MIPC:%s\n", ptszMsgBuffer);
	NetCore_PIPIpc_Free(ptszMsgBuffer);
	NetCore_PIPIpc_Close(nKey);
#endif
	return 0;
}
int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	Test_Anonymous();
	Test_MMap();
	Test_MIPC();
	Test_Named();
	//Test_MailSlot();
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}
