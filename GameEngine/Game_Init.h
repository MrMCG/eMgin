#pragma once
#include "SDL_Setup.h"
#include "Level.h"
#include "Resources.h"
#include "Input.h"

/* Init game engine, resources and levels */

const int LEVELS = 3; // number of levels

class CGame_Init
{
public:
	CGame_Init();
	~CGame_Init(void);

	void Run();

private:
	CSDL_Setup* csdl_setup; // SDL init
	CLevel* game; // loaded level
	CResources* resources; // stored resources
	CInput* input; // input used
	string levels[LEVELS]; // name of levels
};

