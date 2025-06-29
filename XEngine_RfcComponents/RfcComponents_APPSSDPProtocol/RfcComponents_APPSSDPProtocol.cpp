﻿#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#else
#include <unistd.h>
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
#include <XEngine_Include/XEngine_Core/NetCore_Define.h>
#include <XEngine_Include/XEngine_Core/NetCore_Error.h>
#include <XEngine_Include/XEngine_Client/APIClient_Define.h>
#include <XEngine_Include/XEngine_Client/APIClient_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/SSDPProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/SSDPProtocol_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_Core/XEngine_Core.lib")
#pragma comment(lib,"XEngine_Client/XClient_APIHelp.lib")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_SSDPProtocol.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Core/NetCore_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Core/NetCore_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_SSDPProtocol/SSDPProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_SSDPProtocol/SSDPProtocol_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_APIHelp/APIClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_APIHelp/APIClient_Error.h"
#ifdef _MSC_BUILD
#ifdef _WIN64
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_Core.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/RfcComponents_SSDPProtocol.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XClient_APIHelp.lib")
#else
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Core.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_SSDPProtocol.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_APIHelp.lib")
#endif
#endif
#endif

//Linux::g++ -std=c++20 -Wall -g RfcComponents_APPSSDPProtocol.cpp -o RfcComponents_APPSSDPProtocol.exe -lXEngine_BaseLib -lXEngine_Core -lRfcComponents_SSDPProtocol -lNetHelp_APIClient -lpthread

int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	XSOCKET hSocket;
	//准备
	if (!NetCore_GroupCast_Create(&hSocket, 1900, _X("239.255.255.250"), _X("10.0.1.88")))
	{
		printf("NetCore_GroupCast_Create:%lX\n", NetCore_GetLastError());
		return -1;
	}
	int nMsgLen = 0;
	XCHAR tszMsgBuffer[1024];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	SSDPProtocol_Packet_REQSearch(tszMsgBuffer, &nMsgLen, XENGINE_RFCCOMPONENTS_SSDP_PROTOCOL_ST_ALL, "XEngine RFCComponents_SSDPProtocol/V7.45 PCWindows");
	NetCore_GroupCast_Send(hSocket, tszMsgBuffer, nMsgLen);

	while (true)
	{
		nMsgLen = 1024;
		XCHAR tszClientAddr[128];

		memset(tszClientAddr, '\0', sizeof(tszClientAddr));
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		if (NetCore_GroupCast_Recv(hSocket, tszMsgBuffer, &nMsgLen, tszClientAddr))
		{
			int nListCount = 0;
			XCHAR** pptszHDRList;
			XCHAR tszValueStr[XPATH_MAX];
			RFCCOMPONENTS_SSDP_HDRPARAM st_SSDPHdr;

			memset(tszValueStr, '\0', XPATH_MAX);
			memset(&st_SSDPHdr, '\0', sizeof(RFCCOMPONENTS_SSDP_HDRPARAM));

			SSDPProtocol_Parse_Hdr(tszMsgBuffer, nMsgLen, &st_SSDPHdr, &pptszHDRList, &nListCount);
			if (0 == _tcsnicmp("NOTIFY", st_SSDPHdr.tszHttpMethod, 6))
			{
				SSDPProtocol_Parse_GetField(&pptszHDRList, nListCount, "Location", tszValueStr);
				printf("%s:Location:%s\n", tszClientAddr, tszValueStr);

				int nBLen = 0;
				XCHAR* ptszMsgBuffer = NULL;
				APIClient_Http_Request("GET", tszValueStr, NULL, NULL, &ptszMsgBuffer, &nBLen);
				printf("%s\n", ptszMsgBuffer);
				BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
				continue;
			}
		}
	}
	NetCore_GroupCast_Close(hSocket);

#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}