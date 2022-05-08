#pragma once

#include <Windows.h>
#include "gemo.h"

enum gameModes
{
    DEMO = -1,
    TEAMDEATHMATCH = 0,           // 0
    COOPEDIT,
    DEATHMATCH,
    SURVIVOR,
	TEAMSURVIVOR,
	CTF,                          // 5
	PISTOLFRENZY,
	BOTTEAMDEATHMATCH,
	BOTDEATHMATCH,
    GMODE_LASTSWISSSTANDING,
    ONESHOTONEKILL,               // 10
    TEAMONESHOTONEKILL,
    BOTONESHOTONEKILL,
	HUNTTHEFLAG,
    TEAMKEEPTHEFLAG,
    KEEPTHEFLAG,                  // 15
    TEAMPF,
	TEAMLSS,
    BOTPISTOLFRENZY,
    BOTLSS,
    BOTTEAMSURVIVOR,              // 20
    BOTTEAMONESHOTONKILL,
    NUM
};

class ent;				//???????
class weapon {

public:
	char pad_0x0000[0x4];
	__int32 ID; //0x0004
	ent * owner; //0x0008
	uintptr_t * guninfo; //0x000c
	int* ammo2; //0x0010
	int* ammo; //0x0014
	int* gunWait;
	int shots;
	int bReload;
};

class ent
{
public:
	DWORD vTable; //0x0				???
	Vector3 head; //0x0004
	char _0x0010[36];
	Vector3 pos; //0x0034
	Vector3 angle; //0x0040
	char _0x004C[37];
	BYTE bScoping; //0x0071
	char _0x0072[134];
	__int32 health; //0x00F8
	__int32 armor; //0x00FC
	char _0x0100[292];
	BYTE bAttacking; //0x0224
	char name[16]; //0x0225
	char _0x0235[247];
	BYTE team; //0x032C
	char _0x032D[11];
	BYTE state; //0x0338
	char _0x0339[59];
	weapon* weapon; //0x0374
	char _0x0378[520];
};




struct entList
{
	ent* ents[31];
};

