#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVCollect.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AVCollect/AVCollect_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AVCollect/AVCollect_Error.h"

//Linux::g++ -std=c++17 -Wall -g AVCoder_APPCollect.cpp -o AVCoder_APPCollect.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCoder -lXEngine_BaseLib -lXEngine_AVCollect -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCoder,--disable-new-dtags
//MacOS::g++ -std=c++17 -Wall -g AVCoder_APPCollect.cpp -o AVCoder_APPCollect.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_AVCoder -lXEngine_BaseLib -lXEngine_AVCollect

FILE* pSt_File;
int nWidth;
int nHeight;
void __stdcall XEngine_AVCollect_CBVideo(uint8_t* punStringY, int nYLen, uint8_t* punStringU, int nULen, uint8_t* punStringV, int nVLen, LPVOID lParam)
{
	fwrite(punStringY, 1, nWidth * nHeight, pSt_File);
	fwrite(punStringU, 1, nWidth * nHeight / 4, pSt_File);
	fwrite(punStringV, 1, nWidth * nHeight / 4, pSt_File);
	printf("%d %d %d\n", nYLen, nULen, nVLen);
}
void __stdcall XEngine_AVCollect_CBAudio(uint8_t* punStringAudio, int nVLen, LPVOID lParam)
{

}
int main()
{
	XNETHANDLE xhVideo;
	XNETHANDLE xhAudio;
	int64_t nBitRate;
#ifdef _WINDOWS
	pSt_File = fopen("D:\\xengine_apps\\Debug\\ds.yuv", "wb");
#else
	pSt_File = fopen("./ds.yuv", "wb");
#endif
	AVCOLLECT_SCREENINFO st_AVScreen;
	memset(&st_AVScreen, '\0', sizeof(AVCOLLECT_SCREENINFO));

	st_AVScreen.nFrameRate = 24;
	strcpy(st_AVScreen.tszVideoDev, "video=screen-capture-recorder");
	//strcpy(st_AVScreen.tszVideoSize, _T("800x600"));

	if (!AVCollect_Video_Init(&xhVideo, &st_AVScreen, XEngine_AVCollect_CBVideo))
	{
		printf(_T("初始化失败"));
		return -1;
	}
// 	if (!AVCollect_Audio_Init(&xhAudio,_T("耳机 (Redmi AirDots 3 Pro Hands-Free AG Audio)"), XEngine_AVCollect_CBAudio))
// 	{
// 		printf(_T("初始化失败"));
// 		return -1;
// 	}

	AVCollect_Video_GetInfo(xhVideo, &nWidth, &nHeight, &nBitRate);
	printf("AVCollect_Screen_GetInfo:%d %d %lld\n", nWidth, nHeight, nBitRate);

	ENUM_AVCOLLECT_AUDIOSAMPLEFORMAT enAVSampleFmt;
	__int64x nARate = 0;
	int nSampleRate = 0;
	int nChannels = 0;
	//AVCollect_Audio_GetInfo(xhAudio, &enAVSampleFmt, &nARate, &nSampleRate, &nChannels);
	//printf("AVCollect_Audio_GetInfo:%d %lld %d %ld\n", enAVSampleFmt, nARate, nSampleRate, nChannels);

	AVCollect_Video_Start(xhVideo);

	while (1)
	{
		std::this_thread::sleep_for(std::chrono::seconds(15));
	}
	AVCollect_Video_Destory(xhVideo);
	return 0;
}