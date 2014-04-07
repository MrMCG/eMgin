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
	background->ADD_Sprite(new CSprite(resources->GetTexResources()->GetTEX(1)));	

	for (int i = 0; i < TILE_COLUMN; i++)
	{
		for (int j = 0; j < TILE_ROW; j++)
		{
			tiles[i][j] = new CEntity(
				(TILE_COLUMN_CALC*i), 
				(TILE_ROW_CALC*j), 
				TILE_COLUMN_CALC, 
				TILE_ROW_CALC);
				
			tiles[i][j]->ADD_Sprite(new CSprite(resources->GetTexResources()->GetTEX(2)));
		}
	}

	tiles[0][0]->SetDraw(false);

	dave = new CEntity(0, 300, (int)(TILE_COLUMN_CALC*1.5), (int)(TILE_ROW_CALC*2));
	paul = new CEntity(500, 300, (int)(TILE_COLUMN_CALC*1.5), (int)(TILE_ROW_CALC*2));
		
	dave->ADD_Animation( new CAnimate(window->GetRenderer(), resources->GetTexResources()->GetTEX(3), resources->GetTexResources()->GetTEX(0)));
	paul->ADD_Sprite(new CSprite(resources->GetTexResources()->GetTEX(1)));

	dave->ADD_Collision( new CCollision(window->GetRenderer()));
	paul->ADD_Collision( new CCollision(window->GetRenderer()));

	dave->GetCollision()->SetColBoxSize(50,50);
	paul->GetCollision()->SetColBoxSize(200,200);

	paul->GetCollision()->SetSolBoundry(true);

	dave->ADD_Physics( new CPhysics());

	dave->GetPhysics()->SetMass(1);

	SCORE = 0;

	score = new CEntity(SCREEN_WIDTH/2-40, 0, 80,45);
	score->ADD_Text(new CText());

	score->GetSprite()->Print(window->GetRenderer(), SCORE, 255, 255, 255);

	timeCurrent = 1;
	timePrevious = 1;
	inputTime = 0;

	idle = true;
	DEBUG = false;
}

CGame::~CGame(void)
{	
	
	delete background;
	delete dave;
	for (int i = 0; i < TILE_COLUMN; i++)
	{
		for (int j = 0; j < TILE_ROW; j++)
		{
			delete tiles[i][j];
		}
	}
	delete score;
	delete debug;

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
			dave->GetPhysics()->ApplyExternalForce(Vector2D(0,-10));
			dave->GetAnimation()->Animation(4);
			idle = false;
		}

		if(keyboard->GetKey(1)->IsEnabled()) // walk down
		{
			dave->GetPhysics()->ApplyExternalForce(Vector2D(0,10));
			dave->GetAnimation()->Animation(1);
			idle = false;
		}

		if(keyboard->GetKey(2)->IsEnabled()) // walk left
		{
			dave->GetPhysics()->ApplyExternalForce(Vector2D(-10,0));
			dave->GetAnimation()->Animation(2);
			idle = false;
		}

		if(keyboard->GetKey(3)->IsEnabled()) // walk right
		{
			dave->GetPhysics()->ApplyExternalForce(Vector2D(10,0));
			dave->GetAnimation()->Animation(3);
			idle = false;
		}

		if(keyboard->GetKey(4)->IsEnabled()) // exit
		{
			quit = true;
			//window->GetMainEvent()->type = SDL_QUIT;
		}

		if(keyboard->GetKey(5)->IsEnabled()) // player score
		{
			SCORE += 5;
			score->GetSprite()->Print(window->GetRenderer(), SCORE, 255, 255, 255);
		}

		if(keyboard->GetKey(6)->IsEnabled()) // dev tools
		{
			DEBUG = true;		
			dave->SetDebug(true);
			paul->SetDebug(true);
		}

		if(keyboard->GetKey(7)->IsEnabled()) // dev tools
		{
			DEBUG = false;		
			dave->SetDebug(false);
			paul->SetDebug(false);
		}

		if(keyboard->GetKey(8)->IsEnabled()) // jump
		{
			dave->GetPhysics()->ApplyExternalForce(Vector2D(0,-40));
		}

		

		if (idle)
		{
			dave->GetAnimation()->Animation(1);
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
	float update = 0.1f;
	dave->GetPhysics()->ApplyBackgroundForce(dave->GetPosition(), update);

	dave->GetPhysics()->UpdatePosition(dave->GetPosition(), update);

	dave->GetCollision()->SetColBoxPos(*dave->GetPosition());

	if(dave->HasCollided(paul))
	{
		SDL_SetTextureColorMod(dave->GetCollision()->GetColTex()->GetTEX(), 0,200,0);		
	} else
	{
		SDL_SetTextureColorMod(dave->GetCollision()->GetColTex()->GetTEX(), 50,50,200);
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
		}
	}		

	paul->Draw(window->GetRenderer());
	dave->Draw(window->GetRenderer());
	score->Draw(window->GetRenderer());

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