#pragma once

void patch(BYTE* dst, BYTE* src, unsigned int size);

bool Hook(char* src, char* dst, int len);

char* trampHook(char* src, char* dst, unsigned int len);