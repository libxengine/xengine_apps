#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <winsock2.h>
#include <Windows.h>
#include <tchar.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
using namespace std;
#if 1 == _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Define.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Error.h>
#include <XEngine_Include/XEngine_StreamMedia/FLVProtocol_Define.h>
#include <XEngine_Include/XEngine_StreamMedia/FLVProtocol_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AVCollect_Define.h>
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AudioCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_StreamMedia/StreamMedia_FLVProtocol.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVHelp.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_ManagePool/ManagePool_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_ManagePool/ManagePool_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_FLVProtocol/FLVProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_FLVProtocol/FLVProtocol_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVCollect/AVCollect_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_VideoCodec/VideoCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AudioCodec/AudioCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/StreamMedia_FLVProtocol.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVHelp.lib")
#endif
#endif

//Linux::g++ -std=c++17 -Wall -g StreamMedia_APPFlv.cpp -o StreamMedia_APPFlv.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCodec -lXEngine_BaseLib -lStreamMedia_FLVProtocol -lXEngine_AVHelp -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCodec,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g StreamMedia_APPFlv.cpp -o StreamMedia_APPFlv.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_StreamMedia -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_AVCodec -lXEngine_BaseLib -lStreamMedia_FLVProtocol -lXEngine_AVHelp

bool FLV_Parse()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszFile = _X("D:\\XEngine_StreamMedia\\XEngine_APPClient\\Debug\\1.flv");
#else
	LPCXSTR lpszFile = _X("480p.flv");
#endif

	FILE* pSt_File = fopen(lpszFile, _X("rb"));
	if (NULL == pSt_File)
	{
		return false;
	}
	LPCXSTR lpszClientID = _X("client");
	XNETHANDLE xhToken = 0;
	FLVProtocol_Parse_Insert(lpszClientID);

	while (1)
	{
		XCHAR tszMsgBuffer[2048];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		int nRet = fread(tszMsgBuffer, 1, sizeof(tszMsgBuffer), pSt_File);
		if (nRet <= 0)
		{
			break;
		}
		FLVProtocol_Parse_Send(lpszClientID, tszMsgBuffer, nRet);

		int nMsgLen = 0;
		int nAVType = 0;
		XCHAR* ptszMsgBuffer = NULL;
		XENGINE_FLVVIDEO st_FLVVideo;
		XENGINE_FLVAUDIO st_FLVAudio;

		memset(&st_FLVVideo, '\0', sizeof(XENGINE_FLVVIDEO));
		memset(&st_FLVAudio, '\0', sizeof(XENGINE_FLVAUDIO));

		if (FLVProtocol_Parse_Recv(lpszClientID, &ptszMsgBuffer, &nMsgLen, &nAVType, &st_FLVVideo, &st_FLVAudio))
		{
			if (0 == nAVType && 0 != st_FLVVideo.byFrameType)
			{
			}
		}
	}

	int nPPSLen = 0;
	int nSPSLen = 0;
	XCHAR tszPPSBuffer[1024];
	XCHAR tszSPSBuffer[1024];

	memset(tszPPSBuffer, '\0', sizeof(tszPPSBuffer));
	memset(tszSPSBuffer, '\0', sizeof(tszSPSBuffer));

	if (FLVProtocol_Parse_GetMetaInfo(lpszClientID, tszSPSBuffer, tszPPSBuffer, NULL, &nSPSLen, &nPPSLen, NULL))
	{
		printf("%d %d\n", nSPSLen, nPPSLen);
	}

	int nListCount = 0;;
	XENGINE_FLVAVINFO** ppSt_FLVInfoList;
	if (FLVProtocol_Parse_GetScriptInfo(lpszClientID, &ppSt_FLVInfoList, &nListCount))
	{
		for (int i = 0; i < nListCount; i++)
		{
			printf("%s %s\n", ppSt_FLVInfoList[i]->tszKeyStr, ppSt_FLVInfoList[i]->tszVluStr);
		}
	}
	FLVProtocol_Parse_Delete(lpszClientID);
	return true;
}

bool FLV_PacketVideo()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszVFile = _X("D:\\h264 file\\480p.264");
	LPCXSTR lpszFLVFile = _X("D:\\h264 file\\480.flv");
#else
	LPCXSTR lpszVFile = _X("480p.flv");
	LPCXSTR lpszFLVFile = _X("480.flv");
