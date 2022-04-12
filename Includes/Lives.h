#pragma once
#pragma once
#pragma once
#pragma once
//-----------------------------------------------------------------------------
// File: CPlayer.cpp
//
// Desc: This file stores the player object class. This class performs tasks
//	   such as player movement, some minor physics as well as rendering.
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

#ifndef _LIVES_H__
#define _LIVES_H_

//-----------------------------------------------------------------------------
// CPlayer Specific Includes
//-----------------------------------------------------------------------------
#include "Main.h"
#include "Sprite.h"
#include <list>
#include <vector>
#include <windows.h>
#include <cstdlib>
#include <iostream>
#include <time.h>



//-----------------------------------------------------------------------------
// Main Class Definitions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name : CPlayer (Class)
// Desc : Player class handles all player manipulation, update and management.
//-----------------------------------------------------------------------------
class Lives
{
public:
	//-------------------------------------------------------------------------
	// Enumerators
	//-------------------------------------------------------------------------


	//-------------------------------------------------------------------------
	// Constructors & Destructors for This Class.
	//-------------------------------------------------------------------------
	Lives(const BackBuffer* pBackBuffer);
	virtual ~Lives();

	//-------------------------------------------------------------------------
	// Public Functions for This Class.
	//-------------------------------------------------------------------------
	void					Update(float dt);
	void					Draw();
	void					DrawHearts();
	std::vector<Sprite*> getVectorHearts();
	std::vector<Sprite*> getVectorHearts2();
	void updateLives2(std::vector<Sprite*> hearts);
	void updateLives(std::vector <Sprite*> hearts);
	void AddLifeP1();
	void AddLifeP2();
	int NoLivesP1();
	int NoLivesP2();
	void SetNumberLivesP1(int);
	void SpawnHearts();
	void SetNumberLivesP2(int);



private:
	//-------------------------------------------------------------------------
	// Private Variables for This Class.
	//-------------------------------------------------------------------------
	std::vector<Sprite*>	hearts;
	std::vector<Sprite*>	hearts2;
	const BackBuffer* pBackBuffer;
};

#endif