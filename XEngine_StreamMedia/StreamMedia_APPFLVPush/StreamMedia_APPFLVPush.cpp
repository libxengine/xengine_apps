#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/StreamMedia_XClient.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVCollect.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVHelp.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>
#include <thread>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AVCollect/AVCollect_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_VideoCoder/VideoCoder_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AudioCoder/AudioCoder_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AVHelp/AVHelp_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AVHelp/AVHelp_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_XClient/XClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_XClient/XClient_Error.h"

//Linux::g++ -std=c++17 -Wall -g StreamMedia_APPFLVPush.cpp -o StreamMedia_APPFLVPush.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCoder -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia -lXEngine_BaseLib -lXEngine_AVHelp -lStreamMedia_XClient -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCoder:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g StreamMedia_APPFLVPush.cpp -o StreamMedia_APPFLVPush.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_AVCoder -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_StreamMedia -lXEngine_BaseLib -lXEngine_AVHelp -lStreamMedia_XClient 

FILE* pSt_VFile;
FILE* pSt_AFile;
#ifdef _MSC_BUILD
LPCTSTR lpszVFile = _T("D:\\h264 file\\480p.264");
LPCTSTR lpszAFile = _T("D:\\h264 file\\test.aac");
#else
LPCTSTR lpszVFile = _T("480p.264");
LPCTSTR lpszAFile = _T("test.aac");
#endif

int fread_video(LPVOID lParam, uint8_t* puszMsgBuffer, int nSize)
{
	int nRet = fread(puszMsgBuffer, 1, nSize, pSt_VFile);
	if (nRet <= 0)
	{
		fclose(pSt_VFile);
		pSt_VFile = fopen(lpszVFile, "rb");
		nRet = fread(puszMsgBuffer, 1, nSize, pSt_VFile);
		return nRet;
	}
	return nRet;
}
int fread_audio(LPVOID lParam, uint8_t* puszMsgBuffer, int nSize)
{
	int nRet = fread(puszMsgBuffer, 1, nSize, pSt_AFile);
	if (nRet <= 0)
	{
		fclose(pSt_AFile);
		pSt_AFile = fopen(lpszAFile, "rb");
		nRet = fread(puszMsgBuffer, 1, nSize, pSt_AFile);
		return nRet;
	}
	return nRet;
}

int Test_RTMPPush()
{
	XNETHANDLE xhStream = 0;
	LPCTSTR lpszUrl = _T("rtmp://app.xyry.org/live/qyt");
	BOOL bMemory = FALSE;

	if (bMemory)
	{
		pSt_VFile = fopen(lpszVFile, "rb");
		if (NULL == pSt_VFile)
		{
			printf("fopen1:%d\n", errno);
			return -1;
		}
		pSt_AFile = fopen(lpszAFile, "rb");
		if (NULL == pSt_VFile)
		{
			printf("fopen2:%d\n", errno);
			return -1;
		}

		if (!XClient_FilePush_Init(&xhStream))
		{
			printf("XClient_FilePush_Push:%lX\n", StreamClient_GetLastError());
			return -1;
		}
		if (!XClient_FilePush_Input(xhStream, NULL, NULL, fread_video, fread_audio, NULL, NULL))
		{
			printf("XClient_FilePush_Input:%lX\n", StreamClient_GetLastError());
			//return -1;
		}
		if (!XClient_FilePush_Output(xhStream, lpszUrl))
		{
			printf("XClient_FilePush_Output:%lX\n", StreamClient_GetLastError());
			return -1;
		}
		if (!XClient_FilePush_Start(xhStream))
		{
			printf("XClient_FilePush_Output:%lX\n", StreamClient_GetLastError());
			return -1;
		}
	}
	else
	{
		if (!XClient_FilePush_Init(&xhStream))
		{
			printf("XClient_FilePush_Push:%lX\n", StreamClient_GetLastError());
			return -1;
		}
		if (!XClient_FilePush_Input(xhStream, lpszVFile, lpszAFile))
		{
			printf("XClient_FilePush_Input:%lX\n", StreamClient_GetLastError());
			return -1;
		}
		if (!XClient_FilePush_Output(xhStream, lpszUrl))
		{
			printf("XClient_FilePush_Output:%lX\n", StreamClient_GetLastError());
			return -1;
		}
		if (!XClient_FilePush_Start(xhStream))
		{
			printf("XClient_FilePush_Output:%lX\n", StreamClient_GetLastError());
			return -1;
		}
	}

	BOOL bIsPush = TRUE;
	while (bIsPush)
	{
		XClient_FilePush_GetStatus(xhStream, &bIsPush);
	}
	XClient_FilePush_Close(xhStream);
	return 1;
}

