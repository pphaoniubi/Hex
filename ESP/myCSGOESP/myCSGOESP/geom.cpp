#include "pch.h"
#include "geom.h"


bool WorldToScreen(Vector3 pos, Vector3& screen, float viewMatrix[16], int windoWidth, int windoWHeight) {

	Vec4 clipCoords;

	clipCoords.x = pos.x * viewMatrix[0] + pos.y * viewMatrix[4] + pos.z * viewMatrix[8] + viewMatrix[12];
	clipCoords.y = pos.x * viewMatrix[1] + pos.y * viewMatrix[5] + pos.z * viewMatrix[9] + viewMatrix[13];
	clipCoords.z = pos.x * viewMatrix[2] + pos.y * viewMatrix[6] + pos.z * viewMatrix[10] + viewMatrix[14];
	clipCoords.w = pos.x * viewMatrix[3] + pos.y * viewMatrix[7] + pos.z * viewMatrix[11] + viewMatrix[15];


	if (clipCoords.w < 0.1f) {
		return false;
	}


	Vector3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;			//purpose of this??? 



	screen.x = (windoWidth / 2 * NDC.x) + (NDC.x + windoWidth / 2);
	screen.y = -(windoWHeight / 2 * NDC.y) + (NDC.y + windoWHeight / 2);

	return true;


}
