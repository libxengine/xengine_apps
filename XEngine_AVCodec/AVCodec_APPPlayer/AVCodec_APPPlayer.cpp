#ifdef _MSC_BUILD
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
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVPlayer/AVPlayer_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVPlayer/AVPlayer_Error.h"

//Linux::g++ -std=c++17 -Wall -g AVCodec_APPPlayer.cpp -o AVCodec_APPPlayer.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCodec -lXEngine_BaseLib -lXEngine_AVPlayer -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCodec,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g AVCodec_APPPlayer.cpp -o AVCodec_APPPlayer.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_AVCodec -lXEngine_BaseLib -lXEngine_AVPlayer

int Player_Video()
{
	LPCXSTR lpszName = _X("test");

	XHANDLE xhPlayer = AVPlayer_Video_Create(NULL, lpszName, 720, 480);
	if (NULL == xhPlayer)
	{
		return -1;
	}
#ifdef _MSC_BUILD
	LPCXSTR lpszFile = _X("D:\\h264 file\\ds.yuv");
#else
	LPCXSTR lpszFile = _X("./ds.yuv");
#endif
	int nSize = 720 * 480 * 3 / 2;
	XCHAR* ptszBuffer = new XCHAR[nSize];
	FILE* pSt_File = fopen(lpszFile, "rb");
	while (1)
	{
		memset(ptszBuffer, '\0', nSize);
		int nRet = fread(ptszBuffer, 1, nSize, pSt_File);
		if (nRet <= 0)
		{
			break;
		}
		AVPlayer_Video_Push(xhPlayer, (uint8_t*)ptszBuffer, 720);
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}
	AVPlayer_Video_Close(xhPlayer);
	return -2;
}
int Player_Audio()
{
	LPCXSTR lpszName = _X("test");

	XHANDLE xhPlayer = AVPlayer_Audio_Create(44100, 1, 2);
	if (NULL == xhPlayer)
	{
		return -1;
	}
#ifdef _MSC_BUILD
	LPCXSTR lpszFile = _X("D:\\audio\\44.1k_2_16.aac.pcm");
#else
	LPCXSTR lpszFile = _X("44.1k_2_16.aac.pcm");
#endif
	int nSize = 8192;
	XCHAR* ptszBuffer = new XCHAR[nSize];
	FILE* pSt_File = fopen(lpszFile, "rb");
	while (1)
	{
		memset(ptszBuffer, '\0', nSize);
		int nRet = fread(ptszBuffer, 1, nSize, pSt_File);
		if (nRet <= 0)
		{
			break;
		}
		AVPlayer_Audio_Push(xhPlayer, ptszBuffer, nRet);
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}
	AVPlayer_Audio_Close(xhPlayer);
	return -2;
}

int main()
{
	Player_Audio();
	Player_Video();

	return 1;
}