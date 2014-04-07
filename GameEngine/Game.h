#pragma once
#include "StdAfx.h"
#include "SDL_Setup.h"
#include "Sprite.h"
#include "Animate.h"
#include "Entity.h"
#include "settings.h"
#include "Text.h"
#include "Debug.h"
#include "Resources.h"
#include "Input.h"
#include "Collision.h"

using namespace settings;

class CGame
{
public:
	CGame(CSDL_Setup* csdl_setup, CResources* passedResources, CInput* passedKeyboard);
	~CGame(void);
	void GameLoop();
	void HandleEvents();
	void HandlePhysics();
	void DrawEntities();
	string GetFrames();
	void RUNDebug();

	void TEST();

private:
	CSDL_Setup* window;
	CResources* resources;
	CInput* keyboard;

	CDebug* debug;

	CEntity* background;
	CEntity* tiles[TILE_COLUMN][TILE_ROW];
	CEntity* dave;
	CEntity* paul;
	CEntity* score;

	b2Body* body;

	b2World* world;

	b2Body* groundBody;

	bool idle;
	bool DEBUG;

	bool quit;

	int SCORE;

	float timePrevious;
	float timeCurrent;
	float inputTime;
};

