#ifdef _WINDOWS
#include <stdio.h>
#include <Windows.h>
#include <tchar.h>
#include <locale.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Core.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#endif
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Core/NetCore_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Core/NetCore_Error.h"

//Linux::g++ -std=gnu++17 -Wall -g XCore_APPTest.cpp -o XCore_APPTest.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -lXEngine_BaseLib -lXEngine_Algorithm -lXEngine_ManagePool -lXEngine_Core -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core,--disable-new-dtags
//Macos::g++ -std=gnu++17 -Wall -g XCore_APPTest.cpp -o XCore_APPTest.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_Core -lXEngine_BaseLib -lXEngine_Algorithm -lXEngine_ManagePool -lXEngine_Core


int Test_Anonymous()
{
	LPCTSTR lpszPIPName = _T("H:\\XEngine_Apps\\Debug\\Lib_APPAlgorithm.exe");
	LPCTSTR lpszMsgBuffer = _T("hello");
	if (!NetCore_PIPAnonymous_Create(lpszPIPName))
	{
		printf("%lX", NetCore_GetLastError());
		return -1;
	}
	NetCore_PIPAnonymous_Write(lpszPIPName, lpszMsgBuffer, _tcslen(lpszMsgBuffer));
	NetCore_PIPAnonymous_Write(lpszPIPName, lpszMsgBuffer, _tcslen(lpszMsgBuffer));

	int nLen = 1024;
	TCHAR tszMsgBuffer[1024];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	NetCore_PIPAnonymous_Read(lpszPIPName, tszMsgBuffer, &nLen);
	printf("Test_Anonymous:%s\n", tszMsgBuffer);
	NetCore_PIPAnonymous_Close(lpszPIPName);
	return 0;
}
int Test_Named()
{
#ifdef _WINDOWS
	LPCTSTR lpszPIPName = _T("\\\\.\\pipe\\MyNamedPipeOne");
#else
	LPCTSTR lpszPIPName = _T("MyNamedPipeOne");
#endif
	if (!NetCore_PIPNamed_Create(lpszPIPName))
	{
		printf("%lX", NetCore_GetLastError());
		return -1;
	}
#ifdef _WINDOWS
	NetCore_PIPNamed_WaitConnect(lpszPIPName);
#endif

	int nLen = 1024;
	TCHAR tszMsgBuffer[1024];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	NetCore_PIPNamed_Read(lpszPIPName, tszMsgBuffer, &nLen);
	printf("Test_Named:%s\n", tszMsgBuffer);
	NetCore_PIPNamed_Close(lpszPIPName);
	return 0;
}
int Test_MailSlot()
{
#ifdef _WINDOWS
	LPCTSTR lpszPIPName = _T("\\\\.\\mailslot\\MyMailSlot");
#else
	LPCTSTR lpszPIPName = _T("/MyMailSlot");
#endif
	
#ifndef __APPLE__
	if (!NetCore_PIPMailSlot_Create(lpszPIPName))
	{
		printf("%lX", NetCore_GetLastError());
		return -1;
	}

	int nLen = 1024;
	TCHAR tszMsgBuffer[1024];
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
	LPCTSTR lpszPIPName = _T("xyry");
	LPCTSTR lpszMsgBuffer = _T("hello");
	LPCTSTR lpszFileName = _T("1.txt");

	if (!NetCore_PIPMMap_Create(lpszPIPName, lpszFileName))
	{
		printf("%lX", NetCore_GetLastError());
		return -1;
	}
	NetCore_PIPMMap_Write(lpszPIPName, (TCHAR*)lpszMsgBuffer, _tcslen(lpszMsgBuffer));
	NetCore_PIPMMap_Write(lpszPIPName, (TCHAR*)lpszMsgBuffer, _tcslen(lpszMsgBuffer), 5);

#ifdef _WINDOWS
	LPVOID lPBuffer = NetCore_PIPMMap_GetPointer(lpszPIPName);
	memcpy(lPBuffer, _T("123123"), 6);
	NetCore_PIPMMap_FreePointer(lpszPIPName);
#endif

	TCHAR tszMsgBuffer[1024];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	NetCore_PIPMMap_Read(lpszPIPName, tszMsgBuffer, 10, 0);
	printf("Test_MMap:%s\n", tszMsgBuffer);
	NetCore_PIPMMap_Close(lpszPIPName);
	return 0;
}


int Test_MIPC()
{
#ifndef _WINDOWS
	int nKey = 0;
	if (!NetCore_PIPIpc_Create(&nKey, 4096))
	{
		printf("%lX", NetCore_GetLastError());
		return -1;
	}
	printf("Key:%d\n", nKey);

	TCHAR* ptszMsgBuffer = (TCHAR*)NetCore_PIPIpc_Get(nKey);
	_tcscpy(ptszMsgBuffer, "123213");
	printf("Test_MIPC:%s\n", ptszMsgBuffer);
	NetCore_PIPIpc_Free(ptszMsgBuffer);
	NetCore_PIPIpc_Close(nKey);
#endif
	return 0;
}
int main()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	Test_Anonymous();
	Test_MMap();
	Test_MIPC();
	Test_Named();
	//Test_MailSlot();
#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}
