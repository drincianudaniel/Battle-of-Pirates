//-----------------------------------------------------------------------------
// File: CGameApp.cpp
//
// Desc: Game Application class, this is the central hub for all app processing
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CGameApp Specific Includes
//-----------------------------------------------------------------------------
#include "CGameApp.h"
#define TIMER_SEC 3
extern HINSTANCE g_hInst;

CGameApp::CGameApp()
{
	// Reset / Clear all required values
	m_hWnd			= NULL;
	m_hIcon			= NULL;
	m_hMenu			= NULL;
	m_pBBuffer		= NULL;
	m_pPlayer		= NULL;
	m_pPlayer2		= NULL;
	m_LastFrameRate = 0;
}

//-----------------------------------------------------------------------------
// Name : ~CGameApp () (Destructor)
// Desc : CGameApp Class Destructor
//-----------------------------------------------------------------------------
CGameApp::~CGameApp()
{
	// Shut the engine down
	ShutDown();
}

//-----------------------------------------------------------------------------
// Name : InitInstance ()
// Desc : Initialises the entire Engine here.
//-----------------------------------------------------------------------------
bool CGameApp::InitInstance( LPCTSTR lpCmdLine, int iCmdShow )
{
	// Create the primary display device
	if (!CreateDisplay()) { ShutDown(); return false; }

	// Build Objects
	if (!BuildObjects()) 
	{ 
		MessageBox( 0, _T("Failed to initialize properly. Reinstalling the application may solve this problem.\nIf the problem persists, please contact technical support."), _T("Fatal Error"), MB_OK | MB_ICONSTOP);
		ShutDown(); 
		return false; 
	}

	// Set up all required game states
	SetupGameState();

	// Success!
	return true;
}

//-----------------------------------------------------------------------------
// Name : CreateDisplay ()
// Desc : Create the display windows, devices etc, ready for rendering.
//-----------------------------------------------------------------------------
bool CGameApp::CreateDisplay()
{
	LPTSTR			WindowTitle		= _T("GameFramework");
	LPCSTR			WindowClass		= _T("GameFramework_Class");
	USHORT			Width			= 800;
	USHORT			Height			= 600;
	RECT			rc;
	WNDCLASSEX		wcex;


	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= CGameApp::StaticWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= g_hInst;
	wcex.hIcon			= LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_ICON));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= WindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON));

	if(RegisterClassEx(&wcex)==0)
		return false;

	// Retrieve the final client size of the window
	::GetClientRect( m_hWnd, &rc );
	m_nViewX		= rc.left;
	m_nViewY		= rc.top;
	m_nViewWidth	= rc.right - rc.left;
	m_nViewHeight	= rc.bottom - rc.top;

	m_hWnd = CreateWindow(WindowClass, WindowTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, Width, Height, NULL, NULL, g_hInst, this);

	if (!m_hWnd)
		return false;

	// Show the window
	ShowWindow(m_hWnd, SW_MAXIMIZE);
	// Success!!

	return true;
}


//-----------------------------------------------------------------------------
// Name : BeginGame ()
// Desc : Signals the beginning of the physical post-initialisation stage.
//		From here on, the game engine has control over processing.
//-----------------------------------------------------------------------------
int CGameApp::BeginGame()
{
	MSG		msg;

	// Start main loop
	while(true) 
	{
		// Did we recieve a message, or are we idling ?
		if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ) 
		{
			if (msg.message == WM_QUIT) break;
			TranslateMessage( &msg );
			DispatchMessage ( &msg );
		} 
		else 
		{
			// Advance Game Frame.
			FrameAdvance();

		} // End If messages waiting
	
	} // Until quit message is receieved

	return 0;
}

//-----------------------------------------------------------------------------
// Name : ShutDown ()
// Desc : Shuts down the game engine, and frees up all resources.
//-----------------------------------------------------------------------------
bool CGameApp::ShutDown()
{
	// Release any previously built objects
	ReleaseObjects ( );
	
	// Destroy menu, it may not be attached
	if ( m_hMenu ) DestroyMenu( m_hMenu );
	m_hMenu		 = NULL;

	// Destroy the render window
	SetMenu( m_hWnd, NULL );
	if ( m_hWnd ) DestroyWindow( m_hWnd );
	m_hWnd		  = NULL;
	
	// Shutdown Success
	return true;
}

