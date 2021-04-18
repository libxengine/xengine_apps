#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVPlayer.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AVPlayer/AVPlayer_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AVPlayer/AVPlayer_Error.h"

//g++ -std=c++17 -Wall -g AVCoder_APPPlayer.cpp -o AVCoder_APPPlayer.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCoder -lXEngine_BaseLib -lXEngine_AVPlayer -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCoder,--disable-new-dtags

int Player_Video()
{
	XNETHANDLE xhPlayer;
	LPCTSTR lpszName = _T("test");
	if (!AVPlayer_Video_Create(&xhPlayer, NULL, lpszName, 1920, 1080))
	{
		return -1;
	}
	LPCTSTR lpszFile = _T("H:\\h264 file\\ds.yuv");
	int nSize = 1920 * 1080 * 3 / 2;
	TCHAR* ptszBuffer = new TCHAR[nSize];
	FILE* pSt_File = fopen(lpszFile, "rb");
	while (1)
	{
		memset(ptszBuffer, '\0', nSize);
		int nRet = fread(ptszBuffer, 1, nSize, pSt_File);
		if (nRet <= 0)
		{
			break;
		}
		AVPlayer_Video_Push(xhPlayer, (uint8_t*)ptszBuffer, 1920);
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}
	AVPlayer_Video_Close(xhPlayer);
	return -2;
}

int main()
{
	Player_Video();

	return 1;
}