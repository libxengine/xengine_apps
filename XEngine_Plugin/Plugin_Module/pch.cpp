// pch.cpp: 与预编译标头对应的源文件

#include "pch.h"

// 当使用预编译的头时，需要使用此源文件，编译才能成功。
extern "C" BOOL PluginCore_Init(LPVOID lParam)
{
	return TRUE;
}
extern "C" BOOL PluginCore_UnInit()
{
	return TRUE;
}
extern "C" BOOL PluginCore_Call(LPVOID lPAaram, LPVOID lPBaram)
{
	return TRUE;
}
extern "C" DWORD PluginCore_GetLastError()
{
	return 110;
}
extern "C" void PluginCore_Print(LPVOID lPAaram, LPVOID lPBaram)
{
	printf("%s\n", (LPCSTR)lPAaram);
}