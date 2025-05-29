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
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_NetHelp/NetHelp_APIHelp.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_APIHelp/APIHelp_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_APIHelp/APIHelp_Error.h"
#ifdef _MSC_BUILD
#ifdef _WIN64
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/NetHelp_APIHelp.lib")
#else
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/NetHelp_APIHelp.lib")
#endif
#endif
#endif
//Linux::g++ -std=c++20 -Wall -g NetHelp_APPHelp.cpp -o NetHelp_APPHelp.exe -lXEngine_BaseLib -lNetHelp_APIHelp

int test_endain()
{
	double nValue64 = 1920;
	XCHAR tszMsgBuffer[XPATH_MAX];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	APIHelp_Endain_ToHexW64((XBYTE*)tszMsgBuffer, *(__int64u*)&nValue64, true);
	return 0;
}

int main()
{
	test_endain();
	return 0;
}