//-----------------------------------------------------------------------------
// Name : StaticWndProc () (Static Callback)
// Desc : This is the main messge pump for ALL display devices, it captures
//		the appropriate messages, and routes them through to the application
//		class for which it was intended, therefore giving full class access.
// Note : It is VITALLY important that you should pass your 'this' pointer to
//		the lpParam parameter of the CreateWindow function if you wish to be
//		able to pass messages back to that app object.
//-----------------------------------------------------------------------------
LRESULT CALLBACK CGameApp::StaticWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	// If this is a create message, trap the 'this' pointer passed in and store it within the window.
	if ( Message == WM_CREATE ) SetWindowLong( hWnd, GWL_USERDATA, (LONG)((CREATESTRUCT FAR *)lParam)->lpCreateParams);

	// Obtain the correct destination for this message
	CGameApp *Destination = (CGameApp*)GetWindowLong( hWnd, GWL_USERDATA );
	
	// If the hWnd has a related class, pass it through
	if (Destination) return Destination->DisplayWndProc( hWnd, Message, wParam, lParam );
	
	// No destination found, defer to system...
	return DefWindowProc( hWnd, Message, wParam, lParam );
}

//-----------------------------------------------------------------------------
// Name : DisplayWndProc ()
// Desc : The display devices internal WndProc function. All messages being
//		passed to this function are relative to the window it owns.
//-----------------------------------------------------------------------------
LRESULT CGameApp::DisplayWndProc( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
	static UINT			fTimer;	

	// Determine message type
	switch (Message)
	{
		case WM_CREATE:
			SetTimer(hWnd, TIMER_SEC, 5000, NULL);
			break;
		
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		
		case WM_SIZE:
			if ( wParam == SIZE_MINIMIZED )
			{
				// App is inactive
				m_bActive = false;
			
			} // App has been minimized
			else
			{
				// App is active
				m_bActive = true;

				// Store new viewport sizes
				m_nViewWidth  = LOWORD( lParam );
				m_nViewHeight = HIWORD( lParam );
		
			
			} // End if !Minimized

			break;

		case WM_LBUTTONDOWN:
			// Capture the mouse
			SetCapture( m_hWnd );
			GetCursorPos( &m_OldCursorPos );
			break;

		case WM_LBUTTONUP:
			// Release the mouse
			ReleaseCapture( );
			break;

		case WM_KEYDOWN:
			switch(wParam)
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			case VK_RETURN:
				fTimer = SetTimer(m_hWnd, 1, 250, NULL);
				m_pPlayer->Explode();
				break;
			case VK_LEFT:
				m_pPlayer->RotateShipLeft();
				break;
			case VK_RIGHT:
				m_pPlayer->RotateShipRight();
				break;
			case VK_UP:
				m_pPlayer->RotateShipUp();
				break;
			case VK_DOWN:
				m_pPlayer->RotateShipDown();
				break;
			}
			break;

		case WM_CHAR:
		{
			switch (wParam)
			{
			case ',':
				m_pPlayer->ShootLeft();
				break;
			case '.':
				m_pPlayer->ShootRight();
				break;
			case 'q':
				m_pPlayer2->ShootLeft();
				break;
			case 'e':
				m_pPlayer2->ShootRight();
				break;
			case 'w':
				m_pPlayer2->RotateShipUp();
				break;
			case 'd':
				m_pPlayer2->RotateShipRight();
				break;
			case 's':
				m_pPlayer2->RotateShipDown();
				break;
			case 'a':
				m_pPlayer2->RotateShipLeft();
				break;
			case 'l':
				enemies->spawnMegalodon();
				break;
			case 'x':
				SaveGame();
				break;
			case 'c':
				LoadGame();
				break;
			}

		}


		case WM_TIMER:
			switch (wParam)
			{
			case 1:
				if (!m_pPlayer->AdvanceExplosion())
					KillTimer(m_hWnd, 1);
			case 2:
				if (!m_pPlayer2->AdvanceExplosion())
					KillTimer(m_hWnd, 1);
			case TIMER_SEC:
				enemies->spawnMegalodon();
			}
			break;

		case WM_COMMAND:
			break;

		default:
			return DefWindowProc(hWnd, Message, wParam, lParam);

	} // End Message Switch
	
	return 0;
}

