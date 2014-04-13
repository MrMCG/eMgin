#include "Game.h"

CGame::CGame(CSDL_Setup* kcsdl_setup, CResources* passedResources, CInput* passedInput)
{
	window = NULL;
	window = kcsdl_setup;
	resources = NULL;
	resources = passedResources;
	input = NULL;
	input = passedInput;

	// Init dev tools
	debug = new CDebug(window->GetRenderer());

	

	for (int i = 0; i < TILE_COLUMN; i++)
	{
		for (int j = 0; j < TILE_ROW; j++)
		{
			tiles[i][j] = new CEntity(
				(TILE_COLUMN_CALC*i), 
				(TILE_ROW_CALC*j), 
				TILE_COLUMN_CALC, 
				TILE_ROW_CALC);
				
			tiles[i][j]->ADD_Sprite(new CSprite(resources->GetTexResources(6)));
		}
	}

	tiles[0][0]->SetDraw(false);

	world = new b2World(b2Vec2(0.0,-100));

	player = new CPlayer(window->GetRenderer(), resources, world);
	background = new CBackground(resources);
	bullet = new CBullet(resources);

	floor = new CEntity();
	testFloor = new CEntity();
	testBox = new CEntity();

	floor->ADD_Sprite(new CSprite(resources->GetTexResources(5)));
	testFloor->ADD_Sprite(new CSprite(resources->GetTexResources(5)));
	testBox->ADD_Sprite(new CSprite(resources->GetTexResources(4)));	
	
	floor->ADD_Physics(new CPhysics(world, 10, -20, 8, 4, false));
	testFloor->ADD_Physics(new CPhysics(world, 18, -20, 8, 4, false));
	testBox->ADD_Physics(new CPhysics(world, 20, -10, 2, 2));

	// create world border
	topSide = new CPhysics(world, 0, 2, 128, 2, false);
	rightSide = new CPhysics(world, 128, 0, 2, 72, false);
	leftSide = new CPhysics(world, -2, 0, 2, 72, false);
	bottomSide = new CPhysics(world, 0, -72, 128, 2, false);

	// randomly create boxes
	size = 4;
	int posX;
	int posY;
	int radius;
	boxes = new CEntity*[size];
	for (int k = 0; k < size; k ++)
	{
		radius = 4;//(int) (rand() % 5) + 2;
		posX = (int) (rand() % 123) + 1;
		posY = (int) (rand() % 30) + 1;
		boxes[k] = new CEntity(0, 150, radius*TILE_PIXEL_METER, radius*TILE_PIXEL_METER);
		boxes[k]->ADD_Sprite(new CSprite(resources->GetTexResources(4)));
		boxes[k]->ADD_Physics(new CPhysics(world, posX, -posY, radius, radius));
	}


	SCORE = 0;

	score = new CEntity(SCREEN_WIDTH/2-40, 0, 80,45);
	score->ADD_Text(new CText());

	score->GetSprite()->Print(window->GetRenderer(), SCORE, 255, 255, 255);

	timeCurrent = 1;
	timePrevious = 1;
	inputTime = 0;

	idle = true;
	DEBUG = false;	

	testListener = new CCollisionListener(player->GetPhysics()->GetBody(), testBox->GetPhysics()->GetBody());
	world->SetContactListener(testListener);
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

	delete leftSide;
	delete rightSide;
	delete topSide;
	delete bottomSide;

	for (int k = 0; k < size; k ++)
	{
		delete boxes[k];
	}

	delete boxes;

	// derefrence
	input = NULL;
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

		input->Poll(window->GetMainEvent());

		HandleEvents();

		DrawEntities();

		window->End();
	}
}

