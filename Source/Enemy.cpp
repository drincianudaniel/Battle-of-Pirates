//-----------------------------------------------------------------------------
// File: CPlayer.cpp
//
// Desc: This file stores the player object class. This class performs tasks
//       such as player movement, some minor physics as well as rendering.
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CPlayer Specific Includes
//-----------------------------------------------------------------------------
#include "Enemy.h"


//-----------------------------------------------------------------------------
// Name : CPlayer () (Constructor)
// Desc : CPlayer Class Constructor
//-----------------------------------------------------------------------------
Enemy::Enemy(const BackBuffer* pBackBuffer)
{

	int n = 0;
	do {
		SpawnEnemy();
		n++;
	} while (n < 30);


	MoveEnemy();
	this->pBackBuffer = pBackBuffer;
	RECT r;
	r.left = 0;
	r.top = 0;
	r.right = 128;
	r.bottom = 128;

	srand(time(0));

	port1 = new Sprite("data/Port.bmp", RGB(255, 255, 255));
	port2 = new Sprite("data/Port2.bmp", RGB(255, 255, 255));

	port1->mPosition.x = 1729;
	port1->mPosition.y = 890;

	port2->mPosition.x = 191;
	port2->mPosition.y = 890;
}
//-----------------------------------------------------------------------------
// Name : ~CPlayer () (Destructor)
// Desc : CPlayer Class Destructor
//-----------------------------------------------------------------------------
Enemy::~Enemy()
{
	for (Sprite* sprite : enemies) {
		delete sprite;
	}

}

void Enemy::Update(float dt)
{
	RECT desktop;
	GetWindowRect(GetDesktopWindow(), &desktop);
	// Update sprite
	if (!(enemies.empty()))
	{
		double maxleft = enemies.at(0)->mPosition.x;
		double maxright = enemies.back()->mPosition.x;
		for (Sprite* sprite : enemies)
		{
			if (sprite->mPosition.x < maxleft)
			{
				maxleft = sprite->mPosition.x;
			}
			if (sprite->mPosition.x > maxright)
			{
				maxright = sprite->mPosition.x;
			}
		}

		for (Sprite* sprite : enemies) {
			sprite->update(dt);
			if (maxleft < desktop.left + 55)
			{
				sprite->mVelocity.x = 100;
			}

			if (maxright > desktop.right - 55)
			{
				sprite->mVelocity.x = -100;
			}

		}
	}




	int i = 0;
	for (Sprite* bullet : bullets) {
		bullet->update(dt);

		if (bullet->mPosition.y > desktop.bottom)
		{
			bullets.erase(bullets.begin() + i);
		}
		i++;
	}


}
void Enemy::updatebullets(std::vector <Sprite*> bulletsP2)
{
	this->bullets = bulletsP2;
}
void Enemy::Draw()
{

	for (Sprite* bullet : bullets)
	{
		bullet->setBackBuffer(pBackBuffer);
		bullet->draw();
	}


	for (Sprite* sprite : enemies) {
		sprite->setBackBuffer(pBackBuffer);
		sprite->draw();
	}


}

void Enemy::SpawnEnemy()
{

	if (enemies.size() % 10 == 0)
	{
		enemies.push_back(new Sprite("data/enemy.bmp", RGB(0xff, 0x00, 0xff)));
		enemies.at(enemies.size() - 1)->mPosition.x = 300;
		enemies.at(enemies.size() - 1)->mPosition.y = (((enemies.size() - 1) / 10) + 1) * 100;
	}
	else
	{
		enemies.push_back(new Sprite("data/enemy.bmp", RGB(0xff, 0x00, 0xff)));
		enemies.at(enemies.size() - 1)->mPosition.x = enemies.at(enemies.size() - 2)->mPosition.x + 150;
		enemies.at(enemies.size() - 1)->mPosition.y = (((enemies.size() - 1) / 10) + 1) * 100;
	}



	/*else
	{
		if (enemies.size() == 0)
		{
			enemies.push_back(new Sprite("data/enemy.bmp", RGB(0xff, 0x00, 0xff)));
			enemies.at(0)->mPosition.x = 300;
			enemies.at(0)->mPosition.y = 100;
		}
		else
		{
			enemies.push_back(new Sprite("data/enemy.bmp", RGB(0xff, 0x00, 0xff)));
			enemies.at(enemies.size() - 1)->mPosition.x = enemies.at(enemies.size() - 2)->mPosition.x + 150;
			enemies.at(enemies.size() - 1)->mPosition.y = 100;
		}
	}*/



}