#endif
	XNETHANDLE xhVideo = 0;
	LPCXSTR lpszClientID = _X("client");

	FLVProtocol_Packet_Insert(lpszClientID, false, true);
	AVHelp_Parse_FrameInit(&xhVideo, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264);

	FILE* pSt_FLVFile = fopen(lpszFLVFile, _X("wb"));
	if (NULL == pSt_FLVFile)
	{
		return false;
	}
	FILE* pSt_VFile = fopen(lpszVFile, _X("rb"));
	if (NULL == pSt_VFile)
	{
		return false;
	}
	int nRBLen = 0;
	int nWBLen = 0;
	XCHAR tszRBBuffer[2048];
	XCHAR tszWBBuffer[2048];

	memset(tszRBBuffer, '\0', sizeof(tszRBBuffer));
	memset(tszWBBuffer, '\0', sizeof(tszWBBuffer));
	FLVProtocol_Packet_FrameHdr(lpszClientID, tszWBBuffer, &nWBLen);

	fwrite(tszWBBuffer, 1, nWBLen, pSt_FLVFile);
	//音视频信息配置
	XENGINE_PROTOCOL_AVINFO st_AVInfo;
	memset(&st_AVInfo, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));

	st_AVInfo.nSize = 821591;
	st_AVInfo.dlTime = 99.84600;
	strcpy(st_AVInfo.tszPktName, "Lavf59.27.100");

	st_AVInfo.st_VideoInfo.bEnable = true;
	st_AVInfo.st_VideoInfo.nWidth = 720;
	st_AVInfo.st_VideoInfo.nHeight = 480;
	st_AVInfo.st_VideoInfo.enAVCodec = 7;

	memset(tszWBBuffer, '\0', sizeof(tszWBBuffer));
	FLVProtocol_Packet_FrameScript(lpszClientID, tszWBBuffer, &nWBLen, &st_AVInfo);
	fwrite(tszWBBuffer, 1, nWBLen, pSt_FLVFile);

	st_AVInfo.st_VideoInfo.nVLen = fread(st_AVInfo.st_VideoInfo.tszVInfo, 1, sizeof(st_AVInfo.st_VideoInfo.tszVInfo), pSt_VFile);
	FLVProtocol_Packet_FrameAVCConfigure(lpszClientID, tszWBBuffer, &nWBLen, &st_AVInfo);
	fwrite(tszWBBuffer, 1, nWBLen, pSt_FLVFile);

	fseek(pSt_VFile, 0, SEEK_SET);
	int nTimeStamp = 0;
	while (true)
	{
		memset(tszRBBuffer, '\0', sizeof(tszRBBuffer));
		nRBLen = fread(tszRBBuffer, 1, sizeof(tszRBBuffer), pSt_VFile);
		if (nRBLen <= 0)
		{
			break;
		}
		int nListCount = 0;
		AVHELP_FRAMEDATA** ppSt_Frame;
		AVHelp_Parse_FrameGet(xhVideo, tszRBBuffer, nRBLen, &ppSt_Frame, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			XCHAR tszMsgBuffer[102400];
			FLVProtocol_Packet_FrameVideo(lpszClientID, tszMsgBuffer, &nWBLen, (LPCXSTR)ppSt_Frame[i]->ptszMsgBuffer, ppSt_Frame[i]->nMsgLen, nTimeStamp);
			fwrite(tszMsgBuffer, 1, nWBLen, pSt_FLVFile);
			nTimeStamp += 42; //每秒24帧
		}
	}
	FLVProtocol_Packet_FrameVideo(lpszClientID, tszWBBuffer, &nWBLen, NULL, 0, nTimeStamp);
	fwrite(tszWBBuffer, 1, nWBLen, pSt_FLVFile);

	fclose(pSt_VFile);
	fclose(pSt_FLVFile);
	AVHelp_Parse_FrameClose(xhVideo);
	FLVProtocol_Packet_Delete(lpszClientID);
	return true;
}
bool FLV_PacketAudio()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszAFile = _X("D:\\h264 file\\1.aac");
	LPCXSTR lpszFLVFile = _X("D:\\h264 file\\480.flv");
#else
	LPCXSTR lpszAFile = _X("1.aac");
	LPCXSTR lpszFLVFile = _X("480.flv");
