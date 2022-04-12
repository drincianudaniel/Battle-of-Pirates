#pragma once
//-----------------------------------------------------------------------------
// File: CPlayer.cpp
//
// Desc: This file stores the player object class. This class performs tasks
//	   such as player movement, some minor physics as well as rendering.
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

#ifndef _ENEMY_H_
#define _ENEMY_H_

//-----------------------------------------------------------------------------
// CPlayer Specific Includes
//-----------------------------------------------------------------------------
#include "Main.h"
#include "Sprite.h"
#include <list>
#include <vector>
#include <windows.h>
#include <random>
#include <iterator>
#include <time.h>
#include <ctime>
//-----------------------------------------------------------------------------
// Main Class Definitions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name : CPlayer (Class)
// Desc : Player class handles all player manipulation, update and management.
//-----------------------------------------------------------------------------
class Enemy
{
public:
	//-------------------------------------------------------------------------
	// Enumerators
	//-------------------------------------------------------------------------
	enum DIRECTION2
	{
		DIR_FORWARD = 1,
		DIR_BACKWARD = 2,
		DIR_LEFT = 4,
		DIR_RIGHT = 8,
	};

	enum ESpeedStates
	{
		SPEED_START,
		SPEED_STOP
	};

	//-------------------------------------------------------------------------
	// Constructors & Destructors for This Class.
	//-------------------------------------------------------------------------
	Enemy(const BackBuffer* pBackBuffer);
	virtual ~Enemy();

	//-------------------------------------------------------------------------
	// Public Functions for This Class.
	//-------------------------------------------------------------------------
	void					Update(float dt);
	void					Draw();
	void SpawnEnemy();
	void MoveEnemy();
	Vec2& Position();
	Vec2& Velocity();
	double					getShipWidth();
	std::vector<Sprite*> getVectorEnemy();
	void updateEnemy(std::vector<Sprite*> enemies);
	void clearVector();
	void spawnMegalodon();
	void moveMegalodon();
	void updateMegalodon(float dt);
	void updateBarrel(float dt);
	void drawBarrel();
	std::vector<Sprite*> getBarrelVector();
	void drawMegalodon();
	std::vector<Sprite*> getMegalodonVector();
	void updateMegalodonVector(std::vector<Sprite*> megalodon);
	void updateBarrelVector(std::vector<Sprite*> barrels);
	Vec2& PositionBarrel();
	void setPosition(Vec2& position);
	void updatePorts(float dt);
	void spawnBarrel();
	std::vector<Sprite*> getBulletsEnemy();
	void					Shoot();
	int getposP2();
	void updatebullets(std::vector <Sprite*> bulletsP2);


private:
	//-------------------------------------------------------------------------
	// Private Variables for This Class.
	//-------------------------------------------------------------------------
	std::vector<Sprite*> enemies;
	ESpeedStates			m_eSpeedState;
	float					m_fTimer;



	std::vector<Sprite*>	bullets;
	bool					m_bullet;
	const BackBuffer* pBackBuffer;

	int pos = 1;

	std::vector<Sprite*> megalodon;
	std::vector<Sprite*> barrels;
	Sprite* port1;
	Sprite* port2;
	template<typename Iter, typename RandomGenerator>
	Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
		std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
		std::advance(start, dis(g));
		return start;
	}

	template<typename Iter>
	Iter select_randomly(Iter start, Iter end) {
		static std::random_device rd;
		static std::mt19937 gen(rd());
		return select_randomly(start, end, gen);
	}
	int size = 30;
	int aux = 30;
	int n = 0;

	int i = 0;
};

#endif // _CPLAYER2_H_