#pragma once


#include <Windows.h>
#include <iostream>
#include <fstream>
#include <TlHelp32.h>

using f_loalLibraryA = HINSTANCE(WINAPI*)(const char * lpLibFilename);

using f_GetProcAddress = UINT_PTR(WINAPI*)(HINSTANCE hModule, const char * lpProcName);

using f_DLL_ENTRY_POINT = BOOL(WINAPI*)(void * hDll, DWORD dwReason, void * pReserved);


struct MANUAL_MAPPING_DATA {


	f_loalLibraryA pLoadLibraryA;
	f_GetProcAddress pGetProcAddress;
	HINSTANCE hMod;




};


bool ManualMap(HANDLE hProc, const char * szDllFile);