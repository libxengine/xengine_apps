#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_ProxyServer.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_ProxyServer/ProxyServer_Error.h"


//g++ -std=c++17 -Wall -g RfcComponents_APPTunnel.cpp -o RfcComponents_APPTunnel.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents -lXEngine_BaseLib -lXEngine_Core -lRfcComponents_HttpServer -lRfcComponents_SIPPorotocol -lpthread -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk,--disable-new-dtags

void CALLBACK RfcComponents_Callback_Tunnel(LPCSTR lpszClientAddr, LPCSTR lpszMsgBuffer, int nMsgLen, BOOL bIsHttp, LPVOID lParam)
{

}

int main()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	int nListCount = 1;
	TCHAR** pptszListUser;
	BaseLib_OperatorMemory_Malloc((XPPPMEM)&pptszListUser, nListCount, sizeof(TCHAR));
	for (int i = 0; i < nListCount; i++)
	{
		strcpy(pptszListUser[i], "MTIzMTIzYWE6MTIzMTIz");
	}

	if (!RfcComponents_ProxyTunnel_Start(10081, &pptszListUser, nListCount, RfcComponents_Callback_Tunnel))
	{
		printf("错误\n");
		return -1;
	}
	printf("启动服务成功\n");
	while (1)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	BaseLib_OperatorMemory_Free((XPPPMEM)&pptszListUser, nListCount);
	RfcComponents_ProxyTunnel_Stop();
#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}
