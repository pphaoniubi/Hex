#include "include.h"

typedef __int32 uint32_t;

void Hack::Init() {

	moduleBase = (uintptr_t)GetModuleHandle(L"client.dll");
	engineModule = (uintptr_t)GetModuleHandle(L"engine.dll");

	entList = (EntList*)(moduleBase + dwEntList);
	localEnt = (Ent*)(moduleBase + dwlocalPlayer);
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


Vec3 Hack::GetBonePos(Ent* ent, int bone) {

	uintptr_t bonePtr = ent->boneMatrix;
	Vec3 bonePos;
	bonePos.x = *(float*)(bonePtr + 0x30 * bone + 0x0C);			//HARD CODED YOOOO 
	bonePos.y = *(float*)(bonePtr + 0x30 * bone + 0x1C);				//30 BYTES??
	bonePos.z = *(float*)(bonePtr + 0x30 * bone + 0x2C);

	return bonePos;

}



void Hack::noRecoil() {

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


Ent* Hack::GetClosestEnt() {

	float closestDistance = 1000000;
	int closestDistanceIndex = -1;
	EntList* entList;


	for (int i = 1; i < 32; i++) {			//hard coding max player, losing performance

		Ent* curEnt = GetEnt(i);

		if (!curEnt || !(*(uint32_t*)curEnt) || (uint32_t)curEnt == (uint32_t)localEnt) {
			continue;
		}

		if (curEnt->iTeamNum == localEnt->iTeamNum) {
			continue;
		}

		if (curEnt->iHealth < 1 || localEnt->iHealth < 1) {
			continue;
		}

		float curDistance = localEnt->GetDistance(curEnt->vecOrigin);

		if (curDistance < closestDistance) {
			closestDistance = curDistance;
			closestDistanceIndex = i;
		}

	}

	if (closestDistanceIndex == -1) {
		return NULL;
	}

	return GetEnt(closestDistanceIndex);


}

const double PI = 3.1415926;
void Hack::AimAt(Vec3* target) {

	static uint32_t engineModule = (uint32_t)GetModuleHandle(L"engine.dll");
	static Vec3* viewAngles = (Vec3*)(*(uint32_t*)(engineModule + dwClientState) + dwClientState_viewAngles);

	Vec3 origin = *GetOrigin();
	Vec3 viewOffset = *(Vec3*)this.vecOrigin;
	Vec3* myPos = &(origin + viewOffset);					//??????


	Vec3* deltaVec; 
	deltaVec->x = target->x - myPos->x; 
	deltaVec->y = target->y - myPos->y; 
	deltaVec->z = target->z - myPos->z;					//substracting enemy head and our head


	float deltaVecLength = sqrt(deltaVec->x * deltaVec->x + deltaVec->y * deltaVec->y + deltaVec->z * deltaVec->z);
	//get the length of the hypotenus

	float pitch = -asin(deltaVec->z / deltaVecLength) * (180/ PI);			//converting to deg
	float yaw = atan2(deltaVec->y, deltaVec->x) * (180/PI);


	if (pitch >= -89 && pitch <= 89 && yaw >= -180 && yaw <= 180) {
		viewAngles->x = pitch;
		viewAngles->y = yaw;
	}


}
void Hack::Run() {

	Ent* closestEnt = GetClosestEnt();

	if (closestEnt) {

		Hack::AimAt(closestEnt->GetBonePos(8));
	 
	}
}
