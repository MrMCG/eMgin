#include "Level.h"

CLevel::CLevel(CSDL_Setup* csdl_setup, CResources* passedResources, CInput* passedInput)
{
	// get application resources
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

	// init dynamic entities
	size[ENEMIES] = 2;
	size[CRATES] = 0;
	size[FLOOR] = 2;
	size[MAX_ENTITIES] = size[ENEMIES]+size[CRATES]+size[FLOOR];
	crates = new CCrate*[size[CRATES]];
	enemies = new CEnemy*[size[ENEMIES]];
	floor = new CTile*[size[FLOOR]];

	// Init dev tools
	debug = new CDebug(window->GetRenderer());

	// create world
	world = new b2World(b2Vec2(0.0,-100));

	// Enable collision detection
	colList = new CCollisionListener();
	world->SetContactListener(colList);

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
	tiles[7][3]->ADD_Sprite(new CSprite(resources->GetTex(5)));
	tiles[7][3]->SetSolid(world);
	tiles[9][3]->ADD_Sprite(new CSprite(resources->GetTex(5)));
	tiles[9][3]->SetSolid(world);
	floor[0] = tiles[7][2];
	floor[1] = tiles[9][3];

	// create player
	player = new CPlayer(window->GetRenderer(), resources, world);
	player->GetPhysics()->EnableDebugTex(window->GetRenderer(), world, colList);

	// create re-usable bullet
	bullet = new CBullet(resources, world);
	bullet->GetPhysics()->EnableDebugTex(window->GetRenderer(), world, colList);

	// create score timer
	time = 20000+SDL_GetTicks();
	timer = new CWriting(SCREEN_WIDTH/2-70, 0, 140,70);
	timer->SetColor(255,255,255);
	timer->Print(window->GetRenderer(), time);

	// create bullet status
	bulletStatus = new CWriting(0, SCREEN_HEIGHT-70, 140,70);
	bulletStatus->SetColor(0,255,0);
	bulletStatus->Print(window->GetRenderer(), "ARMED");

	// create end game display
	restart = new CWriting(0, SCREEN_HEIGHT/4, SCREEN_WIDTH, SCREEN_HEIGHT/4);
	restart->SetColor(255,0,0);
	restart->SetBackdrop(new CSprite(resources->GetTex(7)));
	restart->Print(window->GetRenderer(), "YOU LOSE!");

	// create end instruction
	instruction = new CWriting(0, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT/8);
	instruction->SetColor(255,255,255);
	instruction->SetBackdrop(new CSprite(resources->GetTex(7)));
	instruction->Print(window->GetRenderer(), "Press 'r' To Restart");
	
	// randomly create boxes
	int posX;
	int posY;
	int radius;
	
	for (int k = 0; k < size[MAX_ENTITIES]; k ++)
	{
		radius = 4;//(int) (rand() % 5) + 2;
		posX = (int) (rand() % 123) + 1;
		posY = (int) (rand() % 60) + 1;

		if (k < size[CRATES])
		{
			crates[k] = new CCrate(resources, world, posX, posY, radius);
		}

		if (k < size[ENEMIES])
		{
			enemies[k] = new CEnemy(window->GetRenderer(), resources, world, posX, posY, 6, 6);
		}
	}
	
	// init game variables
	timeCurrent = 1;
	timePrevious = 1;
	inputTime = 0;

	DEBUG = false;	
	quit = false;
	playerWon = false;

	// ------------------------- Create collisions -------------------------
	// ------------------------- Order is important! -----------------------
	for (int k = 0; k < size[ENEMIES]; k ++)
	{
		colDet.push_back( colList->ADD_Collision(bullet->GetPhysicsData(), enemies[k]->GetPhysicsData()) );
	}
}

CLevel::~CLevel(void)
{	
	// derefrence
	input = NULL;
	window = NULL;
	resources = NULL;

	delete debug;
	delete world;
	delete screenBoundry;
	delete background;	
	delete player;

	for (int k = 0; k < size[MAX_ENTITIES]; k ++)
	{
		if (k < size[CRATES])
		{
			delete crates[k];
		}
		if (k < size[ENEMIES])
		{
			delete enemies[k];
		}
		if (k < size[FLOOR])
		{
			floor[k]=NULL;
		}
	}
	delete crates;
	delete enemies;
	delete floor;

	for (int i = 0; i < TILE_COLUMN; i++)
	{
		for (int j = 0; j < TILE_ROW; j++)
		{
			delete tiles[i][j];
		}
	}

	delete bullet;
	delete timer;
	delete bulletStatus;
	delete restart;
	delete instruction;

	delete colList;
}

void CLevel::GameLoop()
{
	
	while(window->GetMainEvent()->type != SDL_QUIT && !quit)
	{	
		// update game timers and frames
		timePrevious = timeCurrent;
		timeCurrent = SDL_GetTicks();		

		window->Begin(); // clear frame

		

		HandleEvents(); // update game state

		DrawEntities(); // draw

		window->End(); // end frame
	}
}

