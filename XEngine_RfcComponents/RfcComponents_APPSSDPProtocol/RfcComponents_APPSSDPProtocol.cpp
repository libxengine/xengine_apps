﻿#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Core.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_SSDPProtocol.lib")
#else
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Core/NetCore_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Core/NetCore_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_SSDPProtocol/SSDPProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_SSDPProtocol/SSDPProtocol_Error.h"

//Linux::g++ -std=c++17 -Wall -g RfcComponents_APPSSDPProtocol.cpp -o RfcComponents_APPSSDPProtocol.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents -lXEngine_BaseLib -lXEngine_Core -lRfcComponents_SSDPProtocol -lpthread -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g RfcComponents_APPSSDPProtocol.cpp -o RfcComponents_APPSSDPProtocol.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_Core -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_RfcComponents -lXEngine_BaseLib -lXEngine_Core -lRfcComponents_SSDPProtocol -lpthread

int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	SOCKET hSocket;
	//准备
	if (!NetCore_GroupCast_Create(&hSocket, 1900, _T("239.255.255.250"), _T("192.168.1.8"), 0, FALSE))
	{
		printf("NetCore_GroupCast_Create:%lX\n", NetCore_GetLastError());
		return -1;
	}
	int nMsgLen = 0;
	TCHAR tszMsgBuffer[1024];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	SSDPProtocol_Packet_REQSearch(tszMsgBuffer, &nMsgLen, XENGINE_RFCCOMPONENTS_SSDP_PROTOCOL_ST_ALL, "XEngine RFCComponents_SSDPProtocol/V7.45 PCWindows");
	NetCore_GroupCast_Send(hSocket, tszMsgBuffer, nMsgLen);

	while (TRUE)
	{
		nMsgLen = 1024;
		TCHAR tszClientAddr[128];

		memset(tszClientAddr, '\0', sizeof(tszClientAddr));
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		if (NetCore_GroupCast_Recv(hSocket, tszMsgBuffer, &nMsgLen, tszClientAddr))
		{
			int nListCount = 0;
			TCHAR** pptszHDRList;
			TCHAR tszValueStr[MAX_PATH];
			RFCCOMPONENTS_SSDP_HDRPARAM st_SSDPHdr;

			memset(tszValueStr, '\0', MAX_PATH);
			memset(&st_SSDPHdr, '\0', sizeof(RFCCOMPONENTS_SSDP_HDRPARAM));

			SSDPProtocol_Parse_Hdr(tszMsgBuffer, nMsgLen, &st_SSDPHdr, &pptszHDRList, &nListCount);
			if (0 == _tcsnicmp("NOTIFY", st_SSDPHdr.tszHttpMethod, 6))
			{
				SSDPProtocol_Parse_GetField(&pptszHDRList, nListCount, "Location", tszValueStr);
				printf("%s:Location:%s\n", tszClientAddr, tszValueStr);
			}
		}
	}
	NetCore_GroupCast_Close(hSocket);

#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}