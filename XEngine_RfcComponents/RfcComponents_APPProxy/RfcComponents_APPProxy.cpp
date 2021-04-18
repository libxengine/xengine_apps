#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_ProxyServer.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <inttypes.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_ProxyServer/ProxyServer_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_ProxyServer/ProxyServer_Error.h"

//g++ -std=c++17 -Wall -g RfcComponents_APPProxy.cpp -o RfcComponents_APPProxy.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents -lXEngine_BaseLib -lRfcComponents_ProxyServer -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents,--disable-new-dtags

BOOL CALLBACK RfcProxy_Callback_User(LPCSTR lpszClientAddr, LPCSTR lpszUserName, LPCSTR lpszUserPass, ENUM_RFCCOMPONENTS_PROXYSOCKS_CBUSERTYPE enStatus, LPVOID lParam)
{
	TCHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	printf(tszMsgBuffer, _T("%s:%s_%s %d\n"), lpszClientAddr, lpszUserName, lpszUserPass, enStatus);

	return TRUE;
}
int main()
{
	if (!RfcComponents_ProxySocks_Start(RfcProxy_Callback_User))
	{
		printf("启动服务失败");
	}
	return 0;
}