void CGame::HandleEvents()
{
	if (inputTime+INPUT_DELAY < timeCurrent)
	{		
		if(input->GetKey(0)->IsEnabled()) // walk up - w
		{
			player->MoveUp(2000);
			player->GetAnimation()->PerformAnimation(3);
			idle = false;
		}

		if(input->GetKey(1)->IsEnabled()) // walk down - s
		{
			player->MoveDown(2000);
			player->GetAnimation()->PerformAnimation(0);
			idle = false;
		}

		if(input->GetKey(2)->IsEnabled()) // walk left - a
		{	
			player->MoveLeft(2000);
			
			player->GetAnimation()->PerformAnimation(1);
			idle = false;
		}

		if(input->GetKey(3)->IsEnabled()) // walk right - d
		{
			player->MoveRight(2000);

			player->GetAnimation()->PerformAnimation(2);
			idle = false;
		}

		if(input->GetKey(4)->IsEnabled()) // exit - esc
		{
			window->GetMainEvent()->type = SDL_QUIT; 		
		}

		if(input->GetKey(5)->IsEnabled()) // reset - r
		{
			quit = true;
			/*
			SCORE += 5;
			score->GetSprite()->Print(window->GetRenderer(), SCORE, 255, 255, 255);
			*/
		}

		if(input->GetKey(6)->IsEnabled()) // dev tools on - '
		{
			DEBUG = true;		
			player->SetDebug(true);
			testFloor->SetDebug(true);
			testBox->SetDebug(true);

			for (int k = 0; k < size; k ++)
			{
				boxes[k]->SetDebug(true);
			}
		}

		if(input->GetKey(7)->IsEnabled()) // dev tools off - ;
		{
			DEBUG = false;		
			player->SetDebug(false);
			testFloor->SetDebug(false);
			testBox->SetDebug(false);

			for (int k = 0; k < size; k ++)
			{
				boxes[k]->SetDebug(false);
			}
		}

		if(input->GetKey(8)->IsEnabled()) // explode - space
		{
			
		}
		
		if (input->GetMouseClick()) // mouse click
		{
			if (!bullet->IsActive())
			{
				b2Vec2 mouse = b2Vec2(input->GetMouseX(), -input->GetMouseY());
				bullet->Fire(world, player, mouse);
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
	world->Step(1.0f/30.0f, 5, 3);

	// Update entity Positions

	// boxes
	for (int k = 0; k < size; k ++)
	{
		boxes[k]->UpdatePosition();
	}

	// player
	player->UpdatePosition();
	// floor
	floor->UpdatePosition();
	// bullet
	if (bullet->IsActive())
	{
		bullet->UpdatePosition();
	}
	// testfloor
	testFloor->UpdatePosition();
	// testbox
	testBox->UpdatePosition();

	if (bullet->CollidesWith(testBox))
	{
		if (bullet->IsActive())
		{
			bullet->Explode(world, 50, 10000);
		}
	}

	if (testListener->HasCollided())
	{
		cout << " collision " << endl;
	}
}

void CGame::DrawEntities()
{
	background->Draw(window->GetRenderer());	

	if (DEBUG)
	{
		for (int i = 0; i < TILE_COLUMN; i++)
		{
			for (int j = 0; j < TILE_ROW; j++)
			{
				tiles[i][j]->Draw(window->GetRenderer());
			}
		}		
	}

	for (int k = 0; k < size; k ++)
	{
		boxes[k]->Draw(window->GetRenderer());
	}

	
	floor->Draw(window->GetRenderer());
	if (bullet->IsActive())
	{
		bullet->Draw(window->GetRenderer());
	}
	score->Draw(window->GetRenderer());

	testFloor->Draw(window->GetRenderer());
	testBox->Draw(window->GetRenderer());
	player->Draw(window->GetRenderer());

	if (DEBUG)
	{
		debug->Run(window->GetRenderer());
	}
}

string CGame::GetFrames()
{
	float num = timeCurrent - timePrevious;
	if (num < 5)
	{
		return "Frames: 200+";
	} else
	{
		return "Frames: ~"+to_string((long long)(1000 / num));
	}
}

void CGame::RUNDebug()
{
	debug->UpdateFrames(window->GetRenderer(), GetFrames());
}