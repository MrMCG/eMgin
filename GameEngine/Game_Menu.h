#pragma once
#include "SDL_Setup.h"
#include "Game.h"
#include "Resources.h"
#include "Input.h"
#include "Input_Event.h"
#include "Keyboard_Event.h"

class CGame_Menu
{
public:
	CGame_Menu();
	~CGame_Menu(void);

	void Run();

private:
	CSDL_Setup* csdl_setup;
	CGame* game;
	CResources* resources;
	CInput* keyboard;

	CInput_Event* walkUp;
	CInput_Event* walkDown;
	CInput_Event* walkLeft;
	CInput_Event* walkRight;
	CInput_Event* exit;
	CInput_Event* devToolOn;
	CInput_Event* playerScore;
};

