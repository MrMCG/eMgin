#pragma once
//#include "StdAfx.h"
#include "SDL_Setup.h"
#include "Sprite.h"
#include "Animate.h"
#include "Entity.h"
#include "settings.h"
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
	FLOOR
};

class CLevel
{
public:
	CLevel(CSDL_Setup* csdl_setup, CResources* passedResources, CInput* passedInput);
	~CLevel(void);
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
	CPlayer* player;

	CEnemy** enemies;
	CCrate** crates;
	CTile** floor;

	CTile* tiles[TILE_COLUMN][TILE_ROW];
	
	CBullet* bullet;
	CWriting* timer;
	CWriting* bulletStatus;
	CWriting* restart;
	CWriting* instruction;

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

