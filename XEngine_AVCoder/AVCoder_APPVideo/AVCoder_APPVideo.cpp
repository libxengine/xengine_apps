#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_VideoCoder.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AVCollect/AVCollect_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_VideoCoder/VideoCoder_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_VideoCoder/VideoCoder_Error.h"

//Linux::g++ -std=c++17 -Wall -g AVCoder_APPVideo.cpp -o AVCoder_APPVideo.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCoder -lXEngine_BaseLib -lXEngine_VideoCoder -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCoder,--disable-new-dtags
//MacOS::g++ -std=c++17 -Wall -g AVCoder_APPVideo.cpp -o AVCoder_APPVideo.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_AVCoder -lXEngine_BaseLib -lXEngine_VideoCoder

XNETHANDLE xhDeVideo;
XNETHANDLE xhEnVideo;
XNETHANDLE xhFilterVideo;
FILE* pSt_YUVFile;
FILE* pSt_264File;

void __stdcall VideoCodec_Stream_Callback(XNETHANDLE xhVideo, uint8_t* pszYBuffer, uint8_t* pszUBuffer, uint8_t* pszVBuffer, int nYLen, int nULen, int nVLen, AVCODEC_VIDEO_INFO* pSt_VideoInfo, LPVOID lParam)
{
	//printf("%llu: %d number:%d nWidth:%d nHeight\n", xhVideo, pSt_VideoInfo->en_FrameType, pSt_VideoInfo->nWidth, pSt_VideoInfo->nHeight);
	//fwrite(pszYBuffer, 1, nYLen, pSt_YUVFile);
	//fwrite(pszUBuffer, 1, nULen, pSt_YUVFile);
	//fwrite(pszVBuffer, 1, nVLen, pSt_YUVFile);
	int nLen = 1920 * 1080 * 3;
	int nFilterLen = 1920 * 1080 * 3;
	uint8_t* ptszEnBuffer = (uint8_t*)malloc(nLen);
	uint8_t* ptszFilterBuffer = (uint8_t*)malloc(nFilterLen);

	memset(ptszEnBuffer, '\0', nLen);
	memset(ptszFilterBuffer, '\0', nFilterLen);

	nLen = 0;

	if (VideoCodec_Help_FilterCvt(xhFilterVideo, pszYBuffer, pszUBuffer, pszVBuffer, nYLen, nULen, nVLen, ptszFilterBuffer, &nFilterLen))
	{
		int nRet = fwrite(ptszFilterBuffer, 1, nFilterLen, pSt_YUVFile);
		printf("%d == %d\n", nFilterLen, nRet);

		VideoCodec_Stream_EnCodec(xhEnVideo, ptszFilterBuffer, NULL, NULL, nFilterLen, 0, 0, ptszEnBuffer, &nLen);
		if (nLen > 0)
		{
			fwrite(ptszEnBuffer, 1, nLen, pSt_264File);
		}
	}
	free(ptszEnBuffer);
	free(ptszFilterBuffer);
}
int Test_CodecFilter()
{
#ifdef _WINDOWS
	FILE* pSt_File = _tfopen("H:\\h264 file\\1080P.264", "rb");
	pSt_264File = _tfopen("H:\\h264 file\\en.h264", "wb");
	pSt_YUVFile = _tfopen("H:\\h264 file\\ds.yuv", "wb");
#else
	FILE* pSt_File = fopen("1080P.264", "rb");
	pSt_264File = fopen("en.h264", "wb");
	pSt_YUVFile = fopen("ds.yuv", "wb");
#endif

	if (!VideoCodec_Stream_EnInit(&xhEnVideo, 1920, 1080, ENUM_ENTENGINE_AVCODEC_VEDIO_TYPE_H264))
	{
		printf("VideoCodec_Stream_EnInit\n");
		return -1;
	}
	if (!VideoCodec_Stream_DeInit(&xhDeVideo, ENUM_ENTENGINE_AVCODEC_VEDIO_TYPE_H264, VideoCodec_Stream_Callback))
	{
		printf("VideoCodec_Stream_DeInit\n");
		return -1;
	}
	const char* filter_descr = "drawtext=fontfile=Arial.ttf:fontcolor=green:fontsize=30:x=100:y=10:text='www.xyry.org'";
	VideoCodec_Help_FilterInit(&xhFilterVideo, filter_descr, 1920, 1080);
	while (1)
	{
		TCHAR tszBuffer[10240];
		memset(tszBuffer, '\0', sizeof(tszBuffer));
		int nRet = fread(tszBuffer, 1, 10240, pSt_File);
		if (nRet <= 0)
		{
			break;
		}
		if (!VideoCodec_Stream_DeCodec(xhDeVideo, (uint8_t*)tszBuffer, nRet))
		{
			printf("VideoCodec_Stream_DeCodec\n");
		}
	}
	VideoCodec_Stream_DeCodec(xhDeVideo, NULL, 0);
	VideoCodec_Stream_Destroy(xhDeVideo);
	VideoCodec_Stream_Destroy(xhEnVideo);
	return 0;
}
int main()
{
	int nListCount = 0;
	AVCODEC_VIDEO_HWCODEC **ppSt_ListHWCodec;
	VideoCodec_Help_GetHWCodec(&ppSt_ListHWCodec, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		printf("%d = %s\n", ppSt_ListHWCodec[i]->enHWDevice, ppSt_ListHWCodec[i]->tszHWName);
	}
	Test_CodecFilter();
	return 0;
}