void Enemy::MoveEnemy()
{
	RECT desktop;
	GetWindowRect(GetDesktopWindow(), &desktop);
	for (Sprite* sprite : enemies) {
		sprite->mVelocity.x = -100;
	}

}

Vec2& Enemy::Position()
{
	for (Sprite* sprite : enemies) {
		return sprite->mPosition;
	}

}

Vec2& Enemy::Velocity()
{
	for (Sprite* sprite : enemies) {
		return sprite->mVelocity;
	}
}

std::vector<Sprite*> Enemy::getBulletsEnemy()
{
	return bullets;
}

void Enemy::Shoot()
{
	Sprite* sprite = *select_randomly(enemies.begin(), enemies.end());
	bullets.push_back(new Sprite("data/bullet2.bmp", RGB(0xff, 0x00, 0xff)));
	bullets.back()->mPosition = sprite->mPosition;
	bullets.back()->mVelocity.y = 1000;
}

int Enemy::getposP2()
{
	return pos;
}
double Enemy::getShipWidth()
{
	for (Sprite* sprite : enemies) {
		return sprite->width();
	}

}
std::vector<Sprite*> Enemy::getVectorEnemy()
{
	return enemies;
}

void Enemy::updateEnemy(std::vector <Sprite*> enemies)
{
	this->enemies = enemies;
}


void Enemy::clearVector()
{
	enemies.clear();
}

void Enemy::spawnMegalodon()
{
	i++;
	if (i % 2 == 0)
	{
		megalodon.push_back(new Sprite("data/megalodon2.bmp", RGB(255, 255, 255)));
		megalodon.back()->mPosition.x = 0;
		megalodon.back()->mPosition.y = rand() % 550 + 200;
		megalodon.back()->mVelocity.x = 500;
	}
	else
	{
		megalodon.push_back(new Sprite("data/megalodon.bmp", RGB(255, 255, 255)));
		megalodon.back()->mPosition.x = 1920;
		megalodon.back()->mPosition.y = rand() % 550 + 200;
		megalodon.back()->mVelocity.x = -500;
	}
}

void Enemy::moveMegalodon()
{
	
}

void Enemy::updateMegalodon(float dt)
{
	for (Sprite* meg : megalodon)
	{
		meg->update(dt);
	}
}

void Enemy::drawMegalodon()
{
	for (Sprite* meg : megalodon)
	{
		meg->setBackBuffer(pBackBuffer);
		meg->draw();
	}
	port1->setBackBuffer(pBackBuffer);
	port1->draw();
	port2->setBackBuffer(pBackBuffer);
	port2->draw();
}

std::vector<Sprite*> Enemy::getMegalodonVector()
{
	return megalodon;
}

void Enemy::updateMegalodonVector(std::vector <Sprite*> megalodon)
{
	this->megalodon = megalodon;
}

void Enemy::updatePorts(float dt)
{
	port1->update(dt);
	port2->update(dt);
}


void Enemy::spawnBarrel()
{
	barrels.push_back(new Sprite("data/barrel.bmp", RGB(255, 255, 255)));
}


void Enemy::updateBarrel(float dt)
{
	for (Sprite* barrel : barrels)
	{
		barrel->update(dt);
	}
}

void Enemy::drawBarrel()
{
	for (Sprite* barrel : barrels)
	{
		barrel->setBackBuffer(pBackBuffer);
		barrel->draw();
	}
}

std::vector<Sprite*> Enemy::getBarrelVector()
{
	return barrels;
}

void Enemy::updateBarrelVector(std::vector <Sprite*> barrels)
{
	this->barrels = barrels;
}

Vec2& Enemy::PositionBarrel()
{
	for (Sprite* barrel : barrels) {
		return barrel->mPosition;
	}
}

void Enemy::setPosition(Vec2& position)
{
	for (Sprite* barrel : barrels) {
	  barrel->mPosition = position;
	}
}