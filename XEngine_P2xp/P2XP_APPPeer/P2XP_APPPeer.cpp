#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_P2XPPeer.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_P2xp/XEngine_P2XPPeer/P2XPPeer_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_P2xp/XEngine_P2XPPeer/P2XPPeer_Error.h"

//g++ -std=c++17 -Wall -g P2XP_APPPeer.cpp -o P2XP_APPPeer.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_P2xp -lXEngine_BaseLib -lXEngine_P2XPPeer -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_P2xp,--disable-new-dtags
int main()
{
	XENGINE_P2XP_PEERINFO st_P2XPPeer;
	memset(&st_P2XPPeer, '\0', sizeof(XENGINE_P2XP_PEERINFO));

	st_P2XPPeer.bIsLogin = TRUE;
	strcpy(st_P2XPPeer.st_AuthUser.tszUserName, _T("123123aa"));
	strcpy(st_P2XPPeer.st_AuthUser.tszUserPass, _T("123123"));

	st_P2XPPeer.st_PeerTimer.dwUserTime = BaseLib_OperatorTime_GetTickCount64();

	strcpy(st_P2XPPeer.st_PeerAddr.tszUserName, st_P2XPPeer.st_AuthUser.tszUserName);
	strcpy(st_P2XPPeer.st_PeerAddr.tszPublicAddr, _T("140.44.44.44"));
	strcpy(st_P2XPPeer.st_PeerAddr.tszConnectAddr, _T("192.168.1.5"));
	strcpy(st_P2XPPeer.st_PeerAddr.tszPrivateAddr, _T("192.168.1.5"));

	if (!P2XPPeer_Manage_Add(st_P2XPPeer))
	{
		return -1;
	}

	memset(&st_P2XPPeer, '\0', sizeof(XENGINE_P2XP_PEERINFO));

	st_P2XPPeer.bIsLogin = TRUE;
	strcpy(st_P2XPPeer.st_AuthUser.tszUserName, _T("aaadddzxc"));
	strcpy(st_P2XPPeer.st_AuthUser.tszUserPass, _T("123123"));

	st_P2XPPeer.st_PeerTimer.dwUserTime = BaseLib_OperatorTime_GetTickCount64();

	strcpy(st_P2XPPeer.st_PeerAddr.tszUserName, st_P2XPPeer.st_AuthUser.tszUserName);
	strcpy(st_P2XPPeer.st_PeerAddr.tszPublicAddr, _T("140.44.44.44"));
	strcpy(st_P2XPPeer.st_PeerAddr.tszConnectAddr, _T("192.168.1.6"));
	strcpy(st_P2XPPeer.st_PeerAddr.tszPrivateAddr, _T("192.168.1.6"));
	if (!P2XPPeer_Manage_Add(st_P2XPPeer))
	{
		return -2;
	}

	memset(&st_P2XPPeer, '\0', sizeof(XENGINE_P2XP_PEERINFO));

	st_P2XPPeer.bIsLogin = TRUE;
	strcpy(st_P2XPPeer.st_AuthUser.tszUserName, _T("adsasd"));
	strcpy(st_P2XPPeer.st_AuthUser.tszUserPass, _T("123123"));

	st_P2XPPeer.st_PeerTimer.dwUserTime = BaseLib_OperatorTime_GetTickCount64();

	strcpy(st_P2XPPeer.st_PeerAddr.tszUserName, st_P2XPPeer.st_AuthUser.tszUserName);
	strcpy(st_P2XPPeer.st_PeerAddr.tszPublicAddr, _T("140.44.44.55"));
	strcpy(st_P2XPPeer.st_PeerAddr.tszConnectAddr, _T("192.168.1.1"));
	strcpy(st_P2XPPeer.st_PeerAddr.tszPrivateAddr, _T("192.168.1.1"));
	if (!P2XPPeer_Manage_Add(st_P2XPPeer))
	{
		return -3;
	}

	int nWLanCount = 0;
	TCHAR** ppszP2XPWLan;
	P2XPPeer_Manage_GetWList(&ppszP2XPWLan, &nWLanCount);
	for (int i = 0; i < nWLanCount; i++)
	{
		printf("WLan:%s\n", ppszP2XPWLan[i]);

		int nLanCount = 0;
		TCHAR** ppszP2XPLan;
		P2XPPeer_Manage_GetLList(ppszP2XPWLan[i], &ppszP2XPLan, &nLanCount);
		for (int j = 0; j < nLanCount; j++)
		{
			printf("Lan:%s\n", ppszP2XPLan[j]);

			int nListCount = 0;
			XENGINE_P2XPPEER_PROTOCOL** ppSt_P2XPClient;
			P2XPPeer_Manage_GetLan(ppszP2XPWLan[i], ppszP2XPLan[j], &ppSt_P2XPClient, &nListCount);
			for (int k = 0; k < nListCount; k++)
			{
				printf("Addr:User %s Pub %s Pri %s\n", ppSt_P2XPClient[k]->tszUserName, ppSt_P2XPClient[k]->tszPublicAddr, ppSt_P2XPClient[k]->tszPrivateAddr);
			}
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_P2XPClient, nListCount);
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppszP2XPLan, nLanCount);
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppszP2XPWLan, nWLanCount);

	P2XPPeer_Manage_DelAll();
	return 0;
}
