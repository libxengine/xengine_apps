#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_ImageRecognition.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Image/XEngine_ImageRecognition/ImageRecognition_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Image/XEngine_ImageRecognition/ImageRecognition_Error.h"

//g++ -std=c++17 -Wall -g ImageRecognition_APPTest.cpp -o ImageRecognition_APPTest.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Image -lXEngine_ImageInformation -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Image,--disable-new-dtags

int Video_QRScan()
{
	TCHAR tszMsgBuffer[1024];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	if (ImageRecognition_Help_QRCamera(tszMsgBuffer, 0, 5))
	{
		printf("QR:%s\n", tszMsgBuffer);
	}
	else
	{
		printf("error:%lX\n", ImageRecognition_GetLastError());
	}
	return 0;
}
int main()
{
	Video_QRScan();
	return 0;
	LPCTSTR lpszFile = _T("D:\\xengine_apps\\Debug\\1.png");

	TCHAR tszMsgBuffer[1024];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	ImageRecognition_Help_QRDecodec(lpszFile, NULL, tszMsgBuffer);
	printf("%s\n", tszMsgBuffer);
	return 0;
}