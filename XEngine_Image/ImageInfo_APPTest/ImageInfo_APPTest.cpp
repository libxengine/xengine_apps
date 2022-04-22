#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#ifdef _DEBUG
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_SystemApi.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_ImageInformation.lib")
#else
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Release/XEngine_SystemApi.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Release/XEngine_ImageInformation.lib")
#endif
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_ProcFile/ProcFile_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_ProcFile/ProcFile_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_SystemApi/SystemApi_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_SystemSdk/XEngine_SystemApi/SystemApi_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Image/XEngine_ImageInformation/ImageInfo_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Image/XEngine_ImageInformation/ImageInfo_Error.h"

//g++ -std=c++17 -Wall -g ImageInfo_APPTest.cpp -o ImageInfo_APPTest.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Image -lXEngine_ImageInformation -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Image,--disable-new-dtags

int Image_Small()
{
	LPCTSTR lpszDir = _T("./");
	int nListCount = 0;
	CHAR** ppszListDir = NULL; 
	SystemApi_File_EnumFile(lpszDir, &ppszListDir, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		IMAGEINFO_BASEATTR st_BaseInfo;
		memset(&st_BaseInfo, '\0', sizeof(IMAGEINFO_BASEATTR));

		ImageInfo_Get_Attr(ppszListDir[i], &st_BaseInfo);
		if (ImageInfo_Set_FileResolution(ppszListDir[i], st_BaseInfo.nHeigth / 2, st_BaseInfo.nWidth / 2))
		{
			printf("裁剪文件:%s 成功\r\n", ppszListDir[i]);
		}
		else
		{
			printf("裁剪文件:%s 失败\r\n", ppszListDir[i]);
		}
	}
	return 0;
}
int main()
{
	Image_Small();
	return 0;
	IMAGEINFO_BASEATTR st_BaseInfo;
	IMAGEINFO_EXTENDATTR st_ExtInfo;
	LPCTSTR lpszFile = _T("D:\\xengine_apps\\Debug\\1.png");

	memset(&st_BaseInfo, '\0', sizeof(IMAGEINFO_BASEATTR));
	memset(&st_ExtInfo, '\0', sizeof(IMAGEINFO_EXTENDATTR));

	ImageInfo_Get_Attr(lpszFile, &st_BaseInfo, &st_ExtInfo);
	ImageInfo_Set_FileResolution(lpszFile, 1024, 768);
	return 0;
}