#ifdef _WINDOWS
#include <stdio.h>
#include <Windows.h>
#include <tchar.h>
#include <locale.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Core.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#endif
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_Core/NetCore_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_Core/NetCore_Error.h"

//g++ -std=gnu++17 -Wall -g XCore_PIPClient.cpp -o XCore_PIPClient.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -lXEngine_BaseLib -lXEngine_Algorithm -lXEngine_ManagePool -lXEngine_Core -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core,--disable-new-dtags

int Test_PIPNamed()
{
#ifdef _WINDOWS
	LPCTSTR lpszPIPName = _T("\\\\.\\pipe\\MyNamedPipeOne");
#else
	LPCTSTR lpszPIPName = _T("MyNamedPipeOne");
#endif
	LPCTSTR lpszMsgBuffer = _T("hello");
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
#ifdef _WINDOWS
	LPCTSTR lpszPIPName = _T("\\\\.\\mailslot\\MyMailSlot");
#else
	LPCTSTR lpszPIPName = _T("/MyMailSlot");
#endif
	
	LPCTSTR lpszMsgBuffer = _T("hello");
	if (!NetCore_PIPMailSlot_OPen(lpszPIPName))
	{
		printf("%lX", NetCore_GetLastError());
		return -1;
	}
	NetCore_PIPMailSlot_Write(lpszPIPName, lpszMsgBuffer, _tcslen(lpszMsgBuffer));

	NetCore_PIPNamed_Close(lpszPIPName);
	return 0;
}

int main(int argc, char** argv)
{
	Test_PIPNamed();
	Test_PIPMailSlot();

	return 0;
}