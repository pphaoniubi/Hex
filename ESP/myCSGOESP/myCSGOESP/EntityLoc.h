#pragma once
#include <Windows.h>
#include "geom.h"

Vector3 GetEntityLoc(HANDLE handle, uintptr_t entListAddr, uintptr_t entityDist);
