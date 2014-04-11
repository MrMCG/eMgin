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
#include "Characters.h"

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

private:
	CSDL_Setup* window;
	CResources* resources;
	CInput* keyboard;

	CDebug* debug;

	CEntity* background;
	CEntity* tiles[TILE_COLUMN][TILE_ROW];
	CPlayer* player;
	CBullet* bullet;
	CEntity* floor;
	CEntity* score;

	CEntity* testFloor;
	CEntity* testBox;

	CPhysics* leftSide;
	CPhysics* rightSide;
	CPhysics* topSide;
	CPhysics* bottomSide;

	CEntity** boxes;
	int size;

	b2World* world;

	bool idle;
	bool DEBUG;
	bool fired;

	bool quit;

	int SCORE;

	float timePrevious;
	float timeCurrent;
	float inputTime;
};

