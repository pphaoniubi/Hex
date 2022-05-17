#pragma once

#include "assultCube.h"				//wrong 
#include "glDraw.h"
#include "gltext.h"

//scaling //use to fit every resolution???

const int VIRTUAL_SCREEN_WIDTH = 800;
const int GAME_UNIT_MAGIC = 400;


const float PLAYER_HEIGHT = 5.25f;			//comes from reverse engineering // different from every GAME
const float PLAYER_WIDTH = 2.0f;		//comes from reverse engineering // different from every GAME

const float EYE_HEIGHT = 4.5f;    //???

const float PLAYER_ASPECT_RATIO = PLAYER_HEIGHT / PLAYER_WIDTH;


const int ESP_FONT_HEIGHT = 15;
const int ESP_FONT_WIDTH = 9;  //consolas font size (height and width)??



class ESP
{
	//again, comes from reverse engineering. 
public: 
	int* gameMode = (int*)(0x50F49C);
	int* numOfPlayers = (int*)(0x50f500);
	float* matrix = (float*)(0x501ae8);
	ent* localPlayer = *(ent**)(0x50f4f4);				//...
	entList* entlist = *(entList**)(0x50f4f8);			//...

	int viewport[4];  //OPENGL

	bool IsTeamGame();
	bool IsEnemy(ent* e);
	bool IsValidEnt(ent* ent);


	void DrawESPBox(ent* p, Vector3, GL::Font& font);

	void Draw(GL::Font& font);




};