void CLevel::HandleEvents()
{
	if (time)
	{
		time -= timeCurrent - timePrevious; // update score
	} 

	if (inputTime+INPUT_DELAY < timeCurrent)
	{		
		input->Poll(window->GetMainEvent()); // get input


		// ------------------------- Player related updates -------------------------
		if (player->isAlive() && !playerWon)
		{
			if(input->GetKey(0)->IsEnabled()) // walk up - w
			{
				player->MoveUp(2000);
				player->GetAnimation()->PerformAnimation(1);
			
				resources->PlaySound(3,0,1);
			
			
				player->SetIdle(false);
			} 

			if(input->GetKey(1)->IsEnabled()) // walk down - s
			{
				player->MoveDown(1200);
			}

			if(input->GetKey(2)->IsEnabled()) // walk left - a
			{	
				player->MoveLeft(1500);		
				player->GetAnimation()->PerformAnimation(1);
			
				resources->PlaySound(3,0,1);
			
				player->SetIdle(false);
			}

			if(input->GetKey(3)->IsEnabled()) // walk right - d
			{
				player->MoveRight(1500);
				player->GetAnimation()->PerformAnimation(1);

				resources->PlaySound(3,0,1);
				
				player->SetIdle(false);
			}

			if(input->GetKey(8)->IsEnabled()) // fire - space
			{
				if (!bullet->IsActive() && bullet->Fire(world, player))
				{
					resources->PlaySound(2,0);
				}
			}

			if (player->GetIdle()) // display idle animation
			{
				player->GetAnimation()->PerformAnimation(0);		
			} 

			player->SetIdle(true);

			// ------------------------- determine if player has won -------------------------
			playerWon = true;
			for (int k = 0; k < size[ENEMIES]; k ++)
			{
				if (enemies[k]->isAlive())
				{
					playerWon = false;
					break;
				}
			}

			if (bullet->IsActive()) // update bullet status
			{
				bulletStatus->SetColor(255,0,0);
				bulletStatus->Print(window->GetRenderer(), "DISARMED");
			} else
			{
				bulletStatus->SetColor(0,255,0);
				bulletStatus->Print(window->GetRenderer(), "ARMED");
			}

			if (time < 0) // update player state based on timer
			{
				time = 0;
				player->Kill();
				timer->Print(window->GetRenderer(), "0");
			}

			timer->Print(window->GetRenderer(), time/10);

			if (playerWon) // handle player win
			{
				restart->SetColor(0,255,0);
				restart->Print(window->GetRenderer(), "YOU WON!");

				instruction->Print(window->GetRenderer(), "-r- To Restart      -e- To Continue");

				timer->Print(window->GetRenderer(), time/10);
				time = 0;
			}		
		}

		// ------------------------- Other input -------------------------
		if(input->GetKey(4)->IsEnabled()) // exit - esc
		{
			window->GetMainEvent()->type = SDL_QUIT; 		
		}

		if(input->GetKey(5)->IsEnabled() && time/10 < 1900) // reset - r
		{
			quit = true;
		}

		if(input->GetKey(6)->IsEnabled()) // dev tools on - '
		{
			DEBUG = true;		
			player->SetDebug(true);
			bullet->SetDebug(true);
		}

		if(input->GetKey(7)->IsEnabled()) // dev tools off - ;
		{
			DEBUG = false;		
			player->SetDebug(false);
			bullet->SetDebug(false);
		}

		// ------------------------- Handle Other Events -------------------------
		

		if (DEBUG) // update debug
		{		
			RUNDebug();
		}

		HandlePhysics();

		// update time state
		inputTime = timeCurrent;
		
	}
}

void CLevel::HandlePhysics()
{	
	// Update world physics
	world->Step(1.0f/30.0f, 5, 3);

	// ----------------- Update entity Positions --------------
	// dynamic entities
	for (int k = 0; k < size[MAX_ENTITIES]; k ++)
	{
		if (k < size[CRATES])
		{
			crates[k]->UpdatePosition();
		}
		if (k < size[ENEMIES])
		{
			enemies[k]->UpdatePosition();
			enemies[k]->GetAnimation()->PerformAnimation(0);
			if (DEBUG)
			{
				enemies[k]->GetPhysics()->UpdateDebugBox(colList);
			}
		}	
	}

	// player
	player->UpdatePosition();

	// bullet
	if (bullet->IsActive())
	{
		bullet->UpdatePosition();
	}

	if (DEBUG)
	{
		player->GetPhysics()->UpdateDebugBox(colList);
		bullet->GetPhysics()->UpdateDebugBox(colList);
	}


	// ------------------------- COLLISIONS -------------------------
	int entColCount = size[ENEMIES];
	for (int k = 0; k < entColCount; k ++)
	{
		if (colList->HasCollided(colDet[k]))
		{

			resources->PlaySound(1,0,2);
			if (bullet->IsActive())
			{
				bullet->Explode(world, 10, 1000);
			}
			enemies[k]->Kill();
			
		}
	}
}

void CLevel::DrawEntities()
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
	

	for (int k = 0; k < size[MAX_ENTITIES]; k ++)
	{
		if(k < size[CRATES])
		{
			crates[k]->Draw(window->GetRenderer());
		}

		if(k < size[ENEMIES])
		{
			if (enemies[k]->isAlive())
			{
				
				enemies[k]->Draw(window->GetRenderer());
			}
		}
	}

	screenBoundry->Draw(window->GetRenderer());

	if (bullet->IsActive())
	{
		bullet->Draw(window->GetRenderer());
	}

	if (player->isAlive() && !playerWon)
	{
		player->Draw(window->GetRenderer());
	} else 
	{
		restart->Draw(window->GetRenderer());
		instruction->Draw(window->GetRenderer());
	}

	timer->Draw(window->GetRenderer());
	bulletStatus->Draw(window->GetRenderer());

	if (DEBUG)
	{
		debug->Run(window->GetRenderer());
	}
}

string CLevel::GetFrames() // update frame rate (approx)
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

void CLevel::RUNDebug() // send framerate to debug
{
	debug->UpdateFrames(window->GetRenderer(), GetFrames());
}