#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/StreamMedia_XClient.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AVCollect/AVCollect_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCoder/XEngine_AudioCoder/AudioCoder_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_XClient/XClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_XClient/XClient_Error.h"

//g++ -std=c++17 -Wall -g StreamMedia_APPFLVPush.cpp -o StreamMedia_APPFLVPush.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia -lXEngine_BaseLib -lStreamMedia_XClient -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia,--disable-new-dtags

FILE* pSt_VFile;
FILE* pSt_AFile;
LPCTSTR lpszVFile = _T("H:\\h264 file\\480p.264");
LPCTSTR lpszAFile = _T("H:\\h264 file\\test.aac");

int fread_video(LPVOID lParam, uint8_t* puszMsgBuffer, int nSize)
{
	//printf("%d\n", nSize);
	int nRet = fread(puszMsgBuffer, 1, 1024, pSt_VFile);
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
	printf("%d\n", nSize);
	int nRet = fread(puszMsgBuffer, 1, 1024, pSt_AFile);
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
	LPCTSTR lpszUrl = _T("rtmp://stream.xyry.org/flv/qyt");
	BOOL bMemory = TRUE;

	if (bMemory)
	{
		pSt_VFile = fopen(lpszVFile, "rb");
		if (NULL == pSt_VFile)
		{
			printf("fopen:%d\n", errno);
			return -1;
		}
		pSt_AFile = fopen(lpszAFile, "rb");
		if (NULL == pSt_VFile)
		{
			printf("fopen:%d\n", errno);
			return -1;
		}

		if (!XClient_FilePush_Init(&xhStream))
		{
			printf("XClient_FilePush_Push:%lX\n", XClient_GetLastError());
			return -1;
		}
		if (!XClient_FilePush_Input(xhStream, NULL, NULL, fread_video, fread_audio, NULL, NULL))
		{
			printf("XClient_FilePush_Input:%lX\n", XClient_GetLastError());
			//return -1;
		}
		if (!XClient_FilePush_Output(xhStream, lpszUrl))
		{
			printf("XClient_FilePush_Output:%lX\n", XClient_GetLastError());
			return -1;
		}
		if (!XClient_FilePush_Start(xhStream))
		{
			printf("XClient_FilePush_Output:%lX\n", XClient_GetLastError());
			return -1;
		}
	}
	else
	{
		if (!XClient_FilePush_Init(&xhStream))
		{
			printf("XClient_FilePush_Push:%lX\n", XClient_GetLastError());
			return -1;
		}
		if (!XClient_FilePush_Input(xhStream, lpszVFile, lpszAFile))
		{
			printf("XClient_FilePush_Input:%lX\n", XClient_GetLastError());
			return -1;
		}
		if (!XClient_FilePush_Output(xhStream, lpszUrl))
		{
			printf("XClient_FilePush_Output:%lX\n", XClient_GetLastError());
			return -1;
		}
		if (!XClient_FilePush_Start(xhStream))
		{
			printf("XClient_FilePush_Output:%lX\n", XClient_GetLastError());
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

int CBStream_Pull(LPVOID lParam, uint8_t* puszMsgBuffer, int nSize)
{
	FILE* pSt_File = (FILE*)lParam;
	printf("%d\n", nSize);
	int nRet = fwrite(puszMsgBuffer, 1, nSize, pSt_File);

	return nRet;
}
int Test_RTMPPull()
{
	XNETHANDLE xhStream = 0;
	LPCTSTR lpszUrl = _T("http://stream.xyry.org:8800/flv?app=flv&stream=qyt");
	LPCTSTR lpszFile = _T("stream.flv");

	FILE* pSt_File = fopen(lpszFile, _T("wb"));
	if (NULL == pSt_File)
	{
		printf("file:%d\n", errno);
		return -1;
	}

	if (!XClient_StreamPull_Init(&xhStream, lpszUrl, CBStream_Pull, pSt_File))
	{
		printf("XClient_FilePush_Push:%lX\n", XClient_GetLastError());
		return -1;
	}
	BOOL bPull = FALSE;
	XClient_StreamPull_GetStatus(xhStream, &bPull);

	while (bPull)
	{
	}
	XClient_StreamPull_Close(xhStream);

	return 1;
}
int Test_LivePush()
{
	XNETHANDLE xhStream = 0;
	LPCTSTR lpszUrl = _T("rtmp://stream.xyry.org:1935/live/qyt");
	LPCTSTR lpszVFile = _T("./480p.264");
	LPCTSTR lpszAFile = _T("./test.aac");

	XENGINE_AVPROTOCOL st_MediaStream;
	memset(&st_MediaStream, '\0', sizeof(XENGINE_AVPROTOCOL));

	st_MediaStream.st_PushVideo.bEnable = TRUE;
	st_MediaStream.st_PushVideo.nBitRate = 64000;
	st_MediaStream.st_PushVideo.nFrameRate = 24;
	st_MediaStream.st_PushVideo.nWidth = 720;
	st_MediaStream.st_PushVideo.nHeight = 480;
	st_MediaStream.st_PushVideo.enAvCodec = 27;

	st_MediaStream.st_PushAudio.bEnable = FALSE;
	st_MediaStream.st_PushAudio.nChannel = 2;
	st_MediaStream.st_PushAudio.nBitRate = 64000;
	st_MediaStream.st_PushAudio.nSampleRate = 44100;
	st_MediaStream.st_PushAudio.nSampleFmt = ENUM_AVCOLLECT_AUDIO_SAMPLE_FMT_FLTP;
	st_MediaStream.st_PushAudio.enAvCodec = ENUM_AVCODEC_AUDIO_TYPE_AAC;
	XClient_CodecPush_Init(&xhStream, lpszUrl, &st_MediaStream, "flv", TRUE);
	//XClient_CodecPush_WriteHdr(xhStream);// 如果你有视频并且写入了SPSPPS信息到初始化,那么可以优先调用,否则必须写入后才可调用
	FILE* pSt_VFile = fopen(lpszVFile, "rb");
	if (NULL == pSt_VFile)
	{
		printf("fopen:%d\n", errno);
		return -1;
	}
	FILE* pSt_AFile = fopen(lpszAFile, "rb");
	if (NULL == pSt_AFile)
	{
		printf("fopen:%d\n", errno);
		return -1;
	}

	BOOL bInit = FALSE;
	while (TRUE)
	{
		TCHAR tszVBuffer[8096];
		memset(tszVBuffer, '\0', sizeof(tszVBuffer));
		int nLen = fread(tszVBuffer, 1, sizeof(tszVBuffer), pSt_VFile);
		if (nLen <= 0)
		{
			fseek(pSt_VFile, 0, SEEK_SET);
			continue;
		}
		while (1)
		{
			if (XClient_CodecPush_PushVideo(xhStream, tszVBuffer, nLen))
			{
				break;
			}
		}

		if (!bInit)
		{
			XClient_CodecPush_WriteHdr(xhStream);
			bInit = TRUE;
		}
		/*
		TCHAR tszABuffer[2048];
		memset(tszABuffer, '\0', sizeof(tszABuffer));

		int nRet = fread(tszABuffer, 1, sizeof(tszABuffer), pSt_AFile);
		if (nRet <= 0)
		{
			fseek(pSt_AFile, 0, SEEK_SET);
			continue;
		}
		XClient_CodecPush_PushAudio(xhStream, tszABuffer, nRet);
		*/
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

	Test_RTMPPush();
	//Test_RTMPPull();
	
#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}