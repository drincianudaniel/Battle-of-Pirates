#include "CPlayer2.h"
#include "..\Includes\CPlayer.h"

CPlayer2::CPlayer2(const BackBuffer* pBackBuffer)
{
	this->pBackBuffer = pBackBuffer;
	//m_pSprite = new Sprite("data/planeimg.bmp", "data/planemask.bmp");
	m_pSprite = new Sprite("data/Ship2up.bmp", RGB(255, 255, 255));
	m_pSprite->setBackBuffer(pBackBuffer);
	RotateShipUp();
	m_eSpeedState = SPEED_STOP;
	m_fTimer = 0;


	// Animation frame crop rectangle
	RECT r;
	r.left = 0;
	r.top = 0;
	r.right = 128;
	r.bottom = 128;

	m_pExplosionSprite = new AnimatedSprite("data/explosion.bmp", "data/explosionmask.bmp", r, 16);
	m_pExplosionSprite->setBackBuffer(pBackBuffer);
	m_bExplosion = false;
	m_iExplosionFrame = 0;

}

CPlayer2::~CPlayer2()
{
	delete m_pSprite;
	delete m_pExplosionSprite;
}

void CPlayer2::Update(float dt)
{
	// Update sprite
	m_pSprite->update(dt);

	int i = 0;
	RECT desktop;
	GetWindowRect(GetDesktopWindow(), &desktop);
	for (Sprite* bullet : bullets) {
		bullet->update(dt);
		i = 0;
		if (bullet->mPosition.y < desktop.top || bullet->mPosition.y > desktop.bottom || bullet->mPosition.x > desktop.right || bullet->mPosition.x < desktop.left) // || m_bExplosion)
		{
			bullets.erase(bullets.begin() + i);
		}
		i++;
	}

	// Get velocity
	double v = m_pSprite->mVelocity.Magnitude();
	m_fTimer += dt;

	switch (m_eSpeedState)
	{
	case SPEED_STOP:
		if (v > 35.0f)
		{
			m_eSpeedState = SPEED_START;
			PlaySound("data/jet-start.wav", NULL, SND_FILENAME | SND_ASYNC);
			m_fTimer = 0;
		}
		break;
	case SPEED_START:
		if (v < 25.0f)
		{
			m_eSpeedState = SPEED_STOP;
			PlaySound("data/jet-stop.wav", NULL, SND_FILENAME | SND_ASYNC);
			m_fTimer = 0;
		}
		else
			if (m_fTimer > 1.f)
			{
				PlaySound("data/jet-cabin.wav", NULL, SND_FILENAME | SND_ASYNC);
				m_fTimer = 0;
			}
		break;
	}

}

void CPlayer2::Draw()
{
	for (Sprite* bullet : bullets)
	{
		bullet->setBackBuffer(pBackBuffer);
		bullet->draw();
	}

	if (m_bullet && !m_bExplosion)
	{
	}

	if (!m_bExplosion)
		m_pSprite->draw();
	else
		m_pExplosionSprite->draw();

}

void CPlayer2::Move(ULONG ulDirection)
{
	RECT desktop;
	GetWindowRect(GetDesktopWindow(), &desktop);
	if (ulDirection & CPlayer2::DIR_LEFT)
	{
		m_pSprite->mVelocity.x -= 1;
		if (m_pSprite->mVelocity.x < -100)
			m_pSprite->mVelocity.x = -100;
	}
	if (m_pSprite->mPosition.x < desktop.left + m_pSprite->width() / 2)
		m_pSprite->mVelocity.x = 10;


	if (ulDirection & CPlayer2::DIR_RIGHT)
	{
		m_pSprite->mVelocity.x += 1;
		if (m_pSprite->mVelocity.x > 100)
			m_pSprite->mVelocity.x = 100;
	}
	if (m_pSprite->mPosition.x > desktop.right - m_pSprite->width() / 2)
		m_pSprite->mVelocity.x = -10;


	if (ulDirection & CPlayer2::DIR_FORWARD)
	{
		m_pSprite->mVelocity.y -= 1;
		if (m_pSprite->mVelocity.y < -100)
			m_pSprite->mVelocity.y = -100;
	}
	if (m_pSprite->mPosition.y < desktop.top + m_pSprite->height() / 2)
		m_pSprite->mVelocity.y = 10;


	if (ulDirection & CPlayer2::DIR_BACKWARD)
	{
		m_pSprite->mVelocity.y += 1;
		if (m_pSprite->mVelocity.y > 100)
			m_pSprite->mVelocity.y = 100;
	}
	if (m_pSprite->mPosition.y > desktop.bottom - m_pSprite->height() / 2 - 50)
		m_pSprite->mVelocity.y = -10;

	if (m_pSprite->mPosition.x < 382 && m_pSprite->mPosition.y > 722)
	{
		m_pSprite->mVelocity.y = -10;
		m_pSprite->mVelocity.x = 10;
	}

	if (m_pSprite->mPosition.x > 1529 && m_pSprite->mPosition.y > 722)
	{
		m_pSprite->mVelocity.y = -10;
		m_pSprite->mVelocity.x = -10;
	}
}


Vec2& CPlayer2::Position()
{
	return m_pSprite->mPosition;

}

Vec2& CPlayer2::Velocity()
{
	return m_pSprite->mVelocity;

}

void CPlayer2::Explode()
{
	m_pExplosionSprite->mPosition = m_pSprite->mPosition;
	m_pExplosionSprite->SetFrame(0);
	PlaySound("data/explosion.wav", NULL, SND_FILENAME | SND_ASYNC);
	m_bExplosion = true;
}

