#include "Game.h"

CGame::CGame(CSDL_Setup* kcsdl_setup, CResources* passedResources, CInput* passedKeyboard)
{
	window = NULL;
	window = kcsdl_setup;
	resources = NULL;
	resources = passedResources;
	keyboard = NULL;
	keyboard = passedKeyboard;

	// Init dev tools
	debug = new CDebug(window->GetRenderer());

	background = new CEntity(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	background->ADD_Sprite(new CSprite(resources->GetTexResources(1)));	

	for (int i = 0; i < TILE_COLUMN; i++)
	{
		for (int j = 0; j < TILE_ROW; j++)
		{
			tiles[i][j] = new CEntity(
				(TILE_COLUMN_CALC*i), 
				(TILE_ROW_CALC*j), 
				TILE_COLUMN_CALC, 
				TILE_ROW_CALC);
				
			tiles[i][j]->ADD_Sprite(new CSprite(resources->GetTexResources(2)));
		}
	}

	tiles[0][0]->SetDraw(false);

	//dave = new CEntity(0, 300, (int)(TILE_COLUMN_CALC*1.5), (int)(TILE_ROW_CALC*2));
	//paul = new CEntity(500, 300, (int)(TILE_COLUMN_CALC*1.5), (int)(TILE_ROW_CALC*2));

	player = new CPlayer(0, 150, 20, 20);
	bullet = new CBullet(100, 300, 20, 20);
	floor = new CEntity(0, 150, 20, 20);
	testFloor = new CEntity(0, 150, 20, 20);
	testBox = new CEntity(0, 150, 20, 20);

	CAnimate* playerAnim = new CAnimate(window->GetRenderer(), resources, 3);
	playerAnim->SetSpriteSheet(4,4);
	playerAnim->SetSpeed(128);

	player->ADD_Animation(playerAnim);
	bullet->ADD_Sprite(new CSprite(resources->GetTexResources(1)));
	floor->ADD_Sprite(new CSprite(resources->GetTexResources(1)));
	testFloor->ADD_Sprite(new CSprite(resources->GetTexResources(1)));
	testBox->ADD_Sprite(new CSprite(resources->GetTexResources(1)));

	world = new b2World(b2Vec2(0.0,-10));

	player->ADD_Physics(new CPhysics(world, 0, -20, 2, 2));
	floor->ADD_Physics(new CPhysics(world, 0, -25, 2, 2, false));
	bullet->ADD_Physics(new CPhysics(world, 10, -15, 2, 2));
	testFloor->ADD_Physics(new CPhysics(world, 20, -15, 2, 2, false));
	testBox->ADD_Physics(new CPhysics(world, 20, -10, 2, 2));

	player->GetPhysics()->SetFixedRot(true);
	player->GetPhysics()->GetBody()->SetLinearDamping(2);
	player->GetPhysics()->GetBody()->SetGravityScale(1);

	testFloor->GetPhysics()->EnableDebugTex(window->GetRenderer());
	testBox->GetPhysics()->EnableDebugTex(window->GetRenderer());
	player->GetPhysics()->EnableDebugTex(window->GetRenderer());

	bullet->GetPhysics()->GetBody()->SetBullet(true);

	SCORE = 0;

	score = new CEntity(SCREEN_WIDTH/2-40, 0, 80,45);
	score->ADD_Text(new CText());

	score->GetSprite()->Print(window->GetRenderer(), SCORE, 255, 255, 255);

	timeCurrent = 1;
	timePrevious = 1;
	inputTime = 0;

	idle = true;
	DEBUG = false;
	fired = false;
	
}

CGame::~CGame(void)
{	
	
	delete background;
	delete player;
	for (int i = 0; i < TILE_COLUMN; i++)
	{
		for (int j = 0; j < TILE_ROW; j++)
		{
			delete tiles[i][j];
		}
	}
	delete score;
	delete world;
	delete bullet;
	delete floor;
	delete debug;

	delete testFloor;
	delete testBox;

	// derefrence
	keyboard = NULL;
	window = NULL;
	resources = NULL;
}

void CGame::GameLoop()
{
	quit = false;
	while(window->GetMainEvent()->type != SDL_QUIT && !quit)
	{	
		timePrevious = timeCurrent;
		timeCurrent = SDL_GetTicks();		

		window->Begin();	

		keyboard->Poll(window->GetMainEvent());

		HandleEvents();

		DrawEntities();

		window->End();	
	}
}

void CGame::HandleEvents()
{
	if (inputTime+INPUT_DELAY < timeCurrent)
	{		
		if(keyboard->GetKey(0)->IsEnabled()) // walk up
		{
			player->MoveUp(100);
			player->GetAnimation()->PerformAnimation(3);
			idle = false;
		}

		if(keyboard->GetKey(1)->IsEnabled()) // walk down
		{
			player->MoveDown(100);
			player->GetAnimation()->PerformAnimation(0);
			idle = false;
		}

		if(keyboard->GetKey(2)->IsEnabled()) // walk left
		{
			player->MoveLeft(100);
			player->GetAnimation()->PerformAnimation(1);
			idle = false;
		}

		if(keyboard->GetKey(3)->IsEnabled()) // walk right
		{
			player->MoveRight(100);
			player->GetAnimation()->PerformAnimation(2);
			idle = false;
		}

		if(keyboard->GetKey(4)->IsEnabled()) // exit
		{
			quit = true;
		}

		if(keyboard->GetKey(5)->IsEnabled()) // player score
		{
			SCORE += 5;
			score->GetSprite()->Print(window->GetRenderer(), SCORE, 255, 255, 255);
		}

		if(keyboard->GetKey(6)->IsEnabled()) // dev tools on
		{
			DEBUG = true;		
			player->SetDebug(true);
			testFloor->SetDebug(true);
			testBox->SetDebug(true);
		}

		if(keyboard->GetKey(7)->IsEnabled()) // dev tools off
		{
			DEBUG = false;		
			player->SetDebug(false);
			testFloor->SetDebug(false);
			testBox->SetDebug(false);
		}

		if(keyboard->GetKey(8)->IsEnabled()) // jump
		{
			int numRays = 20;
			for (int i = 0; i < numRays; i++) 
			{
				  float angle = (i / (float)numRays) * 360 * (3.142/180);
				  b2Vec2 rayDir( sinf(angle), cosf(angle) );
				  b2Vec2 rayEnd = bullet->GetPhysics()->GetBody()->GetPosition();
				  rayEnd += 100 * rayDir;
				  //check what this ray hits
				  RaysCastCallback callback;//basic callback to record body and hit point
 				  world->RayCast(&callback, bullet->GetPhysics()->GetBody()->GetPosition(), rayEnd);
				  if ( callback.m_fixture ) 
				  {
					  CPhysics::ApplyBlastImpulse(callback.m_fixture->GetBody(), bullet->GetPhysics()->GetBody()->GetPosition(), callback.m_point, 5000);	  
				  }
			 }
		}

		if (keyboard->GetMouseClick())
		{
			if (!fired)
			{
				b2Vec2 mouse = b2Vec2(keyboard->GetMouseX(), -keyboard->GetMouseY());
				bullet->Fire(player->GetPhysics()->GetBody(), mouse);
				fired = true;
			}
		}

		if (idle)
		{
			player->GetAnimation()->UseFrame(1,0);
		} 

		if (DEBUG)
		{		
			RUNDebug();
		}

		HandlePhysics();

		inputTime = timeCurrent;
		idle = true;
	}
}

void CGame::HandlePhysics()
{	
	// Update world physics
	world->Step(1.0f/30.0f, 5, 5);

	// Update entity Positions

	// player
	player->UpdatePosition();
	// floor
	floor->UpdatePosition();
	// bullet
	bullet->UpdatePosition();
	// testfloor
	testFloor->UpdatePosition();
	// testbox
	testBox->UpdatePosition();
}

void CGame::DrawEntities()
{
	background->Draw(window->GetRenderer());	

	for (int i = 0; i < TILE_COLUMN; i++)
	{
		for (int j = 0; j < TILE_ROW; j++)
		{
			tiles[i][j]->Draw(window->GetRenderer());
		}
	}		

	player->Draw(window->GetRenderer());
	floor->Draw(window->GetRenderer());
	bullet->Draw(window->GetRenderer());
	score->Draw(window->GetRenderer());

	testFloor->Draw(window->GetRenderer());
	testBox->Draw(window->GetRenderer());

	if (DEBUG)
	{
		debug->Run(window->GetRenderer());
	}
}

string CGame::GetFrames()
{
	float num = timeCurrent - timePrevious;
	if (num < 10)
	{
		return "Frames: 100+";
	} else
	{
		return "Frames: ~"+to_string((long long)(1000 / num));
	}
}

void CGame::RUNDebug()
{
	debug->UpdateFrames(window->GetRenderer(), GetFrames());
}