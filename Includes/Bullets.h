#pragma once

#include <string>
#include "BackBuffer.h"
#include "Vec2.h"
#include <list>
using namespace std;
// global variables
HPEN					gGunPen;
Vec2					gGundir(100, 200);
list<Vec2>				gBulletList;