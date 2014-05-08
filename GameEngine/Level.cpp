#include "Level.h"

CLevel::CLevel(CSDL_Setup* csdl_setup, CResources* passedResources, CInput* passedInput, string levelName)
{
	// get application resources
	window = NULL;
	window = csdl_setup;
	resources = NULL;
	resources = passedResources;
	input = NULL;
	input = passedInput;

	// init dynamic entities
	tiles = new vector<CTile*>;
	floor = new vector<CTile*>;
	crates = new vector<CCrate*>;
	enemies = new vector<CEnemy*>;

	// init contacts
	contacts[BULLET_ENEMIES] = -1;
	contacts[BULLET_CRATES] = -1;
	contacts[PLAYER_CRATES] = -1;

	// Init dev tools
	debug = new CDebug(window->GetRenderer(),levelName);

	// create world
	world = new b2World(b2Vec2(0.0,-100));

	// load map
	location = LEVEL_LOCATION+levelName;
	ifstream file(location+"_map.txt");
	size_t pos = 0;
	string line;
	string current;
	if (file)
	{
		int x,y,s;
		while(file >> x >> y >> s)
		{
			CTile* tile = new CTile(resources,x,y,5);
			if (s)
			{			
				tile->SetSolid(world);
				floor->push_back(tile);
			} else
			{
				tiles->push_back(tile);
			}
		}
	}
	file.close();

	// load crates
	file.open(location+"_crates.txt");
	pos = 0;
	line = "";
	current = "";
	if (file)
	{
		int x,y,s;
		while(file >> x >> y >> s)
		{
			CCrate* crate = new CCrate(resources, world,x,y,s);
			crates->push_back(crate);
		}
	}
	file.close();

	// load enemies
	file.open(location+"_enemies.txt");
	pos = 0;
	line = "";
	current = "";
	if (file)
	{
		int x,y;
		while(file >> x >> y)
		{
			CEnemy* enemy = new CEnemy(window->GetRenderer(), resources,world,x,y);
			enemies->push_back(enemy);
		}
	}
	file.close();

	// Enable collision detection
	colList = new CCollisionListener();
	world->SetContactListener(colList);

	// create world border
	screenBoundry = new CBoundry(resources, world);

	// create background
	background = new CBackground(resources);

	// create player
	player = new CPlayer(window->GetRenderer(), resources, world);
	player->GetPhysics()->EnableDebugTex(window->GetRenderer(), world, colList);

	// create re-usable bullet
	bullet = new CBullet(resources, world);
	bullet->GetPhysics()->EnableDebugTex(window->GetRenderer(), world, colList);

	// create score timer
	time = 60000+SDL_GetTicks(); // set to 30 seconds
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

	// load score
	scores = new CWriting(0, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT/8);
	scores->SetColor(255,255,255);
	scores->SetBackdrop(new CSprite(resources->GetTex(7)));
	scores->Print(window->GetRenderer(), "     Your Score: 0     ");

	// create end instruction
	instruction = new CWriting(0, (SCREEN_HEIGHT/8)*5, SCREEN_WIDTH, SCREEN_HEIGHT/8);
	instruction->SetColor(255,255,255);
	instruction->SetBackdrop(new CSprite(resources->GetTex(7)));
	instruction->Print(window->GetRenderer(), "   Press 'r' To Restart   ");

	// init game variables
	timeCurrent = 1;
	timePrevious = 1;
	inputTime = 0;

	DEBUG = false;	// is debug enabled
	quit = false; // has player restarted level
	playerWon = false; // has the player won
	next = false; // able to proceed to next level

	// ------------------------- Create collisions -------------------------
	// ------------------------- Order is important! -----------------------
	// enemies and bullets
	for (vector<CEnemy*>::iterator it = enemies->begin(); it < enemies->end(); it++)
	{
		colDet.push_back( colList->ADD_Collision(bullet->GetPhysicsData(), (*it)->GetPhysicsData()) );
		contacts[BULLET_ENEMIES]++;
	}

	// bullets and crates
	contacts[BULLET_CRATES] = contacts[BULLET_ENEMIES];
	for (vector<CCrate*>::iterator it = crates->begin(); it < crates->end(); it++)
	{
		colDet.push_back( colList->ADD_Collision(bullet->GetPhysicsData(), (*it)->GetPhysicsData()) );
		contacts[BULLET_CRATES]++;
	}

	// player and crates
	contacts[PLAYER_CRATES] = contacts[BULLET_CRATES];
	for (vector<CCrate*>::iterator it = crates->begin(); it < crates->end(); it++)
	{
		colDet.push_back( colList->ADD_Collision(player->GetPhysicsData(), (*it)->GetPhysicsData()) );
		contacts[PLAYER_CRATES]++;
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

	for (vector<CEnemy*>::iterator it = enemies->begin(); it < enemies->end(); it++)
	{
		delete (*it);
	}
	delete enemies;
	for (vector<CCrate*>::iterator it = crates->begin(); it < crates->end(); it++)
	{
		delete (*it);
	}
	delete crates;
	for (vector<CTile*>::iterator it = tiles->begin(); it < tiles->end(); it++)
	{
		delete (*it);
	}
	delete tiles;
	for (vector<CTile*>::iterator it = floor->begin(); it < floor->end(); it++)
	{
		delete (*it);
	}
	delete floor;

	delete bullet;
	delete timer;
	delete bulletStatus;
	delete restart;
	delete instruction;
	delete scores;

	delete colList;
}

int CLevel::GameLoop()
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

	return next;
}

