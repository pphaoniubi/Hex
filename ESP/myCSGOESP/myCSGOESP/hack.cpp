#include "include.h"


void Hack::Init() {

	moduleBase = (uintptr_t)GetModuleHandle(L"client.dll");
	engineModule = (uintptr_t)GetModuleHandle(L"engine.dll");

	entList = (EntList*)(moduleBase + dwEntList);
	localEnt = (Ent*)(entList + 0x10);
	viewAngles = (Vec3*)(*(uintptr_t*)(engineModule + dwClientState) + dwClientState_viewAngles);

	iShotsFired = (int*)(localEnt + m_iShotsFired);
	aimPunchAngleNoRecoil = (Vec3*)(localEnt + m_aimPunchAngle);





}


void Hack::Update() { // steal viewMatrix


	memcpy(&viewMatrix, (BYTE*)(moduleBase + dwViewMatrix), sizeof(viewMatrix));

}


bool Hack::checkValidEnt(Ent* ent) {

	if (ent == NULL)
		return false;

	if (ent == localEnt)
		return false;
	if (ent->iHealth == 0) {
		return false;
	}
	if (ent->isDormant)
		return false;


	return true;

}


bool Hack::WorldToScreen(Vec3 pos, Vec2& screen) {
	
	Vec4 clipCoords; 
	clipCoords.x = pos.x * viewMatrix[0] + pos.y * viewMatrix[1] + pos.z * viewMatrix[2] + viewMatrix[3];
	clipCoords.y = pos.x * viewMatrix[4] + pos.y * viewMatrix[5] + pos.z * viewMatrix[6] + viewMatrix[7];
	clipCoords.z = pos.x * viewMatrix[8] + pos.y * viewMatrix[9] + pos.z * viewMatrix[10] + viewMatrix[11];
	clipCoords.w = pos.x * viewMatrix[12] + pos.y * viewMatrix[13] + pos.z * viewMatrix[14] + viewMatrix[15];


	if (clipCoords.w < 0.1f) {		//this means the enemy is behind us
		return false;
	}



	Vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;


	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);


	return true;

}

Vec3 Hack:: GetBonePos(Ent* ent, int bone) {

	uintptr_t bonePtr = ent->boneMatrix;
	Vec3 bonePos;
	bonePos.x = *(float*)(bonePtr + 0x30 * bone + 0x0C);			//HARD CODED YOOOO 
	bonePos.y = *(float*)(bonePtr + 0x30 * bone + 0x1C);				//30 BYTES??
	bonePos.z = *(float*)(bonePtr + 0x30 * bone + 0x2C);

	return bonePos;

}
void noRecoil(Vec3* aimPunchAngleNoRecoil, int* iShotsFired, Vec3 oPunch, Vec3* viewAngles) {

	Vec3 punchAngle = *aimPunchAngleNoRecoil * 2;
	if (*iShotsFired > 1) {
		// calc rcs
		Vec3 newAngle = *viewAngles + oPunch - punchAngle;
		// normalize
		newAngle.Normalize();
		// set
		*viewAngles = newAngle;
	}
	// fix
	oPunch = punchAngle;
}

