#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#include <time.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
using namespace std;
#if 1 == _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_BaseLib/Algorithm_Define.h>
#include <XEngine_Include/XEngine_BaseLib/Algorithm_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_Algorithm.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_Algorithm/Algorithm_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_Algorithm/Algorithm_Error.h"
#ifdef _MSC_BUILD
#ifdef _WIN64
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_Algorithm.lib")
#else
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Algorithm.lib")
#endif
#endif
#endif

//Linux Macos:g++ -std=c++20 -Wall -g Lib_APPAlgorithm.cpp -o Lib_APPAlgorithm.exe -lXEngine_Algorithm

void XCALLBACK Test_CBPassive(XHANDLE xhToken, bool bSDFlow, bool bRVFlow, bool bTime, __int64u nSDFlow, __int64u nRVFlow, __int64u nTimeFlow, XPVOID lParam)
{
	XCHAR tszClientAddr[128];
	memset(tszClientAddr, '\0', 128);

	_tcsxcpy(tszClientAddr, (LPCXSTR)lParam);
	printf("%s\n", tszClientAddr);
	return;
}

int Test_Calulation()
{
	XHANDLE xhToken = Algorithm_Calculation_Create(3);
	if (NULL == xhToken)
	{
		return 0;
	}
	for (int i = 0; i < 10; i++)
	{
		Algorithm_Calculation_SetTime(xhToken);
	}
	std::this_thread::sleep_for(std::chrono::seconds(2));
	__int64u nTime = 0;
	Algorithm_Calculation_GetTime(xhToken, &nTime);
	_xtprintf(_X("%llu\n"), nTime);

	Algorithm_Calculation_Reset(xhToken);

	for (int i = 0; i < 10; i++)
	{
		__int64u nFlow = 0;
		Algorithm_Calculation_ADDRVFlow(xhToken, rand());
		Algorithm_Calculation_ADDRVFlow(xhToken, rand());
		std::this_thread::sleep_for(std::chrono::seconds(2));
		Algorithm_Calculation_GetRVFlow(xhToken, &nFlow);
		_xtprintf(_X("%llu\n"), nFlow);
		Algorithm_Calculation_GetRVFlow(xhToken, &nFlow, true);
		_xtprintf(_X("%llu\n"), nFlow);
	}

	Algorithm_Calculation_Reset(xhToken);
	LPCXSTR lpszIPAddr = _X("127.0.0.1");
	Algorithm_Calculation_PassiveOPen(xhToken, Test_CBPassive, 1024, 0, 0, false, (XPVOID)lpszIPAddr);
	for (int i = 0; i < 500; i++)
	{
		Algorithm_Calculation_ADDSDFlow(xhToken, rand());
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}
	std::this_thread::sleep_for(std::chrono::seconds(5));
	Algorithm_Calculation_PassiveClose(xhToken);
	Algorithm_Calculation_Close(xhToken);

	return 0;
}
int main()
{
	//Test_Calulation();
	int nIntA = 0;
	int nIntB = 0;
	LPCXSTR lpszIntBuffer = _X("123456798");
	Algorithm_String_GetMemoryInt(lpszIntBuffer, 3, 4, &nIntA);
	Algorithm_String_GetMemoryInt(lpszIntBuffer, 4, 6, &nIntB);

	int nPos = 0;
	LPCXSTR lpszSourceStr = _X("123helhello54124666");
	LPCXSTR lpszFindStr = _X("hell");
	if (Algorithm_String_XFastMatch(lpszSourceStr, lpszFindStr, &nPos))
	{
		_xtprintf(_X("Algorithm_String_XFastMatch:%d,%s\n"), nPos, lpszSourceStr + nPos);
	}

	int nSwapA = 100;
	int nSwapB = 200;
	if (Algorithm_Math_Swap(&nSwapA, &nSwapB))
	{
		_xtprintf(_X("%d = %d\n"), nSwapA, nSwapB);
	}

	int nCount = 0;
	__int64x nArrayKey = 1230456;
	__int64x nArrayValue[10];
	if (Algorithm_Math_GetValue(nArrayKey, nArrayValue, &nCount))
	{
		for (int i = 0; i < nCount; i++)
		{
			_xtprintf(_X("%lld\n"), nArrayValue[i]);
		}
	}

	int nInBit = 10;
	int nOutBit = 0;
	if (Algorithm_Math_GetBit(&nInBit, 3, &nOutBit))
	{
		_xtprintf(_X("%d\n"), nOutBit);
	}

	if (Algorithm_Math_SetBit(&nInBit, 4, 7))
	{
		_xtprintf(_X("%d\n"), nInBit);
	}

	return 0;
}