int Test_LivePush()
{
	XNETHANDLE xhStream = 0;
	FILE* pSt_VFile = NULL;
	FILE* pSt_AFile = NULL;
	LPCTSTR lpszUrl = _T("rtmp://app.xyry.org/live/qyt");

	int nPos = 0;
	int nVLen = 0;
	int nALen = 0;
	TCHAR tszVBuffer[8096];
	TCHAR tszABuffer[8096];
	UCHAR tszSPSBuffer[MAX_PATH];
	UCHAR tszPPSBuffer[MAX_PATH];
	XENGINE_PROTOCOL_AVINFO st_MediaStream;

	memset(tszVBuffer, '\0', sizeof(tszVBuffer));
	memset(tszABuffer, '\0', sizeof(tszABuffer));
	memset(tszSPSBuffer, '\0', MAX_PATH);
	memset(tszPPSBuffer, '\0', MAX_PATH);
	memset(&st_MediaStream, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));

	st_MediaStream.st_VideoInfo.bEnable = FALSE;
	st_MediaStream.st_VideoInfo.nBitRate = 64000;
	st_MediaStream.st_VideoInfo.nFrameRate = 24;
	st_MediaStream.st_VideoInfo.nWidth = 720;
	st_MediaStream.st_VideoInfo.nHeight = 480;
	st_MediaStream.st_VideoInfo.enAVCodec = 27;
	if (st_MediaStream.st_VideoInfo.bEnable)
	{
		pSt_VFile = fopen(lpszVFile, "rb");
		if (NULL == pSt_VFile)
		{
			printf("fopen:%d\n", errno);
			return -1;
		}
		nVLen = fread(tszVBuffer, 1, sizeof(tszVBuffer), pSt_VFile);
		AVHelp_Parse_264Hdr(tszVBuffer, nVLen, tszSPSBuffer, tszPPSBuffer, NULL, NULL, NULL, NULL, NULL, &nPos);

		st_MediaStream.st_VideoInfo.nVLen = nPos;
		memcpy(st_MediaStream.st_VideoInfo.tszVInfo, tszVBuffer, nPos);
	}

	st_MediaStream.st_AudioInfo.bEnable = TRUE;
	st_MediaStream.st_AudioInfo.nChannel = 2;
	st_MediaStream.st_AudioInfo.nBitRate = 64000;
	st_MediaStream.st_AudioInfo.nSampleRate = 44100;
	st_MediaStream.st_AudioInfo.nFrameSize = 1024; //aac frame size
	st_MediaStream.st_AudioInfo.nSampleFmt = ENUM_AVCOLLECT_AUDIO_SAMPLE_FMT_FLTP;
	st_MediaStream.st_AudioInfo.enAVCodec = ENUM_AVCODEC_AUDIO_TYPE_AAC;
	if (st_MediaStream.st_AudioInfo.bEnable)
	{
		pSt_AFile = fopen(lpszAFile, "rb");
		if (NULL == pSt_AFile)
		{
			printf("fopen:%d\n", errno);
			return -1;
		}
		nALen = fread(tszABuffer, 1, sizeof(tszABuffer), pSt_AFile);
		int nProfile = 0;
		int nConfig = 0;
		AVHelp_Parse_AACInfo((const UCHAR*)tszABuffer, nALen, &st_MediaStream.st_AudioInfo.nChannel, &st_MediaStream.st_AudioInfo.nSampleRate, &nProfile, &nConfig);
	}

	XClient_CodecPush_Init(&xhStream, lpszUrl, &st_MediaStream, "flv", TRUE, TRUE);
	XClient_CodecPush_WriteHdr(xhStream);

	while (TRUE)
	{
		if (st_MediaStream.st_VideoInfo.bEnable)
		{
			while (TRUE)
			{
				if (XClient_CodecPush_PushVideo(xhStream, tszVBuffer, nVLen))
				{
					break;
				}
			}
			memset(tszVBuffer, '\0', sizeof(tszVBuffer));
			nVLen = fread(tszVBuffer, 1, sizeof(tszVBuffer), pSt_VFile);
			if (nVLen <= 0)
			{
				fseek(pSt_VFile, 0, SEEK_SET);
				continue;
			}
		}
		if (st_MediaStream.st_AudioInfo.bEnable)
		{
			while (TRUE)
			{
				if (XClient_CodecPush_PushAudio(xhStream, tszABuffer, nALen))
				{
					break;
				}
			}
			memset(tszABuffer, '\0', sizeof(tszABuffer));
			nALen = fread(tszABuffer, 1, sizeof(tszABuffer), pSt_AFile);
			if (nALen <= 0)
			{
				fseek(pSt_AFile, 0, SEEK_SET);
				continue;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
	}
	XClient_CodecPush_Close(xhStream);
	return 0;
}

int main()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	Test_LivePush();
	//Test_RTMPPush();
	
#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}