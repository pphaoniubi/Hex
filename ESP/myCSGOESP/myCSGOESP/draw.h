#pragma once

#include "hack.h"		//huh

void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR col);


void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color);


void DrawLine(Vec2 src, Vec2 dst, int thickness, D3DCOLOR color);


void DrawESPBox2D(Vec2 top, Vec2 bot, int thickness, D3DCOLOR color);