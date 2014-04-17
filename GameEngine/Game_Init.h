#pragma once
#include "SDL_Setup.h"
#include "Level.h"
#include "Resources.h"
#include "Input.h"

const int LEVELS = 3;

class CGame_Init
{
public:
	CGame_Init();
	~CGame_Init(void);

	void Run();

	int screenWidth;

private:
	CSDL_Setup* csdl_setup;
	CLevel* game;
	CResources* resources;
	CInput* input;
	string levels[LEVELS];
};

