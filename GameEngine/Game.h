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

enum dynamicEntites
{
	MAX_ENTITIES,
	ENEMIES,
	CRATES,
	FLOOR,
	EXPLOSIVES
};

class CGame
{
public:
	CGame(CSDL_Setup* csdl_setup, CResources* passedResources, CInput* passedInput);
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
	CInput* input;

	CDebug* debug;
	b2World* world;
	CBoundry* screenBoundry;
	CBackground* background;
	CTile* tiles[TILE_COLUMN][TILE_ROW];
	CPlayer* player;

	CEnemy** enemies;
	CCrate** crates;
	CBullet* bullet;

	CWriting* timer;
	CWriting* bulletStatus;
	CWriting* restart;

	CEntity* floor;
	CEntity* testFloor;
	CCollisionListener* colList;

	int size[4];
	vector<int> colDet;
	bool DEBUG;
	bool quit;
	int time;
	float timePrevious;
	float timeCurrent;
	float inputTime;
	bool playerWon;
};

