#include "include.h"

void patch(BYTE* dst, BYTE* src, unsigned int size) {

	DWORD oProtection;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oProtection);

	memcpy(dst, src, size);

	VirtualProtect(dst, size, oProtection, &oProtection);
	

}

bool Hook(char* src, char* dst, int len) {

	if (len < 5)
		return false;

	DWORD oProtection;

	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oProtection);

	memset(src, 0x90, len);

	uintptr_t relativeAddr = (uintptr_t)(dst - src - 5);
	*src = (char)0xE9;
	*(uintptr_t*)(src + 1) = (uintptr_t)relativeAddr;

	VirtualProtect(src, len, oProtection, &oProtection);


	return true;

}

char* trampHook(char* src, char* dst, unsigned int len) {

	if (len < 5) return 0;


	char* gateway = (char*)VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	memcpy(gateway, src, len);
	uintptr_t jmpAddy = (uintptr_t)(src - gateway - 5);		//still dont understand 

	*(gateway + len) = (char)0xE9;
	*(uintptr_t*)(gateway + len + 1) = jmpAddy;

	if (Hook(src, dst, len)) {

		return gateway;
	}
	else return nullptr;

	

}