void CLevel::HandleEvents()
{
	if (time)
	{
		time -= timeCurrent - timePrevious; // update score
	} 

	if(window->ResetScreen())
	{
		quit = true; // if the screen is resized, uppdate rendereer and window	
	}

	if (inputTime+INPUT_DELAY < timeCurrent)
	{		
		input->Poll(window->GetMainEvent()); // get input

		

		// ------------------------- Player related updates -------------------------
		if (player->isAlive() && !playerWon)
		{
			if(input->GetKey(0)->IsEnabled()) // walk up - w
			{
				player->MoveUp(2500);
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
				player->MoveLeft(2000);		
				player->GetAnimation()->PerformAnimation(1); //activate lights
			
				resources->PlaySound(3,0,1); // play rocket sound
			
				player->SetIdle(false);
			}

			if(input->GetKey(3)->IsEnabled()) // walk right - d
			{
				player->MoveRight(2000);
				player->GetAnimation()->PerformAnimation(1); //activate lights

				resources->PlaySound(3,0,1); // play rocket sound
				
				player->SetIdle(false);
			}

			if(input->GetKey(8)->IsEnabled()) // fire - space
			{
				if (!bullet->IsActive() && bullet->Fire(world, player)) // if able to fire bullet, fire bullet
				{
					resources->PlaySound(2,0); // play bullet sound
				}
			}

			if (player->GetIdle()) // display idle animation
			{
				player->GetAnimation()->PerformAnimation(0); // idle player animation		
			} 

			player->SetIdle(true); // reset idle stance

			// ------------------------- determine if player has won -------------------------
			playerWon = true;
			for (vector<CEnemy*>::iterator it = enemies->begin(); it < enemies->end(); it++)
			{
				if ((*it)->isAlive())
				{
					playerWon = false;
					break;
				}
			}

			if (playerWon)
			{
				// determine score
				ifstream file;
				file.open(location+"_scores.txt");
				size_t pos = 0;
				string line;
				string current;
				int hiScore = 0;
				if (file)
				{
					int x;
					while(file >> x)
					{
						hiScore = x;
					}
				}
				file.close();

				if (time/10 > hiScore)
				{
					scores->Print(window->GetRenderer(), "   New Hi-Score!: "+to_string((long long)time/10)+"   ");
					hiScore = time/10;
					ofstream writer(location+"_scores.txt");
					writer << hiScore << endl;		
					writer.close();
				} else
				{
					scores->Print(window->GetRenderer(), "   Your Score: "+to_string((long long)time/10)+ "     Hi-Score: "+to_string((long long)hiScore)+"   ");
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

			timer->Print(window->GetRenderer(), time/10); // print updated score

			if (playerWon) // handle player win end title
			{
				restart->SetColor(0,255,0);
				restart->Print(window->GetRenderer(), "YOU WON!");

				instruction->Print(window->GetRenderer(), "  -r- To Restart      -e- To Continue  ");

				timer->Print(window->GetRenderer(), time/10);
				time = 0;
			}		
		}

		// ------------------------- Other input -------------------------
		if(input->GetKey(4)->IsEnabled()) // exit - esc
		{
			window->GetMainEvent()->type = SDL_QUIT; 		
		}

		if(input->GetKey(5)->IsEnabled() && time < 59000) // reset - r
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

		if( (playerWon || DEBUG) && input->GetKey("e")->IsEnabled() ) // continue to next level - e ;
		{
			next = true;
			quit = true;
		}

		// ------------------------- Handle Other Events -------------------------
		if (time < 0) // update player state based on timer
		{
			time = 0;
			player->Kill();
			timer->Print(window->GetRenderer(), "0");
		}

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
	if (player->isAlive())
	{
		world->Step(1.0f/40.0f, 5, 3);
	} else // if player is dead, slow physics down
	{
		world->Step(1.0f/1000.0f, 5, 3);
	}

	// ----------------- Update entity Positions --------------
	// dynamic entities
	// enemies
	for (vector<CEnemy*>::iterator it = enemies->begin(); it < enemies->end(); it++)
	{
		(*it)->UpdatePosition();
		(*it)->GetAnimation()->PerformAnimation(0);

		// debug updates
		if (DEBUG)
		{
			(*it)->GetPhysics()->UpdateDebugBox(colList);
		}
	}

	// crates
	for (vector<CCrate*>::iterator it = crates->begin(); it < crates->end(); it++)
	{
		(*it)->UpdatePosition();
	}

	// player
	player->UpdatePosition();

	// bullet
	if (bullet->IsActive())
	{
		bullet->UpdatePosition();
	}

	// debug updates
	if (DEBUG)
	{
		player->GetPhysics()->UpdateDebugBox(colList);
		bullet->GetPhysics()->UpdateDebugBox(colList);
	}


	// ------------------------- COLLISIONS -------------------------
	// bullet and enemies
	int entColCount = contacts[BULLET_ENEMIES];
	for (int k = 0; k <= entColCount; k ++)
	{
		if (colList->HasCollided(colDet[k]))
		{

			resources->PlaySound(1,0,2);
			if (bullet->IsActive())
			{
				bullet->Explode(world, 10, 1000);
			}
			enemies->at(k)->Kill();		
		}
	}

	// bullet and crate
	for (int k = entColCount; k <= contacts[BULLET_CRATES]; k ++)
	{
		if (colList->HasCollided(colDet[k]))
		{
			if (bullet->IsActive())
			{
				resources->PlaySound(1,0,2);
				bullet->Explode(world, 15, 1500);
			}	
		}
	}

	// player and crate
	entColCount = contacts[BULLET_CRATES];
	for (int k = entColCount; k <= contacts[PLAYER_CRATES]; k ++)
	{
		// if player collides with crates and is alive
		if (colList->HasCollided(colDet[k]) && player->isAlive()) 
		{
			// play explosion
			resources->PlaySound(1,0,2); 
			if (bullet->IsActive()) // if bullet is active - explode
			{
				bullet->Explode(world, 10, 1000);
			} else
			{
				if (bullet->Fire(world, player)) // fire bullet and explode
				{
					bullet->Explode(world, 10, 1000);
				}
			} 
			// kill player
			player->Kill();	
		}
	}
}

void CLevel::DrawEntities()
{
	background->Draw(window->GetRenderer());	

	for (vector<CTile*>::iterator it = tiles->begin(); it < tiles->end(); it++)
	{
		(*it)->Draw(window->GetRenderer());
		if (DEBUG)
		{
			(*it)->DrawDebug(window->GetRenderer());
		}
	}

	for (vector<CTile*>::iterator it = floor->begin(); it < floor->end(); it++)
	{
		(*it)->Draw(window->GetRenderer());
		if (DEBUG)
		{
			(*it)->DrawDebug(window->GetRenderer());
		}
	}	

	for (vector<CCrate*>::iterator it = crates->begin(); it < crates->end(); it++)
	{
		(*it)->Draw(window->GetRenderer());
	}

	for (vector<CEnemy*>::iterator it = enemies->begin(); it < enemies->end(); it++)
	{
		if ((*it)->isAlive())
		{
			(*it)->Draw(window->GetRenderer());
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
		scores->Draw(window->GetRenderer());
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