//-----------------------------------------------------------------------------
// Name : BuildObjects ()
// Desc : Build our demonstration meshes, and the objects that instance them
//-----------------------------------------------------------------------------
bool CGameApp::BuildObjects()
{
	m_pBBuffer = new BackBuffer(m_hWnd, m_nViewWidth, m_nViewHeight);
	m_pPlayer = new CPlayer(m_pBBuffer);
	m_pPlayer2 = new CPlayer2(m_pBBuffer);
	LivesShip = new Lives(m_pBBuffer);
	enemies = new Enemy(m_pBBuffer);
	if(!m_imgBackground.LoadBitmapFromFile("data/Background2.bmp", GetDC(m_hWnd)))
		return false;

	// Success!
	return true;
}

//-----------------------------------------------------------------------------
// Name : SetupGameState ()
// Desc : Sets up all the initial states required by the game.
//-----------------------------------------------------------------------------
void CGameApp::SetupGameState()
{
	m_pPlayer->Position() = Vec2(452, 900);
	m_pPlayer2->Position() = Vec2(1450, 900);
	
}

//-----------------------------------------------------------------------------
// Name : ReleaseObjects ()
// Desc : Releases our objects and their associated memory so that we can
//		rebuild them, if required, during our applications life-time.
//-----------------------------------------------------------------------------
void CGameApp::ReleaseObjects( )
{
	if(m_pPlayer != NULL)
	{
		delete m_pPlayer;
		m_pPlayer = NULL;
	}

	if (m_pPlayer2 != NULL)
	{
		delete m_pPlayer2;
		m_pPlayer2 = NULL;
	}

	if(m_pBBuffer != NULL)
	{
		delete m_pBBuffer;
		m_pBBuffer = NULL;
	}

	if (LivesShip != NULL)
	{
		delete LivesShip;
		LivesShip = NULL;
	}

	if (enemies != NULL)
	{
		delete enemies;
		enemies = NULL;
	}

}

//-----------------------------------------------------------------------------
// Name : FrameAdvance () (Private)
// Desc : Called to signal that we are now rendering the next frame.
//-----------------------------------------------------------------------------
void CGameApp::FrameAdvance()
{
	static TCHAR FrameRate[ 50 ];
	static TCHAR TitleBuffer[ 255 ];

	// Advance the timer
	m_Timer.Tick( );

	// Skip if app is inactive
	if ( !m_bActive ) return;
	
	// Get / Display the framerate
	if ( m_LastFrameRate != m_Timer.GetFrameRate() )
	{
		m_LastFrameRate = m_Timer.GetFrameRate( FrameRate, 50 );
		sprintf_s( TitleBuffer, _T("Game : %s"), FrameRate );
		SetWindowText( m_hWnd, TitleBuffer );

	} // End if Frame Rate Altered

	// Poll & Process input devices
	ProcessInput();

	// Animate the game objects
	AnimateObjects();

	// Drawing the game objects
	DrawObjects();

	//Collision
	Collision();
	CollisionBullets();
	CollisionMegalodonShip();
	CollisionBulletsMegalodon();
	CollisionBarrelShip();
	//Drawing on screen
	
}

//-----------------------------------------------------------------------------
// Name : ProcessInput () (Private)
// Desc : Simply polls the input devices and performs basic input operations
//-----------------------------------------------------------------------------
void CGameApp::ProcessInput( )
{
	static UCHAR pKeyBuffer[ 256 ];
	ULONG		Direction = 0;
	ULONG		Direction2 = 0;
	POINT		CursorPos;
	float		X = 0.0f, Y = 0.0f;

	// Retrieve keyboard state
	if ( !GetKeyboardState( pKeyBuffer ) ) return;

	// Check the relevant keys
	if ( pKeyBuffer[ VK_UP	] & 0xF0 ) Direction |= CPlayer::DIR_FORWARD;
	if ( pKeyBuffer[ VK_DOWN  ] & 0xF0 ) Direction |= CPlayer::DIR_BACKWARD;
	if ( pKeyBuffer[ VK_LEFT  ] & 0xF0 ) Direction |= CPlayer::DIR_LEFT;
	if ( pKeyBuffer[ VK_RIGHT ] & 0xF0 ) Direction |= CPlayer::DIR_RIGHT;


	// Move the player
	m_pPlayer->Move(Direction);


	if (pKeyBuffer[0x57] & 0xF0) Direction2 |= CPlayer2::DIR_FORWARD;
	if (pKeyBuffer[0x53] & 0xF0) Direction2 |= CPlayer2::DIR_BACKWARD;
	if (pKeyBuffer[0x41] & 0xF0) Direction2 |= CPlayer2::DIR_LEFT;
	if (pKeyBuffer[0x44] & 0xF0) Direction2 |= CPlayer2::DIR_RIGHT;
	m_pPlayer2->Move(Direction2);



	// Now process the mouse (if the button is pressed)
	if ( GetCapture() == m_hWnd )
	{
		// Hide the mouse pointer
		SetCursor( NULL );

		// Retrieve the cursor position
		GetCursorPos( &CursorPos );

		// Reset our cursor position so we can keep going forever :)
		SetCursorPos( m_OldCursorPos.x, m_OldCursorPos.y );

	} // End if Captured
}

