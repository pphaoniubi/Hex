//Draw the ESP box only




#include "pch.h"
#include "stdafx.h"
#include <iostream>
#include <TlHelp32.h>
#include "mem.h"
#include "proc.h"
#include "hook.h"
#include "gltext.h"
#include "glDraw.h"
#include "ESP.h"


 //get mod base   internal uses GetModuleHandle   module is just ac_client.exe
uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"ac_client.exe");

bool bHealth = false, bAmmo = false, bRecoil = false;


typedef BOOL(__stdcall* twgSwapBuffers) (HDC hdc);

twgSwapBuffers owglSwapBuffers;

GL::Font glFont;
const int FONT_HEIGHT = 15;
const int FONT_WIDTH = 9;


ESP esp;



void Draw() {

    HDC currentHDC = wglGetCurrentDC();
    
    if (!glFont.bBuilt || currentHDC != glFont.hdc) {
        glFont.Build(FONT_HEIGHT);

    }

    GL::SetupOrtho();

    //std::cout << "check";
    //replacing static ESP box with enemy boxxxesssss yoopiii

    esp.Draw(glFont);

    //std::cout << "finsihed drawing";

    GL::RestoreGL();

}

void ammoHealthNoRecoil() {

    if (GetAsyncKeyState(VK_NUMPAD1) & 1) {
        bHealth = !bHealth;


    }
    if (GetAsyncKeyState(VK_NUMPAD2) & 1) {
        bAmmo = !bAmmo;

    }
    if (GetAsyncKeyState(VK_NUMPAD3) & 1) {
        bRecoil = !bRecoil;


        if (bRecoil) {

            mem::Nop((BYTE*)(moduleBase + 0x63786), 10);
        }
        else {
            //write back original instructions
            mem::Patch((BYTE*)(moduleBase + 0x63786), (BYTE*)"\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2", 10);

        }

    }


    //continuous write

    uintptr_t localPlayerPtr = (moduleBase + 0x10f4f4);

    if (localPlayerPtr) {
        if (bHealth) {

            *(int*)(*(uintptr_t*)localPlayerPtr + 0xF8) = 1337;            // this should work?>????
        }

        if (bAmmo) {

            *(int*)mem::FindDMAAddy((moduleBase + 0x10f4f4), { 0x374, 0x14, 0x0 }) = 1337;

        }
    }


}
//hooked func
BOOL __stdcall wglSwapBuffers(HDC hdc) {

    //std::cout << "Hooked" << std::endl;

    Draw();
    
    ammoHealthNoRecoil();
   
    
    return owglSwapBuffers(hdc);    //HDC: a handle to device context

}

DWORD WINAPI HackThread(HMODULE hModule) {

    //Create a console
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    std::cout << "OG for a fee\n";


    //


    //Hook      redirect to our hook function 
    Hook SwapBuffersHook("wglSwapBuffers", "opengl32.dll", (BYTE*)wglSwapBuffers, (BYTE*)&owglSwapBuffers, 5);
        
        
    SwapBuffersHook.Enable();

    while (true) {
    
        if (GetAsyncKeyState(VK_END)) break;
    }

    SwapBuffersHook.Disable();



    //


    //cleanup and eject
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);


    return 0;
}



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

