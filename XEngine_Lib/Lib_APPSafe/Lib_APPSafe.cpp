#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <locale.h>
#if 1 == _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_BaseLib/BaseSafe_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseSafe_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseSafe.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseSafe/BaseSafe_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseSafe/BaseSafe_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseSafe.lib")
#endif
#endif

//Linux MacOS:g++ -std=c++17 -Wall -g Lib_APPSafe.cpp -o Lib_APPSafe.exe -lXEngine_BaseSafe

int main()
{
	XCHAR tszDestStr[8] = {};
	LPCXSTR lpszSourceStr = _X("123456789");

	_xstrcpy(tszDestStr, lpszSourceStr, sizeof(tszDestStr));
	printf("%s\n", tszDestStr);
	return 0;
}