#endif
	XNETHANDLE xhAudio = 0;
	LPCXSTR lpszClientID = _X("client");

	FLVProtocol_Packet_Insert(lpszClientID, false, true);
	AVHelp_Parse_FrameInit(&xhAudio, ENUM_XENGINE_AVCODEC_AUDIO_TYPE_AAC);

	FILE* pSt_FLVFile = fopen(lpszFLVFile, _X("wb"));
	if (NULL == pSt_FLVFile)
	{
		return false;
	}
	FILE* pSt_AFile = fopen(lpszAFile, _X("rb"));
	if (NULL == pSt_AFile)
	{
		return false;
	}

	int nRBLen = 0;
	int nWBLen = 0;
	XCHAR tszRBBuffer[2048];
	XCHAR tszWBBuffer[2048];

	memset(tszRBBuffer, '\0', sizeof(tszRBBuffer));
	memset(tszWBBuffer, '\0', sizeof(tszWBBuffer));
	FLVProtocol_Packet_FrameHdr(lpszClientID, tszWBBuffer, &nWBLen);

	fwrite(tszWBBuffer, 1, nWBLen, pSt_FLVFile);
	//音视频信息配置
	XENGINE_PROTOCOL_AVINFO st_AVInfo;
	memset(&st_AVInfo, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));

	st_AVInfo.nSize = 821591;
	st_AVInfo.dlTime = 99.84600;
	strcpy(st_AVInfo.tszPktName, "Lavf59.27.100");

	st_AVInfo.st_AudioInfo.bEnable = true;
	st_AVInfo.st_AudioInfo.nBitRate = 62;
	st_AVInfo.st_AudioInfo.nSampleRate = 11025;
	st_AVInfo.st_AudioInfo.nSampleFmt = 16;
	st_AVInfo.st_AudioInfo.nChannel = 1;
	st_AVInfo.st_AudioInfo.enAVCodec = 10;

	memset(tszWBBuffer, '\0', sizeof(tszWBBuffer));
	FLVProtocol_Packet_FrameScript(lpszClientID, tszWBBuffer, &nWBLen, &st_AVInfo);
	fwrite(tszWBBuffer, 1, nWBLen, pSt_FLVFile);

	st_AVInfo.st_AudioInfo.enAVCodec = 10;
	st_AVInfo.st_AudioInfo.nSampleRate = 3;
	st_AVInfo.st_AudioInfo.nSampleFmt = 1;
	st_AVInfo.st_AudioInfo.nChannel = 1;

	memset(tszWBBuffer, '\0', sizeof(tszWBBuffer));
	st_AVInfo.st_AudioInfo.nALen = fread(st_AVInfo.st_AudioInfo.tszAInfo, 1, sizeof(st_AVInfo.st_AudioInfo.tszAInfo), pSt_AFile);
	FLVProtocol_Packet_FrameAACConfigure(lpszClientID, tszWBBuffer, &nWBLen, &st_AVInfo);
	fwrite(tszWBBuffer, 1, nWBLen, pSt_FLVFile);

	fseek(pSt_AFile, 0, SEEK_SET);
	int nTimeStamp = 0;

	while (true)
	{
		memset(tszRBBuffer, '\0', sizeof(tszRBBuffer));
		nRBLen = fread(tszRBBuffer, 1, sizeof(tszRBBuffer), pSt_AFile);
		if (nRBLen <= 0)
		{
			break;
		}
		int nListCount = 0;
		AVHELP_FRAMEDATA** ppSt_Frame;
		AVHelp_Parse_FrameGet(xhAudio, tszRBBuffer, nRBLen, &ppSt_Frame, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			XCHAR tszMsgBuffer[102400];
			FLVProtocol_Packet_FrameAudio(lpszClientID, tszMsgBuffer, &nWBLen, (LPCXSTR)ppSt_Frame[i]->ptszMsgBuffer, ppSt_Frame[i]->nMsgLen, NULL, nTimeStamp);
			fwrite(tszMsgBuffer, 1, nWBLen, pSt_FLVFile);
			nTimeStamp += 93;
		}
	}
	fclose(pSt_AFile);
	fclose(pSt_FLVFile);

	AVHelp_Parse_FrameClose(xhAudio);
	FLVProtocol_Packet_Delete(lpszClientID);
	return true;
}

int main()
{
	//FLV_Parse();
	//FLV_PacketVideo();
	FLV_PacketAudio();
	return 0;
}