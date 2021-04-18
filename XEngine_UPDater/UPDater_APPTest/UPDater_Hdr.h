#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/HelpComponents_XLog.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/UPData_FileParser.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/UPData_Downloads.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/UPData_HelpModule.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <signal.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_HelpComponents/HelpComponents_XLog/XLog_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_HelpComponents/HelpComponents_XLog/XLog_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_UPData/UPData_FileParser/FileParser_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_UPData/UPData_FileParser/FileParser_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_UPData/UPData_Downloads/UPDataDl_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_UPData/UPData_Downloads/UPDataDl_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_UPData/UPData_HelpModule/HelpModule_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_UPData/UPData_HelpModule/HelpModule_Error.h"

typedef struct tag_UPDaterService_Config
{
	TCHAR tszUPUrl[MAX_PATH];
	TCHAR tszLocalList[MAX_PATH];
	TCHAR tszDownPath[MAX_PATH];
	TCHAR tszMakePath[MAX_PATH];
	BOOL bIsMake;
	BOOL bUPSys;
	BOOL bIsDown;
	int nLogType;
}UPDATERSERVICE_CONFIG;

extern BOOL bIsRun;
extern XLOG xhLog;
extern UPDATERSERVICE_CONFIG st_P2XPConfig;

#include "UPDater_Config.h"
#include "UPDater_Net.h"