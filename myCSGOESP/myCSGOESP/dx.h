#pragma once


bool GetD3D9Device(void** pTable, size_t size);

typedef HRESULT(APIENTRY* tEndScene)(LPDIRECT3DDEVICE9 pDevice);

HWND static window;

HWND getProcessWindow();

extern int windowHeight, windowWidth; //////

extern LPDIRECT3DDEVICE9 pDevice;