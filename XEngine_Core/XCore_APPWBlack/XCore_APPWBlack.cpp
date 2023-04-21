#ifdef _MSC_BUILD
#include <stdio.h>
#include <Windows.h>
#include <tchar.h>
#include <locale.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_WBlackList.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#endif
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_WBlackList/WBlackList_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_WBlackList/WBlackList_Error.h"

//Linux::g++ -std=gnu++17 -Wall -g XCore_APPWBlack.cpp -o XCore_APPWBlack.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -lXEngine_BaseLib -lXEngine_WBlackList -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core,--disable-new-dtags
//Macos::g++ -std=gnu++17 -Wall -g XCore_APPWBlack.cpp -o XCore_APPWBlack.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_Core -lXEngine_BaseLib -lXEngine_WBlackList

int Test_IPV4BlackList()
{
	XNETHANDLE xhNet;
	LPCXSTR lpszStartAddr1 = _X("192.168.1.115");
	LPCXSTR lpszStartAddr2 = _X("192.168.2.2");
	LPCXSTR lpszEndAddr2 = _X("192.168.2.115");
	LPCXSTR lpszExist1 = _X("192.168.2.2");
	LPCXSTR lpszExist2 = _X("192.168.2.118");

	if (!WBlackList_IPV4Addr_Init(&xhNet))
	{
		return -1;
	}
	WBlackList_IPV4Addr_Add(xhNet, lpszStartAddr1);
	WBlackList_IPV4Addr_Add(xhNet, lpszStartAddr2, lpszEndAddr2, true);
	WBlackList_IPV4Addr_Add(xhNet, lpszExist1, lpszExist2, true);
	WBlackList_IPV4Addr_Add(xhNet, "192.168.1.1", "192.168.2.255", true);
	WBlackList_IPV4Addr_Add(xhNet, "1.1.1.1", "195.222.2.255", false);
	WBlackList_IPV4Addr_Add(xhNet, "200.1.2.2", "200.1.10.255", true);
	WBlackList_IPV4Addr_Add(xhNet, "200.1.-1.2");

	int nBlackCount = 0;
	BLACKLIST_IPV4ADDR **ppSt_ListBlack;

	WBlackList_IPV4Addr_GetList(xhNet, &ppSt_ListBlack, &nBlackCount);
	for (int i = 0; i < nBlackCount; i++)
	{
		printf("%s -> %s\n", ppSt_ListBlack[i]->tszAddrStart, ppSt_ListBlack[i]->tszAddrEnd);
	}

	if (WBlackList_IPV4Addr_IsExistBlack(xhNet, "200.1.2.30"))
	{
		printf("找到\n");
	}
	else
	{
		printf("没找到\n");
	}

	WBlackList_IPV4Addr_AddWhite(xhNet, "200.1.2.30");
	if (WBlackList_IPV4Addr_IsExistBlack(xhNet, "200.1.2.30"))
	{
		printf("找到\n");
	}
	else
	{
		printf("没找到\n");
	}
	WBlackList_IPV4Addr_Destory(xhNet);
	return 0;
}
int Test_DomainBlackList()
{
	XNETHANDLE xhNet;
	LPCXSTR lpszDomain1 = _X("http://www.xyry.org");
	LPCXSTR lpszDomain2 = _X("*.*.cctv.org");
	LPCXSTR lpszDomain3 = _X("http://*.cctv.org/1.html");
	LPCXSTR lpszDomain4 = _X("*.*.*.cctv.org");
	LPCXSTR lpszDomain5 = _X("*.app.cctv.org");
	LPCXSTR lpszDomainExist = _X("app.cctv.org");

	if (!WBlackList_Domain_Init(&xhNet))
	{
		return -1;
	}
	WBlackList_Domain_Add(xhNet, lpszDomain1);
	WBlackList_Domain_Add(xhNet, lpszDomain2);
	WBlackList_Domain_Add(xhNet, lpszDomain3);
	WBlackList_Domain_Add(xhNet, lpszDomain4);
	WBlackList_Domain_Add(xhNet, lpszDomain5);

	int nBlackCount = 0;
	XCHAR **ppszListBlack;
	WBlackList_Domain_GetList(xhNet, &ppszListBlack, &nBlackCount);
	for (int i = 0; i < nBlackCount; i++)
	{
		printf("%s\n", ppszListBlack[i]);
	}

	if (WBlackList_Domain_IsExistBlack(xhNet, lpszDomainExist))
	{
		printf("找到\n");
	}
	else
	{
		printf("没找到\n");
	}

	WBlackList_Domain_AddWhite(xhNet, lpszDomainExist);
	if (WBlackList_Domain_IsExistBlack(xhNet, lpszDomainExist))
	{
		printf("找到\n");
	}
	else
	{
		printf("没找到\n");
	}
	WBlackList_Domain_Destory(xhNet);
	return 0;
}

int Test_IPV6BlackList()
{
	XNETHANDLE xhNet;
	LPCXSTR lpszStartAddr1 = _X("FF01:0:0:0:1:0:0:1");
	LPCXSTR lpszStartAddr2 = _X("FF01:0:0:0:0:0:2:1");
	LPCXSTR lpszEndAddr2 = _X("FF01:0:0:0:0:0:2:222");
	LPCXSTR lpszExist1 = _X("FF01:0:0:0:0:0:-1:1");
	LPCXSTR lpszExist2 = _X("FF01:0:0:0:0:0:3:1101");

	if (!WBlackList_IPV6Addr_Init(&xhNet))
	{
		return -1;
	}
	WBlackList_IPV6Addr_Add(xhNet, lpszStartAddr1);
	WBlackList_IPV6Addr_Add(xhNet, lpszStartAddr2, lpszEndAddr2, false);
	WBlackList_IPV6Addr_Add(xhNet, lpszExist1, lpszExist2, false);
	WBlackList_IPV6Addr_Add(xhNet, "FF01:0:0:0:0:0:0:1", "FF01:0:0:0:0:0:0:FF", false);

	int nListCount = 0;
	BLACKLIST_IPV6ADDR** ppSt_BlackList;
	WBlackList_IPV6Addr_GetList(xhNet, &ppSt_BlackList, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		printf("%s -> %s\n", ppSt_BlackList[i]->tszAddrStart, ppSt_BlackList[i]->tszAddrEnd);
	}

	if (WBlackList_IPV6Addr_IsExistBlack(xhNet, lpszStartAddr1))
	{
		printf("找到\n");
	}
	else
	{
		printf("没找到\n");
	}

	WBlackList_IPV6Addr_AddWhite(xhNet, lpszStartAddr1);
	if (WBlackList_IPV6Addr_IsExistBlack(xhNet, lpszStartAddr1))
	{
		printf("找到\n");
	}
	else
	{
		printf("没找到\n");
	}
	WBlackList_IPV6Addr_Destory(xhNet);
	return 0;
}

int main()
{
	Test_IPV4BlackList();
	Test_DomainBlackList();
	Test_IPV6BlackList();

	return 1;
}