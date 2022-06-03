// tramp hook a graphics function, D3D9 for csgo
#include <Windows.h>
#include <iostream>
#include "include.h"


Hack* hack;

//data
void* d3d9Device[119];      //vtable
BYTE EndSceneBytes[7]{ 0 };
tEndScene oEndScene = nullptr;
extern LPDIRECT3DDEVICE9 pDevice = nullptr;



//hook func
void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice) {

    if (!pDevice) {
        pDevice = o_pDevice;
    }


    //draw here
    //looping entList
    for (int i = 1; i < 32; i++) {
        Ent* curEnt = hack->entList->ents[i].ent;



        if (!hack->checkValidEnt(curEnt)) {
            continue;
        }

        D3DCOLOR color;  
       
            if (curEnt->iTeamNum == hack->localEnt->iTeamNum)
                color = D3DCOLOR_ARGB(255, 255, 0, 0);
            else
                color = D3DCOLOR_ARGB(255, 0, 255, 0);
       


        
        Vec3 entHead3D = hack->GetBonePos(curEnt, 8);   //might not be getting the head pos 
        Vec2 entPos2D, entHead2D;
    

        //snapLine
        
        
        if (hack->localEnt->iTeamNum != curEnt->iTeamNum) {

            

        if (hack->WorldToScreen(curEnt->vecOrigin, entPos2D) && hack->WorldToScreen(entHead3D, entHead2D)) {

            DrawLine(entPos2D.x, entPos2D.y, windowWidth / 2, windowHeight, 2, color);
            DrawESPBox2D(entPos2D, entHead2D, 2, color);

        }
                
              
        }
       
    }



    Vec2 l, r, t, b;

    hack->crosshair2D.x = windowWidth / 2;
    hack->crosshair2D.y = windowHeight / 2;


    l = r = t = b = hack->crosshair2D;
    l.x -= hack->crosshairSize;
    r.x += hack->crosshairSize;
    b.y += hack->crosshairSize;
    t.y -= hack->crosshairSize;

    DrawLine(l, r, 2, D3DCOLOR_ARGB(255, 0, 255, 0));
    DrawLine(t, b, 2, D3DCOLOR_ARGB(255,0, 255, 0));



    // call og function
    oEndScene(pDevice);


}



//have to initialize a thread ..... obviously 
DWORD WINAPI HackThread(HMODULE hModule) {
    
    hack = new Hack();
    hack->Init();           //initialize here 



    //hook endScene() to draw boxes
    if (GetD3D9Device(d3d9Device, sizeof(d3d9Device))) {
        memcpy(EndSceneBytes, (char*)d3d9Device[42], 7);

        oEndScene = (tEndScene)trampHook((char*)d3d9Device[42], (char*)hkEndScene, 7);
    
    }

    //hack loop
    while (!GetAsyncKeyState(VK_END)) {

        hack->Update();
       
        //norecoil
        //hack->noRecoil();         //causes unresolved external

       Vec3 punchAngle = hack->localEnt->aimPunchAngle;

        hack->crosshair2D.x = windowWidth / 2 - (windowWidth / 90 * punchAngle.y);
        hack->crosshair2D.y = windowWidth / 2 - (windowWidth / 90 * punchAngle.x);

    }

    //unhook
    patch((BYTE*)d3d9Device[42], EndSceneBytes, 7);





    //please close this thread and free memory afterwards
    FreeLibraryAndExitThread(hModule, 0);
 

}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{

    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {

        CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0));
    }
  
    return TRUE;
}




