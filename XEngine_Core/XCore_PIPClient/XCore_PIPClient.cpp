#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#else
#include <sys/socket.h>
#endif
#include <stdio.h>
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

int Test_PIPNamed()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszPIPName = _X("\\\\.\\pipe\\MyNamedPipeOne");
#else
	LPCXSTR lpszPIPName = _X("MyNamedPipeOne");
#endif
	LPCXSTR lpszMsgBuffer = _X("hello");
	if (!NetCore_PIPNamed_OPen(lpszPIPName))
	{
		printf("%lX", NetCore_GetLastError());
		return -1;
	}
	NetCore_PIPNamed_Write(lpszPIPName, lpszMsgBuffer, _tcslen(lpszMsgBuffer));

	NetCore_PIPNamed_Close(lpszPIPName);
	return 0;
}
int Test_PIPMailSlot()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszPIPName = _X("\\\\.\\mailslot\\MyMailSlot");
#else
	LPCXSTR lpszPIPName = _X("/MyMailSlot");
#endif
	
#ifndef __APPLE__
	LPCXSTR lpszMsgBuffer = _X("hello");
	if (!NetCore_PIPMailSlot_OPen(lpszPIPName))
	{
		printf("%lX", NetCore_GetLastError());
		return -1;
	}
	NetCore_PIPMailSlot_Write(lpszPIPName, lpszMsgBuffer, _tcslen(lpszMsgBuffer));

	NetCore_PIPMailSlot_Close(lpszPIPName);
#endif
	return 0;
}

int main(int argc, char** argv)
{
	Test_PIPNamed();
	Test_PIPMailSlot();

	return 0;
}