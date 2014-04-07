#include "StdAfx.h"
#include "Game_Menu.h"

CGame_Menu::CGame_Menu(void)
{
	csdl_setup = new CSDL_Setup();
	csdl_setup->SetRenderer(1);
	resources = new CResources(csdl_setup->GetRenderer());
	//game = new CGame(csdl_setup, resources);

	keyboard = NULL;
	keyboard = new CInput(10);

	walkUp = new CKeyboard_Event(SDLK_w);
	walkDown = new CKeyboard_Event(SDLK_s);
	walkLeft = new CKeyboard_Event(SDLK_a);
	walkRight = new CKeyboard_Event(SDLK_d);
	exit = new CKeyboard_Event(SDLK_ESCAPE);
	playerScore = new CKeyboard_Event(SDLK_h);
	devToolOn = new CKeyboard_Event(SDLK_QUOTE);

	CInput_Event* devToolOff = new CKeyboard_Event(SDLK_SEMICOLON);
	CInput_Event* jump = new CKeyboard_Event(SDLK_SPACE);

	keyboard->AddEvent(walkUp);
	keyboard->AddEvent(walkDown);
	keyboard->AddEvent(walkLeft);
	keyboard->AddEvent(walkRight);
	keyboard->AddEvent(exit);
	keyboard->AddEvent(playerScore);
	keyboard->AddEvent(devToolOn);
	keyboard->AddEvent(devToolOff);
	keyboard->AddEvent(jump);


}

CGame_Menu::~CGame_Menu(void)
{
	delete csdl_setup;
	delete resources;
	delete keyboard;
}

void CGame_Menu::Run()
{
	for (int k = 0; k < 20; k++)
	{
		game = new CGame(csdl_setup, resources, keyboard);

		game->GameLoop();

		delete game;
	}
}