//-----------------------------------------------------------------------------
// Name : AnimateObjects () (Private)
// Desc : Animates the objects we currently have loaded.
//-----------------------------------------------------------------------------
void CGameApp::AnimateObjects()
{
	m_pPlayer->Update(m_Timer.GetTimeElapsed());
	m_pPlayer2->Update(m_Timer.GetTimeElapsed());
	LivesShip->Update(m_Timer.GetTimeElapsed());
	enemies->updateMegalodon(m_Timer.GetTimeElapsed());
	enemies->updatePorts(m_Timer.GetTimeElapsed());
	enemies->updateBarrel(m_Timer.GetTimeElapsed());

}

//-----------------------------------------------------------------------------	
// Name : DrawObjects () (Private)
// Desc : Draws the game objects
//-----------------------------------------------------------------------------
void CGameApp::DrawObjects()
{
	m_pBBuffer->reset();

	m_imgBackground.Paint(m_pBBuffer->getDC(), 0, 0);

	m_pPlayer->Draw();

	m_pPlayer2->Draw();

	LivesShip->Draw();

	enemies->drawMegalodon();

	enemies->drawBarrel();

	drawHullIntegrity();

	drawScore();

	m_pBBuffer->present();
}


void CGameApp::Collision()
{
	static UINT fTimer;
	double distance = m_pPlayer->Position().Distance(m_pPlayer2->Position());
	if (distance <= m_pPlayer->getShipWidth()) 
	{
		m_pPlayer->Velocity() = Vec2(0, 0);
		m_pPlayer2->Velocity() = Vec2(0, 0);
	}

}

void CGameApp::CollisionBullets()
{
	int p1 = 0;
	int p2 = 0;
	std::vector <Sprite*> bulletsP2 = m_pPlayer2->getVectorBullets();
	for (Sprite* bullet : bulletsP2)
	{
		double distance = bullet->mPosition.Distance(m_pPlayer->Position());
		if (distance <= (bullet->width() + m_pPlayer->getShipWidth()) /2)
		{
			bulletsP2.erase(bulletsP2.begin() + p1);
			m_pPlayer2->updatebullets(bulletsP2);
			HullIntegrityP1 = HullIntegrityP1 - 10;
			Player1Death();
			p1--;
		}
		p1++;
	}

	std::vector <Sprite*> bulletsP1 = m_pPlayer->getVectorBullets();
	for (Sprite* bullet : bulletsP1)
	{
		
		double distance = bullet->mPosition.Distance(m_pPlayer2->Position());
		if (distance <= (bullet->width() + m_pPlayer2->getShipWidth()) / 2)
		{
			bulletsP1.erase(bulletsP1.begin() + p2);
			m_pPlayer->updatebullets(bulletsP1);
			HullIntegrityP2 = HullIntegrityP2 - 10;
			Player2Death();
			p2--;
		}
		p2++;
	}

}

void CGameApp::CollisionMegalodonShip()
{
	std::vector <Sprite*> megalodon = enemies->getMegalodonVector();
	int p1 = 0;
	int p2 = 0;
	for (Sprite* meg : megalodon)
	{
		double distance = meg->mPosition.Distance(m_pPlayer->Position());
		if (distance <= (meg->width() + m_pPlayer->getShipWidth())/ 2)
		{
			megalodon.erase(megalodon.begin() + p1);
			enemies->updateMegalodonVector(megalodon);
			HullIntegrityP1 -= 50;
			Player1Death();
		}
		p1++;

	}

	for (Sprite* meg : megalodon)
	{
		double distance = meg->mPosition.Distance(m_pPlayer2->Position());
		if (distance <= (meg->width() + m_pPlayer2->getShipWidth()) / 2)
		{
			megalodon.erase(megalodon.begin() + p2);
			enemies->updateMegalodonVector(megalodon);
			HullIntegrityP2 -= 50;
			Player2Death();
		}
		p2++;

	}
}

