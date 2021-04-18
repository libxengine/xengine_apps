#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVCollect.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AVCollect/AVCollect_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AVCollect/AVCollect_Error.h"

//g++ -std=c++17 -Wall -g AVCoder_APPCollect.cpp -o AVCoder_APPCollect.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCoder -lXEngine_BaseLib -lXEngine_AVCollect -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCoder,--disable-new-dtags

FILE* pSt_File;
int nWidth;
int nHeight;
void __stdcall XEngine_AVCollect_Callback(uint8_t* punStringY, int nYLen, uint8_t* punStringU, int nULen, uint8_t* punStringV, int nVLen, LPVOID lParam)
{
	fwrite(punStringY, 1, nWidth * nHeight, pSt_File);
	fwrite(punStringU, 1, nWidth * nHeight / 4, pSt_File);
	fwrite(punStringV, 1, nWidth * nHeight / 4, pSt_File);
	printf("1\n");
}

int main()
{
	XNETHANDLE xhVideo;
	int64_t nBitRate;


#ifdef _WINDOWS
	pSt_File = fopen("H:\\h264 file\\ds.yuv", "wb");
#else
	pSt_File = fopen("ds.yuv", "wb");
#endif

	if (!AVCollect_Screen_Init(&xhVideo, XEngine_AVCollect_Callback))
	{
		printf(_T("初始化失败"));
		return -1;
	}

	AVCollect_Screen_GetInfo(xhVideo, &nWidth, &nHeight, &nBitRate);
	printf("AVCollect_Screen_GetInfo:%d %d %ld\n", nWidth, nHeight, nBitRate);

	AVCollect_Screen_Start(xhVideo);

	std::this_thread::sleep_for(std::chrono::seconds(15));
	AVCollect_Screen_Destory(xhVideo);
	return 0;
}