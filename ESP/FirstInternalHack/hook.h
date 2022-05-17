#pragma once

#include <Windows.h>
#include "mem.h"

bool Detour32(BYTE* src, BYTE* dst, const uintptr_t len);
BYTE* TrampHook32(BYTE* src, BYTE* dst, const uintptr_t len);



struct Hook {

	bool bStatus{ false };
	BYTE* src{ nullptr };
	BYTE* dst{ nullptr };

	BYTE* PtrToGatewayFnPtr{ nullptr };
	int len{ 0 };

	BYTE originalByte[10]{ 0 };


	Hook(BYTE* src, BYTE* dst, BYTE* PtrToGatewayFnPtr, uintptr_t len);

	Hook(const char* exportName, const char* modName, BYTE* dst, BYTE* PtrToGatewayPtr, uintptr_t len);

	void Enable();
	void Disable();
	void Toggle();
};