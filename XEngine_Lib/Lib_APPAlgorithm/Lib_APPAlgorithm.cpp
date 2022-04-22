#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#include <time.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Algorithm.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_Algorithm/Algorithm_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_Algorithm/Algorithm_Error.h"

//g++ -std=gnu++11 -Wall -g Lib_APPAlgorithm.cpp -o Lib_APPAlgorithm.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -lXEngine_Algorithm -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib

int main()
{
	int nIntA = 0;
	int nIntB = 0;
	LPCTSTR lpszIntBuffer = _T("123456798");
	Algorithm_String_GetMemoryInt(lpszIntBuffer, 3, 4, &nIntA);
	Algorithm_String_GetMemoryInt(lpszIntBuffer, 4, 6, &nIntB);

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
	printf("%llu\n", nTime);

	Algorithm_Calculation_ResetTime(xhToken);
	__int64u nFlow = 0;
	Algorithm_Calculation_ADDRVFlow(xhToken, 1000);
	Algorithm_Calculation_ADDRVFlow(xhToken, 1000);
	std::this_thread::sleep_for(std::chrono::seconds(2));
	Algorithm_Calculation_GetRVFlow(xhToken, &nFlow);
	printf("%llu\n", nFlow);
	Algorithm_Calculation_GetRVFlow(xhToken, &nFlow, TRUE);
	printf("%llu\n", nFlow);
	Algorithm_Calculation_Close(xhToken);

	int nPos = 0;
	LPCTSTR lpszSourceStr = _T("123hel54124hello666");
	LPCTSTR lpszFindStr = _T("hell");
	if (Algorithm_String_XFastMatch(lpszSourceStr, lpszFindStr, &nPos))
	{
		printf("Algorithm_String_XFastMatch:%d\n", nPos);
	}

	int nSwapA = 100;
	int nSwapB = 200;
	if (Algorithm_Math_Swap(&nSwapA, &nSwapB))
	{
		printf("%d = %d\n", nSwapA, nSwapB);
	}

	int nCount = 0;
	__int64 nArrayKey = 1230456;
	__int64 nArrayValue[10];
	if (Algorithm_Math_GetValue(nArrayKey, nArrayValue, &nCount))
	{
		for (int i = 0; i < nCount; i++)
		{
			printf("%lld\n", nArrayValue[i]);
		}
	}

	int nInBit = 10;
	int nOutBit = 0;
	if (Algorithm_Math_GetBit(&nInBit, 3, &nOutBit))
	{
		printf("%d\n", nOutBit);
	}

	if (Algorithm_Math_SetBit(&nInBit, 4, 7))
	{
		printf("%d\n", nInBit);
	}

	return 0;
}