void CGameApp::CollisionBarrelShip()
{
	std::vector <Sprite*> barrel = enemies->getBarrelVector();
	int p1 = 0;
	int p2 = 0;
	for (Sprite* meg : barrel)
	{
		double distance = meg->mPosition.Distance(m_pPlayer->Position());
		if (distance <= (meg->width() + m_pPlayer->getShipWidth()) / 2)
		{
			barrel.erase(barrel.begin() + p1);
			enemies->updateBarrelVector(barrel);
			HullIntegrityP1 += 30;
		}
		p1++;

	}

	for (Sprite* meg : barrel)
	{
		double distance = meg->mPosition.Distance(m_pPlayer2->Position());
		if (distance <= (meg->width() + m_pPlayer2->getShipWidth()) / 2)
		{
			barrel.erase(barrel.begin() + p2);
			enemies->updateBarrelVector(barrel);
			HullIntegrityP2 += 30;

		}
		p2++;

	}
}
void CGameApp::CollisionBulletsMegalodon()
{

	std::vector<Sprite*> Enemies = enemies->getMegalodonVector();
	std::vector <Sprite*> bulletsP1 = m_pPlayer->getVectorBullets();
	int p1 = 0;
	int p2 = 0;
	int p3 = 0;
	int p4 = 0;
	for (Sprite* bullet : bulletsP1)
	{
		p1 = 0;
		for (Sprite* enemy : Enemies)
		{
			p2 = 0;
			double distance = bullet->mPosition.Distance(enemy->mPosition);
			if (distance <= (bullet->width() + enemy->width()) / 2)
			{
				Enemies.erase(Enemies.begin() + p1);
				enemies ->updateMegalodonVector(Enemies);
				bulletsP1.erase(bulletsP1.begin() + p2);
				m_pPlayer->updatebullets(bulletsP1);
				ScoreP1++;
			}
			p1++;
		}
		p2++;
	}

	std::vector <Sprite*> bulletsP2 = m_pPlayer2->getVectorBullets();
	for (Sprite* bullet : bulletsP2)
	{
		p3 = 0;
		for (Sprite* enemy : Enemies)
		{
			p4 = 0;
			double distance = bullet->mPosition.Distance(enemy->mPosition);
			if (distance <= (bullet->width() + enemy->width()) / 2)
			{
				Enemies.erase(Enemies.begin() + p3);
				enemies->updateMegalodonVector(Enemies);
				bulletsP2.erase(bulletsP2.begin() + p4);
				m_pPlayer2->updatebullets(bulletsP2);
				ScoreP2++;
			}
			p3++;
			
		}
		p4++;
	}
}


