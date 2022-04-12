#include "Lives.h"


Lives::Lives(const BackBuffer* pBackBuffer)
{
	this->pBackBuffer = pBackBuffer;

	AddLifeP1();
	AddLifeP1();
	AddLifeP1();


	AddLifeP2();
	AddLifeP2();
	AddLifeP2();
	srand(time(NULL));

}

Lives::~Lives()
{
	for (Sprite* heart : hearts) {
		delete(heart);
	}

	for (Sprite* heart : hearts2) {
		delete(heart);
	}
}

void Lives::Update(float dt)
{

	// Update sprite
	for (Sprite* heart : hearts) {
		heart->update(dt);
	}

	for (Sprite* heart : hearts2) {
		heart->update(dt);
	}
}

void Lives::Draw()
{
	for (Sprite* heart : hearts)
	{
		heart->setBackBuffer(pBackBuffer);
		heart->draw();
	}
	for (Sprite* heart : hearts2)
	{
		heart->setBackBuffer(pBackBuffer);
		heart->draw();
	}
}


std::vector<Sprite*> Lives::getVectorHearts()
{
	return hearts;
}


std::vector<Sprite*> Lives::getVectorHearts2()
{
	return hearts2;
}

void Lives::updateLives2(std::vector <Sprite*> hearts)
{
	this->hearts2 = hearts;
}
void Lives::updateLives(std::vector <Sprite*> hearts)
{
	this->hearts = hearts;
}


void Lives::AddLifeP1()
{
	if (hearts.size() == 0)
	{
		hearts.push_back(new Sprite("data/ShipLives.bmp", RGB(255, 255, 255)));
		hearts.at(0)->mPosition.x = 100;
		hearts.at(0)->mPosition.y = 110;
	}
	else
	{
		hearts.push_back(new Sprite("data/ShipLives.bmp", RGB(255, 255, 255)));
		hearts.at(hearts.size() - 1)->mPosition.x = hearts.at(hearts.size() - 2)->mPosition.x + 100;
		hearts.at(hearts.size() - 1)->mPosition.y = 110;
	}
}

void Lives::AddLifeP2()
{
	if (hearts2.size() == 0)
	{
		hearts2.push_back(new Sprite("data/ShipLives2.bmp", RGB(255, 255, 255)));
		hearts2.at(0)->mPosition.x = 1550;
		hearts2.at(0)->mPosition.y = 110;
	}
	else
	{
		hearts2.push_back(new Sprite("data/ShipLives2.bmp", RGB(255, 255, 255)));
		hearts2.at(hearts2.size() - 1)->mPosition.x = hearts2.at(hearts2.size() - 2)->mPosition.x + 100;
		hearts2.at(hearts2.size() - 1)->mPosition.y = 110;
	}
}

int Lives::NoLivesP1()
{
	return hearts.size();
}
int Lives::NoLivesP2()
{
	return hearts2.size();
}

void Lives::SetNumberLivesP1(int NOhearts)
{
	while (NoLivesP1() != NOhearts)
	{
		if (NoLivesP1() > NOhearts)
		{
			hearts.pop_back();
		}
		else
		{
			AddLifeP1();
		}
	}
}

void Lives::SetNumberLivesP2(int NOhearts)
{
	while (NoLivesP2() != NOhearts)
	{
		if (NoLivesP2() > NOhearts)
		{
			hearts.pop_back();
		}
		else
		{
			AddLifeP2();
		}
	}
}

void Lives::SpawnHearts()
{
	RECT desktop;
	GetWindowRect(GetDesktopWindow(), &desktop);
	hearts2.push_back(new Sprite("data/ShipLives.bmp", RGB(255, 255, 255)));
	hearts2.back()->mPosition.x = rand() % ((desktop.right - 100) + (desktop.left + 100));
	hearts2.back()->mPosition.y = rand() % 200 + 700;
}