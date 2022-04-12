#pragma once
#ifndef _CPLAYER2_H_
#define _CPLAYER2_H_

#include "Main.h"
#include "Sprite.h"
#include <list>
#include <vector>
#include <windows.h>
class CPlayer2
{
public:
	enum DIRECTION
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
	CPlayer2(const BackBuffer* pBackBuffer);
	virtual ~CPlayer2();

	void					Update(float dt);
	void					Draw();
	void					Move(ULONG ulDirection);
	Vec2&					Position();
	Vec2&					Velocity();
	void					Explode();
	bool					AdvanceExplosion();

	void ShootRight();

	void ShootLeft();

	double getShipWidth();

	double getShipHeight();

	void RotateShipLeft();

	void RotateShipRight();

	void RotateShipUp();

	void RotateShipDown();

	void updatebullets(std::vector<Sprite*> bulletsP1);

	std::vector<Sprite*> getVectorBullets();

	int getposP2();

private:

	Sprite*					m_pSprite;
	ESpeedStates			m_eSpeedState;
	float					m_fTimer;

	bool					m_bExplosion;
	AnimatedSprite*			m_pExplosionSprite;
	int						m_iExplosionFrame;

	std::vector<Sprite*>	bullets;
	bool					m_bullet;
	const BackBuffer*		pBackBuffer;

	int pos;


};

#endif