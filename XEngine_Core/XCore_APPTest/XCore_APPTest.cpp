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
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_Core/NetCore_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_Core/NetCore_Error.h"

//g++ -std=gnu++17 -Wall -g XCore_APPTest.cpp -o XCore_APPTest.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -lXEngine_BaseLib -lXEngine_Algorithm -lXEngine_ManagePool -lXEngine_Core -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core,--disable-new-dtags

int Test_ClipBoard()
{
#ifdef _WINDOWS
	LPCTSTR lpszPIPName = _T("H:\\XEngine_Apps\\Debug\\Test_CorePIPAnonymous.exe");
	NetCore_PIPClipBoard_Clear();
	if (!NetCore_PIPClipBoard_Set(lpszPIPName, _tcslen(lpszPIPName)))
	{
		printf("%lX", NetCore_GetLastError());
		return -1;
	}
	int nLen = 1024;
	TCHAR tszMsgBuffer[1024];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	NetCore_PIPClipBoard_Get(tszMsgBuffer, &nLen);
	printf("Test_ClipBoard:%s\n", tszMsgBuffer);
#endif
	return 0;
}
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

int Test_DataCache()
{
	XNETHANDLE xhToken;
	LPCTSTR lpszClientID = _T("qyt");

	if (!NetCore_DataCacheEx_Init(&xhToken, 3, 0))
	{
		printf("NetCore_DataCacheEx_Init:error %lX\n", NetCore_GetLastError());
		return -1;
	}
	if (!NetCore_DataCacheEx_Create(xhToken, lpszClientID))
	{
		printf("NetCore_DataCacheEx_Create:error %lX\n", NetCore_GetLastError());
		return -1;
	}

	for (int i = 0; i < 5; i++)
	{
		LPCTSTR lpszMsgBuffer = _T("hello world\n");
		int nLen = _tcslen(lpszMsgBuffer);

		if (!NetCore_DataCacheEx_Send(xhToken, lpszClientID, lpszMsgBuffer, nLen))
		{
			printf("NetCore_DataCacheEx_Send:error %lX\n", NetCore_GetLastError());
			return -1;
		}
	}

	int nIndex = 1;
	for (int i = 0; i < 5; i++)
	{
		int nMsgLen = 2048;
		TCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		if (NetCore_DataCacheEx_Recv(xhToken, lpszClientID, tszMsgBuffer, &nMsgLen, &nIndex))
		{
			printf("NetCore_DataCacheEx_Recv: %s\n", tszMsgBuffer);
		}
		else
		{
			printf("NetCore_DataCacheEx_Recv:error %lX\n", NetCore_GetLastError());
		}
		nIndex++;
	}
	NetCore_DataCacheEx_Close(xhToken, lpszClientID);
	NetCore_DataCacheEx_Destory(xhToken);

	return 0;
}
int main()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	Test_DataCache();
	Test_ClipBoard();
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
