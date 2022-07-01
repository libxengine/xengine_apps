#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_UPNPProtocol.lib")
#else
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_UPNPProtocol/UPNPProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_UPNPProtocol/UPNPProtocol_Error.h"

//Linux::g++ -std=c++17 -Wall -g RfcComponents_APPUPnp.cpp -o RfcComponents_APPUPnp.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents -lXEngine_BaseLib -lNetHelp_APIHelp -lRfcComponents_UPNPProtocol -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g RfcComponents_APPUPnp.cpp -o RfcComponents_APPUPnp.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_NetHelp -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_RfcComponents -lXEngine_BaseLib -lNetHelp_APIHelp -lRfcComponents_UPNPProtocol

int main()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

	int nCount = 0;
	XNETHANDLE xhToken;
	UPNPPROTOCOL_DEVICEINFO st_DeviceInfo;

	memset(&st_DeviceInfo, '\0', sizeof(UPNPPROTOCOL_DEVICEINFO));

	UPnpProtocol_Client_Create(&xhToken);
	UPnpProtocol_Client_MSearch(xhToken);
	UPnpProtocol_Client_GetCount(xhToken, &nCount);
	UPnpProtocol_Client_GetDeviceInfo(xhToken, &st_DeviceInfo);
	UPnpProtocol_Client_GetCtrlInfo(xhToken);

	TCHAR tszIPAddr[64];
	memset(tszIPAddr, '\0', sizeof(tszIPAddr));
	UPnpProtocol_Client_GetExternalIPAddress(xhToken, tszIPAddr);
	UPnpProtocol_Client_Close(xhToken);
#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}