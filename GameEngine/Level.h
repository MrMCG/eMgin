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
	BULLET_ENEMIES,
	BULLET_CRATES,
	PLAYER_CRATES
};

class CLevel
{
public:
	CLevel(CSDL_Setup* csdl_setup, CResources* passedResources, CInput* passedInput, string levelName);
	~CLevel(void);
	int GameLoop();
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

	vector<CEnemy*>* enemies;
	vector<CCrate*>* crates;
	vector<CTile*>* tiles;
	vector<CTile*>* floor;
	
	CBullet* bullet;
	CWriting* timer;
	CWriting* bulletStatus;
	CWriting* restart;
	CWriting* instruction;
	CWriting* scores;

	CCollisionListener* colList;

	int contacts[4];
	vector<int> colDet;
	bool DEBUG;
	bool quit;
	int time;
	float timePrevious;
	float timeCurrent;
	float inputTime;
	bool playerWon;
	bool next;
	string location;
};

