#ifndef _CPLAYER_H_
#define _CPLAYER_H_

#include "Main.h"
#include "Sprite.h"
#include <list>
#include <vector>
#include <windows.h>
class CPlayer
{
public:
	enum DIRECTION 
	{ 
		DIR_FORWARD	 = 1, 
		DIR_BACKWARD	= 2, 
		DIR_LEFT		= 4, 
		DIR_RIGHT	   = 8, 
	};

	enum ESpeedStates
	{
		SPEED_START,
		SPEED_STOP
	};
			 CPlayer(const BackBuffer *pBackBuffer);
	virtual ~CPlayer();

	void					Update( float dt );
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

	int getposP1();

private:

	Sprite*					m_pSprite;
	ESpeedStates			m_eSpeedState;
	float					m_fTimer;
	
	bool					m_bExplosion;
	AnimatedSprite*			m_pExplosionSprite;
	int						m_iExplosionFrame;

	std::vector<Sprite*>	bullets;
	bool					m_bullet;
	const BackBuffer* pBackBuffer;

	int pos = 0;

	int getposP2();

};

#endif