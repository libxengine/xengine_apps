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
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIHelp_Error.h>
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AudioCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AVFrame_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVFrame_Error.h>
#include <XEngine_Include/XEngine_StreamMedia/MP4Protocol_Define.h>
#include <XEngine_Include/XEngine_StreamMedia/MP4Protocol_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_NetHelp/NetHelp_APIHelp.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVHelp.lib")
#pragma comment(lib,"XEngine_StreamMedia/StreamMedia_MP4Protocol.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVFrame.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_ManagePool/ManagePool_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_APIHelp/APIHelp_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_APIHelp/APIHelp_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_VideoCodec/VideoCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AudioCodec/AudioCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFrame/AVFrame_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFrame/AVFrame_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_MP4Protocol/MP4Protocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_MP4Protocol/MP4Protocol_Error.h"
#ifdef _MSC_BUILD
#ifdef _WIN64
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/NetHelp_APIHelp.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_AVHelp.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_AVFrame.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/StreamMedia_MP4Protocol.lib")
#else
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/NetHelp_APIHelp.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVHelp.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVFrame.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/StreamMedia_MP4Protocol.lib")
#endif
#endif
#endif

//Linux::g++ -std=c++20 -Wall -g StreamMedia_MP4App.cpp -o StreamMedia_MP4App.exe -lXEngine_BaseLib -lNetHelp_APIHelp -lStreamMedia_MP4Protocol -lXEngine_AVHelp -lXEngine_AVFrame

int MP4_Parse()
{
	LPCXSTR lpszClientID = _X("127777");
#ifdef _MSC_BUILD
	FILE* pSt_File = _xtfopen(_X("D:\\h264 file\\480p.mp4"), _X("rb"));
#else
	FILE* pSt_File = _xtfopen(_X("480p.mp4"), _X("rb"));
#endif
	if (NULL == pSt_File)
	{
		return -1;
	}

	if (!MP4Protocol_Parse_Init(1))
	{
		printf("MP4Protocol_Parse_Init:%lX\n", MP4Protocol_GetLastError());
	}
	if (!MP4Protocol_Parse_Insert(lpszClientID))
	{
		printf("MP4Protocol_Parse_Insert:%lX\n", MP4Protocol_GetLastError());
	}

	__int64u nFCOunt = 0;
	while (true)
	{
		XCHAR tszMSGBuffer[1024];
		fseek(pSt_File, nFCOunt, SEEK_SET);
		int nRet = fread(tszMSGBuffer, 1, sizeof(tszMSGBuffer), pSt_File);
		if (nRet <= 0)
		{
			break;
		}
		MP4Protocol_Parse_Send(lpszClientID, tszMSGBuffer, nRet);

		bool bBreak = false;
		__int64u nFPos = 0;
		XENGINE_MP4HDR st_MP4Hdr = {};
		while (true)
		{
			if (!MP4Protocol_Parse_Recv(lpszClientID, &st_MP4Hdr, &nFPos))
			{
				break;
			}
			if (0 == st_MP4Hdr.nHDRSize)
			{
				bBreak = true;
				break;
			}
		}
		if (bBreak)
		{
			break;
		}
		if (st_MP4Hdr.enHType == ENUM_XENGINE_STREAMMEDIA_MP4PROTCOL_HTYPE_MDAT)
		{
			nFCOunt = nFPos;
		}
		else
		{
			nFCOunt += nRet;
		}
	}

	MP4Protocol_Parse_Delete(lpszClientID);
	MP4Protocol_Parse_Destory();
	return 0;
}

