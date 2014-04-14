#include "Game.h"

CGame::CGame(CSDL_Setup* csdl_setup, CResources* passedResources, CInput* passedInput)
{
	window = NULL;
	window = csdl_setup;
	resources = NULL;
	resources = passedResources;
	input = NULL;
	input = passedInput;

	
	

	ifstream file(LEVEL_LOCATION+"map.txt");
	size_t pos = 0;
	string line;
	string current;
	while (getline(file, line))
	{
		if (line.substr(0,1) == "[")
		{
			string area = line.substr(1, line.length()-2);
			if (area == "background")
			{
				cout << "BG" << endl;
			} else if (area == "test")
			{
				cout << "tt" << endl;
			} else
			{
				cout << "could not read" << endl;
			}
		} else
		{
			
			while((pos = line.find(",")) != string::npos)
			{
				current = line.substr(0, pos);
				cout << current << endl;
				line.erase(0, pos+1);
			}
		}
	}

	// Init dev tools
	debug = new CDebug(window->GetRenderer());
	
	// create world
	world = new b2World(b2Vec2(0.0,-100));

	// create world border
	screenBoundry = new CBoundry(resources, world);

	// create background
	background = new CBackground(resources);

	// create tiles
	for (int i = 0; i < TILE_COLUMN; i++)
	{
		for (int j = 0; j < TILE_ROW; j++)
		{
			tiles[i][j] = new CTile(resources, TILE_COLUMN_CALC*i, TILE_ROW_CALC*j);
		}
	}
	tiles[0][0]->ADD_Sprite(new CSprite(resources->GetTexResources(5)));

	// create player
	player = new CPlayer(window->GetRenderer(), resources, world);
	
	// create re-usable bullet
	bullet = new CBullet(resources);

	// create crates
	testBox = new CCrate(resources, world, 20, -10, 2, 2);

	// create score
	SCORE = 0;
	score = new CWriting(SCREEN_WIDTH/2-40, 0, 80,45);
	score->SetColor(255,255,255);
	score->Print(window->GetRenderer(), SCORE);



	floor = new CEntity();
	testFloor = new CEntity();
	
	floor->ADD_Sprite(new CSprite(resources->GetTexResources(5)));
	testFloor->ADD_Sprite(new CSprite(resources->GetTexResources(5)));
	testBox->ADD_Sprite(new CSprite(resources->GetTexResources(4)));	
	
	floor->ADD_Physics(new CPhysics(world, 10, -20, 8, 4, false));
	testFloor->ADD_Physics(new CPhysics(world, 18, -20, 8, 4, false));
	
	

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


	
	// init game variables
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

	delete screenBoundry;

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
			player->SetIdle(false);
		}

		if(input->GetKey(1)->IsEnabled()) // walk down - s
		{
			player->MoveDown(2000);
			player->GetAnimation()->PerformAnimation(0);
			player->SetIdle(false);
		}

		if(input->GetKey(2)->IsEnabled()) // walk left - a
		{	
			player->MoveLeft(2000);		
			player->GetAnimation()->PerformAnimation(1);
			player->SetIdle(false);
		}

		if(input->GetKey(3)->IsEnabled()) // walk right - d
		{
			player->MoveRight(2000);
			player->GetAnimation()->PerformAnimation(2);
			player->SetIdle(false);
		}

		if(input->GetKey(4)->IsEnabled()) // exit - esc
		{
			window->GetMainEvent()->type = SDL_QUIT; 		
		}

		if(input->GetKey(5)->IsEnabled()) // reset - r
		{
			quit = true;
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

			SCORE += 5;
			score->Print(window->GetRenderer(), SCORE);
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

		if (player->GetIdle())
		{
			player->GetAnimation()->UseFrame(1,0);		
		} 

		if (DEBUG)
		{		
			RUNDebug();
		}

		HandlePhysics();

		inputTime = timeCurrent;
		player->SetIdle(true);
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

	for (int i = 0; i < TILE_COLUMN; i++)
	{
		for (int j = 0; j < TILE_ROW; j++)
		{
			tiles[i][j]->Draw(window->GetRenderer());
			if (DEBUG)
			{
				tiles[i][j]->DrawDebug(window->GetRenderer());
			}
		}
	}		
	

	for (int k = 0; k < size; k ++)
	{
		boxes[k]->Draw(window->GetRenderer());
	}

	screenBoundry->Draw(window->GetRenderer());
	
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