void CGameApp::drawHullIntegrity()
{
	HFONT hFont, hTmp;
	HDC backbufferHDC = m_pBBuffer->getDC();
	char HullIntegrityP1q[32];
	char HullIntegrityP2q[32];
	SetDCPenColor(backbufferHDC, RGB(255, 8, 220));
	SetBkMode(backbufferHDC, TRANSPARENT);
	sprintf(HullIntegrityP1q, "P1 Hull Integrity:\n %d", HullIntegrityP1);
	sprintf(HullIntegrityP2q, "P2 Hull Integrity:\n %d", HullIntegrityP2);
	hFont = CreateFont(48, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Impact"));
	hTmp = (HFONT)SelectObject(backbufferHDC, hFont);
	SetTextColor(backbufferHDC, RGB(0, 0, 0));
	TextOut(backbufferHDC, 50, 20, HullIntegrityP1q, (int)strlen(HullIntegrityP1q));
	TextOut(backbufferHDC, 1500, 20, HullIntegrityP2q, (int)strlen(HullIntegrityP2q));


}

void CGameApp::drawScore()
{
	HFONT hFont, hTmp;
	HDC backbufferHDC = m_pBBuffer->getDC();
	char Scoreplayer1[32];
	char Scoreplayer2[32];
	SetDCPenColor(backbufferHDC, RGB(255, 8, 220));
	SetBkMode(backbufferHDC, TRANSPARENT);
	sprintf(Scoreplayer1, "\n %d         -", ScoreP1);
	sprintf(Scoreplayer2, "\n %d", ScoreP2);
	hFont = CreateFont(48, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Impact"));
	hTmp = (HFONT)SelectObject(backbufferHDC, hFont);
	SetTextColor(backbufferHDC, RGB(0, 0, 0));
	TextOut(backbufferHDC, 850, 40, Scoreplayer1, (int)strlen(Scoreplayer1));
	TextOut(backbufferHDC, 1010, 40, Scoreplayer2, (int)strlen(Scoreplayer2));
}

void CGameApp::TakeLifePlayer1() {
	std::vector <Sprite*> hearts = LivesShip->getVectorHearts();
	static HINSTANCE hInstance;
	for (Sprite* heart : hearts)
	{
		hearts.pop_back();
		LivesShip->updateLives(hearts);
		break;
	}
}


void CGameApp::TakeLifePlayer2() {
	std::vector <Sprite*> hearts = LivesShip->getVectorHearts2();
	static HINSTANCE hInstance;
	for (Sprite* heart : hearts)
	{
		hearts.pop_back();
		LivesShip->updateLives2(hearts);
		break;
	}
}

void CGameApp::Player1Death()
{
	if (HullIntegrityP1 <= 0)
	{
		TakeLifePlayer1();
		HullIntegrityP1 = 100;
		enemies->spawnBarrel();
		enemies->setPosition(m_pPlayer->Position());
		m_pPlayer->Position() = Vec2(452, 900);
		m_pPlayer->Velocity() = Vec2(0, 0);
		m_pPlayer->RotateShipUp();
		
	}
}

void CGameApp::Player2Death()
{
	if (HullIntegrityP2 <= 0)
	{
		TakeLifePlayer2();
		HullIntegrityP2 = 100;
		enemies->spawnBarrel();
		enemies->setPosition(m_pPlayer2->Position());
		m_pPlayer2->Position() = Vec2(1450, 900);
		m_pPlayer2->Velocity() = Vec2(0, 0);
		m_pPlayer->RotateShipDown();

	}
}

void CGameApp::SaveGame()
{
	Save Data;
	Data.player1pos = m_pPlayer->Position();
	Data.player1rot = m_pPlayer->getposP1();
	Data.score1 = ScoreP1;
	Data.hullintegrity1 = HullIntegrityP1;
	Data.heartsP1 = LivesShip->NoLivesP1();

	//Data.heartsP1 = LivesHearts->NoLivesP1();
	/*Data.heartsP1 = LivesHearts->getVectorHearts();
	Data.heartsP2 = LivesHearts->getVectorHearts2();
	size_t sz = Data.heartsP1.size();*/

	Data.player2pos = m_pPlayer2->Position();
	Data.player2rot = m_pPlayer2->getposP2();
	Data.score2 = ScoreP2;
	Data.hullintegrity2 = HullIntegrityP2;
	Data.heartsP2 = LivesShip->NoLivesP2();

	std::ofstream input("data/save.txt");
	//input.open("data / save.txt");
	input.write((char*)&Data, sizeof(Data));

}


// load game
void CGameApp::LoadGame()
{
	Save Data;
	std::ifstream input("data/save.txt");
	input.read((char*)&Data, sizeof(Data));
	m_pPlayer->Position() = Data.player1pos;
	ScoreP1 = Data.score1;
	HullIntegrityP1 = Data.hullintegrity1;
	LivesShip->SetNumberLivesP1(Data.heartsP1);

	

	switch (Data.player1rot) {
	case 1:
		m_pPlayer->RotateShipUp();
		break;
	case 2:
		m_pPlayer->RotateShipLeft();
		break;
	case 3:
		m_pPlayer->RotateShipRight();
		break;
	case 4:
		m_pPlayer->RotateShipDown();
		break;
	}

	m_pPlayer2->Position() = Data.player2pos;
	ScoreP2 = Data.score2;
	HullIntegrityP2 = Data.hullintegrity2;
	LivesShip->SetNumberLivesP2(Data.heartsP2);

	switch (Data.player2rot) {
	case 1:
		m_pPlayer2->RotateShipUp();
		break;
	case 2:
		m_pPlayer2->RotateShipLeft();
		break;
	case 3:
		m_pPlayer2->RotateShipRight();
		break;
	case 4:
		m_pPlayer2->RotateShipDown();
		break;
	}

	//LivesHearts->SetNumberLivesP1(Data.heartsP1);
	m_pPlayer->Velocity() = Vec2(0, 0);
	m_pPlayer2->Velocity() = Vec2(0, 0);


}