void MP4_PacketMoov(LPCXSTR lpszClientID, FILE* pSt_File, int nPos)
{
	int nSize = 0;
	int nMOOVPos = 0;
	int nTRAKPos = 0;
	int nMDIAPos = 0;
	int nMINFPos = 0;
	int nSTBLPos = 0;
	XCHAR tszMSGBuffer[10240] = {};
	
	MP4Protocol_Packet_HDRBox(lpszClientID, tszMSGBuffer, &nSize, _X("trak"));
	fwrite(tszMSGBuffer, 1, nSize, pSt_File);
	nTRAKPos = nPos;
	nPos += nSize;

	memset(tszMSGBuffer, '\0', sizeof(tszMSGBuffer));
	MP4Protocol_Packet_TKhd(lpszClientID, tszMSGBuffer, &nSize, 0, 720, 480, 90090);
	fwrite(tszMSGBuffer, 1, nSize, pSt_File);
	nPos += nSize;

	memset(tszMSGBuffer, '\0', sizeof(tszMSGBuffer));
	MP4Protocol_Packet_HDRBox(lpszClientID, tszMSGBuffer, &nSize, _X("mdia"));
	fwrite(tszMSGBuffer, 1, nSize, pSt_File);
	nMDIAPos = nPos;
	nPos += nSize;

	memset(tszMSGBuffer, '\0', sizeof(tszMSGBuffer));
	MP4Protocol_Packet_MDhd(lpszClientID, tszMSGBuffer, &nSize);
	fwrite(tszMSGBuffer, 1, nSize, pSt_File);
	nPos += nSize;

	memset(tszMSGBuffer, '\0', sizeof(tszMSGBuffer));
	MP4Protocol_Packet_HDlr(lpszClientID, tszMSGBuffer, &nSize);
	fwrite(tszMSGBuffer, 1, nSize, pSt_File);
	nPos += nSize;

	memset(tszMSGBuffer, '\0', sizeof(tszMSGBuffer));
	MP4Protocol_Packet_HDRBox(lpszClientID, tszMSGBuffer, &nSize, _X("minf"));
	fwrite(tszMSGBuffer, 1, nSize, pSt_File);
	nMINFPos = nPos;
	nPos += nSize;

	memset(tszMSGBuffer, '\0', sizeof(tszMSGBuffer));
	MP4Protocol_Packet_VMhd(lpszClientID, tszMSGBuffer, &nSize);
	fwrite(tszMSGBuffer, 1, nSize, pSt_File);
	nPos += nSize;

	memset(tszMSGBuffer, '\0', sizeof(tszMSGBuffer));
	MP4Protocol_Packet_DInf(lpszClientID, tszMSGBuffer, &nSize);
	fwrite(tszMSGBuffer, 1, nSize, pSt_File);
	nPos += nSize;

	memset(tszMSGBuffer, '\0', sizeof(tszMSGBuffer));
	MP4Protocol_Packet_HDRBox(lpszClientID, tszMSGBuffer, &nSize, _X("stbl"));
	fwrite(tszMSGBuffer, 1, nSize, pSt_File);
	nSTBLPos = nPos;
	nPos += nSize;
	//读取H264的SPS,PPS
	XENGINE_PROTOCOL_AVINFO st_AVInfo = {};
#ifdef _MSC_BUILD
	FILE* pSt_HFile = _xtfopen("D:\\h264 file\\480p.264", "rb");
#else
	FILE* pSt_HFile = _xtfopen("480p.264", "rb");
#endif
	st_AVInfo.st_VideoInfo.nVLen = fread(st_AVInfo.st_VideoInfo.tszVInfo, 1, 39, pSt_HFile);
	fclose(pSt_HFile);

	memset(tszMSGBuffer, '\0', sizeof(tszMSGBuffer));
	st_AVInfo.st_AudioInfo.bEnable = false;
	st_AVInfo.st_VideoInfo.bEnable = true;

	/*
	st_AVInfo.st_AudioInfo.bEnable = true;
	st_AVInfo.st_VideoInfo.bEnable = false;
	st_AVInfo.st_AudioInfo.nChannel = 2;
	st_AVInfo.st_AudioInfo.nSampleFmt = 16;
	st_AVInfo.st_AudioInfo.nSampleRate = 48000;
	st_AVInfo.st_AudioInfo.nALen = 5;
	st_AVInfo.st_AudioInfo.tszAInfo[0] = 0x01;
	st_AVInfo.st_AudioInfo.tszAInfo[1] = 0x02;
	st_AVInfo.st_AudioInfo.tszAInfo[2] = 0x03;
	st_AVInfo.st_AudioInfo.tszAInfo[3] = 0x04;
	st_AVInfo.st_AudioInfo.tszAInfo[4] = 0x05;
	*/

	MP4Protocol_Packet_STsd(lpszClientID, tszMSGBuffer, &nSize, &st_AVInfo);
	fwrite(tszMSGBuffer, 1, nSize, pSt_File);
	nPos += nSize;

	memset(tszMSGBuffer, '\0', sizeof(tszMSGBuffer));
	MP4Protocol_Packet_STts(lpszClientID, tszMSGBuffer, &nSize);
	fwrite(tszMSGBuffer, 1, nSize, pSt_File);
	nPos += nSize;

	memset(tszMSGBuffer, '\0', sizeof(tszMSGBuffer));
	MP4Protocol_Packet_STss(lpszClientID, tszMSGBuffer, &nSize);
	fwrite(tszMSGBuffer, 1, nSize, pSt_File);
	nPos += nSize;

	memset(tszMSGBuffer, '\0', sizeof(tszMSGBuffer));
	MP4Protocol_Packet_STsc(lpszClientID, tszMSGBuffer, &nSize);
	fwrite(tszMSGBuffer, 1, nSize, pSt_File);
	nPos += nSize;

	memset(tszMSGBuffer, '\0', sizeof(tszMSGBuffer));
	MP4Protocol_Packet_STsz(lpszClientID, tszMSGBuffer, &nSize);
	fwrite(tszMSGBuffer, 1, nSize, pSt_File);
	nPos += nSize;

	memset(tszMSGBuffer, '\0', sizeof(tszMSGBuffer));
	MP4Protocol_Packet_STco(lpszClientID, tszMSGBuffer, &nSize);
	fwrite(tszMSGBuffer, 1, nSize, pSt_File);
	nPos += nSize;

	fseek(pSt_File, nSTBLPos, SEEK_SET);
	nSize = XHtonl(nPos - nSTBLPos);
	fwrite(&nSize, 1, 4, pSt_File);

	fseek(pSt_File, nMINFPos, SEEK_SET);
	nSize = XHtonl(nPos - nMINFPos);
	fwrite(&nSize, 1, 4, pSt_File);

	fseek(pSt_File, nMDIAPos, SEEK_SET);
	nSize = XHtonl(nPos - nMDIAPos);
	fwrite(&nSize, 1, 4, pSt_File);

	fseek(pSt_File, nTRAKPos, SEEK_SET);
	nSize = XHtonl(nPos - nTRAKPos);
	fwrite(&nSize, 1, 4, pSt_File);

	fseek(pSt_File, nMOOVPos, SEEK_SET);
	nSize = XHtonl(nPos - nMOOVPos);
	fwrite(&nSize, 1, 4, pSt_File);
}
int MP4_Packet()
{
	XNETHANDLE xhVideo = 0;
	XNETHANDLE xhAudio = 0;
	LPCXSTR lpszClientID = _X("127777");

#ifdef _MSC_BUILD
	FILE* pSt_RFile = _xtfopen(_X("D:\\h264 file\\480p.264"), _X("rb"));
	FILE* pSt_WFile = _xtfopen(_X("D:\\h264 file\\480p.mp4"), _X("wb"));
#else
	FILE* pSt_RFile = _xtfopen(_X("480p.264"), _X("rb"));
	FILE* pSt_WFile = _xtfopen(_X("480p.mp4"), _X("wb"));
#endif
	if (NULL == pSt_RFile)
	{
		return -1;
	}
	if (NULL == pSt_WFile)
	{
		return -1;
	}
	
	MP4Protocol_Packet_Insert(lpszClientID);
	MP4Protocol_Packet_SetTime(lpszClientID);
	AVFrame_Frame_ParseInit(&xhVideo, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264);
	AVFrame_Frame_ParseInit(&xhAudio, ENUM_XENGINE_AVCODEC_AUDIO_TYPE_AAC);
	
	int nRVLen = 0;
	int nSDLen = 0;
	__int64x nMDatPos = 0;
	__int64x nFilePos = 0;
	XCHAR tszRVBuffer[2048] = {};
	XCHAR *ptszSDBuffer = (XCHAR *)malloc(XENGINE_MEMORY_SIZE_MAX);
	MP4Protocol_Packet_FTyp(lpszClientID, ptszSDBuffer, &nSDLen);
	fwrite(ptszSDBuffer, 1, nSDLen, pSt_WFile);
	nFilePos += nSDLen;

	MP4Protocol_Packet_HDRBox(lpszClientID, ptszSDBuffer, &nSDLen, "free");
	fwrite(ptszSDBuffer, 1, nSDLen, pSt_WFile);
	nFilePos += nSDLen;
	nMDatPos = nFilePos;

	MP4Protocol_Packet_HDRBox(lpszClientID, ptszSDBuffer, &nSDLen, "mdat");
	fwrite(ptszSDBuffer, 1, nSDLen, pSt_WFile);
	nFilePos += nSDLen;

	MP4Protocol_Packet_HDRBox(lpszClientID, ptszSDBuffer, &nSDLen, _X("moov"));
	fwrite(ptszSDBuffer, 1, nSDLen, pSt_WFile);
	nFilePos = nSDLen;

	MP4Protocol_Packet_MVhd(lpszClientID, ptszSDBuffer, &nSDLen, 90090);
	fwrite(ptszSDBuffer, 1, nSDLen, pSt_WFile);
	nFilePos += nSDLen;

	while (true)
	{
		nRVLen = 0;
		nSDLen = 0;
		memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));
		memset(ptszSDBuffer, '\0', XENGINE_MEMORY_SIZE_MAX);
		nRVLen = fread(tszRVBuffer, 1, sizeof(tszRVBuffer), pSt_RFile);
		if (nRVLen <= 0)
		{
			break;
		}
		int nListCount = 0;
		XENGINE_MSGBUFFER** ppSt_Frame;
		AVFrame_Frame_ParseGet(xhVideo, tszRVBuffer, nRVLen, &ppSt_Frame, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			XENGINE_AVCODEC_VIDEOFRAMETYPE enVideoFrameType;
			AVHelp_Parse_NaluType((LPCXSTR)ppSt_Frame[i]->unData.ptszMSGBuffer, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264, &enVideoFrameType);

			int nFrameType = 0;
			if ((ENUM_XENGINE_AVCODEC_VIDEO_FRAMETYPE_SPS == enVideoFrameType) || (ENUM_XENGINE_AVCODEC_VIDEO_FRAMETYPE_PPS == enVideoFrameType) || (ENUM_XENGINE_AVCODEC_VIDEO_FRAMETYPE_I == enVideoFrameType))
			{
				nFrameType = 1;
			}
			MP4Protocol_Packet_FrameVideo(lpszClientID, ptszSDBuffer, &nSDLen, (LPCXSTR)ppSt_Frame[i]->unData.ptszMSGBuffer, ppSt_Frame[i]->nMSGLen[0], nFilePos, nFrameType);
			fwrite(ptszSDBuffer, 1, nSDLen, pSt_WFile);
			nFilePos += nSDLen;
			BaseLib_Memory_FreeCStyle((XPPMEM)&ppSt_Frame[i]->unData.ptszMSGBuffer);
		}
		BaseLib_Memory_Free((XPPPMEM)&ppSt_Frame, nListCount);
	}
	MP4Protocol_Packet_HDRBox(lpszClientID, ptszSDBuffer, &nSDLen, "mdat", NULL, nFilePos - nMDatPos - 8);
	fseek(pSt_WFile, nMDatPos, SEEK_SET);
	fwrite(ptszSDBuffer, 1, nSDLen, pSt_WFile);
	fseek(pSt_WFile, nFilePos, SEEK_SET);
	MP4_PacketMoov(lpszClientID, pSt_WFile, nFilePos);
	//////////////////////////////////////////////////////////////////////////
	while (true)
	{
		nRVLen = 0;
		nSDLen = 0;
		memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));
		memset(ptszSDBuffer, '\0', XENGINE_MEMORY_SIZE_MAX);
		nRVLen = fread(tszRVBuffer, 1, sizeof(tszRVBuffer), pSt_RFile);
		if (nRVLen <= 0)
		{
			break;
		}
		int nListCount = 0;
		XENGINE_MSGBUFFER** ppSt_Frame;
		AVFrame_Frame_ParseGet(xhVideo, tszRVBuffer, nRVLen, &ppSt_Frame, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			MP4Protocol_Packet_FrameAudio(lpszClientID, ptszSDBuffer, &nSDLen, (LPCXSTR)ppSt_Frame[i]->unData.ptszMSGBuffer, ppSt_Frame[i]->nMSGLen[0], nFilePos);
			fwrite(ptszSDBuffer, 1, nSDLen, pSt_WFile);
			nFilePos += nSDLen;
			BaseLib_Memory_FreeCStyle((XPPMEM)&ppSt_Frame[i]->unData.ptszMSGBuffer);
		}
		BaseLib_Memory_Free((XPPPMEM)&ppSt_Frame, nListCount);
	}
	MP4Protocol_Packet_HDRBox(lpszClientID, ptszSDBuffer, &nSDLen, "mdat", NULL, nFilePos - nMDatPos - 8);
	fseek(pSt_WFile, nMDatPos, SEEK_SET);
	fwrite(ptszSDBuffer, 1, nSDLen, pSt_WFile);
	fseek(pSt_WFile, nFilePos, SEEK_SET);
	MP4_PacketMoov(lpszClientID, pSt_WFile, nFilePos);

	AVFrame_Frame_ParseClose(xhVideo);
	AVFrame_Frame_ParseClose(xhAudio);
	MP4Protocol_Packet_Delete(lpszClientID);
	fclose(pSt_WFile);
	fclose(pSt_RFile);
	return 0;
}
int main()
{
	MP4_Packet();
	MP4_Parse();

	return 0;
}