bool CPlayer2::AdvanceExplosion()
{
	if (m_bExplosion)
	{
		m_pExplosionSprite->SetFrame(m_iExplosionFrame++);
		if (m_iExplosionFrame == m_pExplosionSprite->GetFrameCount())
		{
			m_bExplosion = false;
			m_iExplosionFrame = 0;
			m_pSprite->mVelocity = Vec2(0, 0);
			m_eSpeedState = SPEED_STOP;
			return false;
		}
	}

	return true;
}

void CPlayer2::ShootRight()
{
	if (bullets.empty()) {

		switch (pos)
		{
		case 1:
			bullets.push_back(new Sprite("data/bullet.bmp", RGB(0xff, 0x00, 0xff)));
			bullets.back()->mPosition.y = m_pSprite->mPosition.y - 60;
			bullets.back()->mPosition.x = m_pSprite->mPosition.x;
			bullets.back()->mVelocity.x = 1000;

			bullets.push_back(new Sprite("data/bullet.bmp", RGB(0xff, 0x00, 0xff)));
			bullets.back()->mPosition.y = m_pSprite->mPosition.y;
			bullets.back()->mPosition.x = m_pSprite->mPosition.x;
			bullets.back()->mVelocity.x = 1000;

			bullets.push_back(new Sprite("data/bullet.bmp", RGB(0xff, 0x00, 0xff)));
			bullets.back()->mPosition.y = m_pSprite->mPosition.y + 60;
			bullets.back()->mPosition.x = m_pSprite->mPosition.x;
			bullets.back()->mVelocity.x = 1000;
			break;
		case 2:
			bullets.push_back(new Sprite("data/bullet.bmp", RGB(0xff, 0x00, 0xff)));
			bullets.back()->mPosition.y = m_pSprite->mPosition.y;
			bullets.back()->mPosition.x = m_pSprite->mPosition.x - 60;
			bullets.back()->mVelocity.y = 1000;

			bullets.push_back(new Sprite("data/bullet.bmp", RGB(0xff, 0x00, 0xff)));
			bullets.back()->mPosition.y = m_pSprite->mPosition.y;
			bullets.back()->mPosition.x = m_pSprite->mPosition.x;
			bullets.back()->mVelocity.y = 1000;

			bullets.push_back(new Sprite("data/bullet.bmp", RGB(0xff, 0x00, 0xff)));
			bullets.back()->mPosition.y = m_pSprite->mPosition.y;
			bullets.back()->mPosition.x = m_pSprite->mPosition.x+ 60;
			bullets.back()->mVelocity.y = 1000;
			break;
		}
	}
}

void CPlayer2::ShootLeft()
{
	if (bullets.empty()) {

		switch (pos)
		{
		case 1:
			bullets.push_back(new Sprite("data/bullet.bmp", RGB(0xff, 0x00, 0xff)));
			bullets.back()->mPosition.y = m_pSprite->mPosition.y - 60;
			bullets.back()->mPosition.x = m_pSprite->mPosition.x - 10;
			bullets.back()->mVelocity.x = -1000;

			bullets.push_back(new Sprite("data/bullet.bmp", RGB(0xff, 0x00, 0xff)));
			bullets.back()->mPosition.y = m_pSprite->mPosition.y;
			bullets.back()->mPosition.x = m_pSprite->mPosition.x;
			bullets.back()->mVelocity.x = -1000;

			bullets.push_back(new Sprite("data/bullet.bmp", RGB(0xff, 0x00, 0xff)));
			bullets.back()->mPosition.y = m_pSprite->mPosition.y + 60;
			bullets.back()->mPosition.x = m_pSprite->mPosition.x + 10;
			bullets.back()->mVelocity.x = -1000;
			break;
		case 2:
			bullets.push_back(new Sprite("data/bullet.bmp", RGB(0xff, 0x00, 0xff)));
			bullets.back()->mPosition.y = m_pSprite->mPosition.y + 10;
			bullets.back()->mPosition.x = m_pSprite->mPosition.x - 60;
			bullets.back()->mVelocity.y = -1000;

			bullets.push_back(new Sprite("data/bullet.bmp", RGB(0xff, 0x00, 0xff)));
			bullets.back()->mPosition.y = m_pSprite->mPosition.y;
			bullets.back()->mPosition.x = m_pSprite->mPosition.x;
			bullets.back()->mVelocity.y = -1000;

			bullets.push_back(new Sprite("data/bullet.bmp", RGB(0xff, 0x00, 0xff)));
			bullets.back()->mPosition.y = m_pSprite->mPosition.y - 10;
			bullets.back()->mPosition.x = m_pSprite->mPosition.x + 60;
			bullets.back()->mVelocity.y = -1000;
			break;
		}
	}
}

double CPlayer2::getShipWidth()
{
	return m_pSprite->width();
}


double CPlayer2::getShipHeight()
{
	return m_pSprite->height();
}

void CPlayer2::RotateShipLeft() {
	m_pSprite->setSprite("data/Ship2left.bmp", RGB(255, 255, 255));
	pos = 2;
}
void CPlayer2::RotateShipRight() {
	m_pSprite->setSprite("data/Ship2right.bmp", RGB(255, 255, 255));
	pos = 2;
}
void CPlayer2::RotateShipUp() {
	m_pSprite->setSprite("data/Ship2up.bmp", RGB(255, 255, 255));
	pos = 1;
}
void CPlayer2::RotateShipDown() {
	m_pSprite->setSprite("data/Ship2down.bmp", RGB(255, 255, 255));
	pos = 1;
}

void CPlayer2::updatebullets(std::vector <Sprite*> bulletsP1)
{
	this->bullets = bulletsP1;
}

std::vector<Sprite*> CPlayer2::getVectorBullets()
{
	return bullets;
}

int CPlayer2::getposP2()
{
	return pos;
}