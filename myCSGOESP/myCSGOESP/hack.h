#pragma once

//defines pads for us 
#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

#define ABS(x) ((x < 0) ? (-x) : (x))

struct Vec2 {

	float x, y;

};

struct Vec3 {

	float x, y, z;

	Vec3 operator+(Vec3 d) {
		return { x + d.x, y + d.y, z + d.z };
	}
	Vec3 operator-(Vec3 d) {
		return { x - d.x, y - d.y, z - d.z };
	}
	Vec3 operator*(float d) {
		return { x * d, y * d, z * d };
	}

	void Normalize() {
		while (y < -180) { y += 360; }
		while (y > 180) { y -= 360; }
		if (x > 89) { x = 89; }
		if (x < -89) { x = -89; }
	}

};

struct Vec4 {

	float x, y, z, w;

};





class Ent {
public:
	union {
		//isDormant
		DEFINE_MEMBER_N(bool, isDormant, 0xED);
		//iHealth
		DEFINE_MEMBER_N(int, iHealth, 0x100);
		//vecOrigin
		DEFINE_MEMBER_N(Vec3, vecOrigin, 0x138);
		//iTeamNum
		DEFINE_MEMBER_N(int, iTeamNum, 0xF4);			//RIGTH HERE
		//boneMatrix
		DEFINE_MEMBER_N(int, boneMatrix, 0x26A8);
		//armor Value
		DEFINE_MEMBER_N(int, armorValue, 0x117CC);
		//aimPunchAngle
		DEFINE_MEMBER_N(Vec3, aimPunchAngle, 0x303C);

	};
	
};


class EntListObj {
public:
	struct Ent* ent;
	char padding[12];		//uhhh pad leftover bytes??
};

class EntList {

public:
	EntListObj ents [32];
};




class Hack {
public:
	uintptr_t dwEntList = 0x4DD7B1C;
	uintptr_t dwViewMatrix = 0x4DC9434;
	uintptr_t engineModule;	
	uintptr_t moduleBase;
	uintptr_t dwClientState = 0x58BFDC;
	uintptr_t dwClientState_viewAngles = 0x4D90;
	uintptr_t m_iShotsFired = 0x103E0;
	uintptr_t m_aimPunchAngle = 0x303C;
	uintptr_t dwlocalPlayer = 0xDBB5CC;



	Vec2 crosshair2D;
	int crosshairSize = 8;

	Ent* localEnt;
	EntList* entList;
	float viewMatrix[16];

	Vec3* viewAngles;
	int* iShotsFired;
	Vec3* aimPunchAngleNoRecoil;
	Vec3 oPunch{ 0,0,0 };

	void Init();
	void Update();
	bool checkValidEnt(Ent* ent);

	bool WorldToScreen(Vec3 pos, Vec2& screen);
	Vec3 GetBonePos(Ent* ent, int bone);
	
	void noRecoil();

	void Run();
	void AimAt(Vec3* target);
	float GetDistance(Vec3*);
	
	
	Ent* GetClosestEnt();

};


