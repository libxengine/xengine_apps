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
	double dlTime = 0;
	Algorithm_Calculation_GetTime(xhToken, &dlTime);
	printf("%fl\n", dlTime);

	Algorithm_Calculation_ResetTime(xhToken);
	double dlFlow = 0;
	Algorithm_Calculation_ADDRVFlow(xhToken, 1000);
	Algorithm_Calculation_ADDRVFlow(xhToken, 1000);
	std::this_thread::sleep_for(std::chrono::seconds(2));
	Algorithm_Calculation_GetRVFlow(xhToken, &dlFlow);
	printf("%lf\n", dlFlow);
	Algorithm_Calculation_GetRVFlow(xhToken, &dlFlow, TRUE);
	printf("%lf\n", dlFlow);
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

	__int64 nArrayKey = 123456;
	__int64 nArrayValue[6];
	if (Algorithm_Math_GetValue(nArrayKey, nArrayValue))
	{
		for (int i = 0; i < 6; i++)
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

