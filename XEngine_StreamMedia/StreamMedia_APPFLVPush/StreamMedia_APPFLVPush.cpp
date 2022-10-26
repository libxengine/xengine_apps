#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/StreamMedia_XClient.lib")
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

void fread_video(XHANDLE xhToken)
{
	for (int i = 0; i < 20; i++)
	{
		TCHAR tszMsgBuffer[40960];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		int nRet = fread(tszMsgBuffer, 1, sizeof(tszMsgBuffer), pSt_VFile);
		if (nRet <= 0)
		{
			fclose(pSt_VFile);
			pSt_VFile = fopen(lpszVFile, "rb");
			nRet = fread(tszMsgBuffer, 1, sizeof(tszMsgBuffer), pSt_VFile);
		}
		while (1)
		{
			if (XClient_FilePush_Push(xhToken, tszMsgBuffer, nRet, 0))
			{
				break;
			}
		}
	}
}
void fread_audio(XHANDLE xhToken)
{
	while (1)
	{
		TCHAR tszMsgBuffer[4096];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		int nRet = fread(tszMsgBuffer, 1, sizeof(tszMsgBuffer), pSt_AFile);
		if (nRet <= 0)
		{
			fclose(pSt_AFile);
			pSt_AFile = fopen(lpszAFile, "rb");
			nRet = fread(tszMsgBuffer, 1, sizeof(tszMsgBuffer), pSt_AFile);
		}
		while (1)
		{
			if (XClient_FilePush_Push(xhToken, tszMsgBuffer, nRet, 1))
			{
				break;
			}
		}
	}
}

int Test_RTMPPush()
{
	XHANDLE xhStream = NULL;
	LPCTSTR lpszUrl = _T("rtmp://app.xyry.org/live/qyt");
	BOOL bMemory = TRUE;

	if (bMemory)
	{
		pSt_VFile = fopen(lpszVFile, "rb");
		if (NULL == pSt_VFile)
		{
			printf("fopen1:%d\n", errno);
			return -1;
		}
		pSt_AFile = fopen(lpszAFile, "rb");
		if (NULL == pSt_AFile)
		{
			printf("fopen2:%d\n", errno);
			return -1;
		}

		xhStream = XClient_FilePush_Init();
		if (NULL == xhStream)
		{
			printf("XClient_FilePush_Push:%lX\n", StreamClient_GetLastError());
			return -1;
		}
		std::thread m_ThreadVideo(fread_video, xhStream);
		//std::thread m_ThreadAudio(fread_audio, xhStream);

		m_ThreadVideo.detach();
		//m_ThreadAudio.detach();
		Sleep(3000);
		if (!XClient_FilePush_Input(xhStream))
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
	else
	{
		xhStream = XClient_FilePush_Init();
		if (NULL == xhStream)
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
	XHANDLE xhStream = NULL;
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

	st_MediaStream.st_VideoInfo.bEnable = TRUE;
	st_MediaStream.st_VideoInfo.nBitRate = 64000;
	st_MediaStream.st_VideoInfo.nFrameRate = 24;
	st_MediaStream.st_VideoInfo.nWidth = 720;
	st_MediaStream.st_VideoInfo.nHeight = 480;
	st_MediaStream.st_VideoInfo.enAVCodec = ENUM_ENTENGINE_AVCODEC_VEDIO_TYPE_H264;
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

	XNETHANDLE xhAParse = 0;
	XNETHANDLE xhVParse = 0;
	AVHelp_Parse_FrameInit(&xhAParse, ENUM_AVCODEC_AUDIO_TYPE_AAC);
	AVHelp_Parse_FrameInit(&xhVParse, ENUM_ENTENGINE_AVCODEC_VEDIO_TYPE_H264);

	xhStream = XClient_CodecPush_Init(lpszUrl, &st_MediaStream);
	XClient_CodecPush_WriteHdr(xhStream);

	while (TRUE)
	{
		if (st_MediaStream.st_VideoInfo.bEnable)
		{
			int nListCount = 0;
			AVHELP_FRAMEDATA** ppSt_Frame;
			AVHelp_Parse_FrameGet(xhVParse, tszVBuffer, nVLen, &ppSt_Frame, &nListCount);
			for (int i = 0; i < nListCount; i++)
			{
				XClient_CodecPush_PushVideo(xhStream, ppSt_Frame[i]->ptszMsgBuffer, ppSt_Frame[i]->nMsgLen);
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
			int nListCount = 0;
			AVHELP_FRAMEDATA** ppSt_Frame;
			AVHelp_Parse_FrameGet(xhAParse, tszABuffer, nALen, &ppSt_Frame, &nListCount);
			for (int i = 0; i < nListCount; i++)
			{
				//不需要AAC头
				XClient_CodecPush_PushAudio(xhStream, ppSt_Frame[i]->ptszMsgBuffer + 7, ppSt_Frame[i]->nMsgLen - 7);
			}
			memset(tszABuffer, '\0', sizeof(tszABuffer));
			nALen = fread(tszABuffer, 1, sizeof(tszABuffer), pSt_AFile);
			if (nALen <= 0)
			{
				fseek(pSt_AFile, 0, SEEK_SET);
				continue;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(40));
	}
	AVHelp_Parse_FrameClose(xhVParse);
	AVHelp_Parse_FrameClose(xhAParse);
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