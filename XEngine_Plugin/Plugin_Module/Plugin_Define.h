#pragma once

extern "C" BOOL PluginCore_Init(LPVOID lParam);
extern "C" BOOL PluginCore_UnInit();
extern "C" BOOL PluginCore_Call(LPVOID lPAaram, LPVOID lPBaram);
extern "C" DWORD PluginCore_GetLastError();
extern "C" void PluginCore_Print(LPVOID lPAaram, LPVOID lPBaram);