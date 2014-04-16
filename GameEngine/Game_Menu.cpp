#include "StdAfx.h"
#include "Game_Menu.h"

namespace settings
{
	int* boxData = new INT;
}

CGame_Menu::CGame_Menu(void)
{
	csdl_setup = new CSDL_Setup();
	csdl_setup->SetRenderer(1);
	resources = new CResources(csdl_setup->GetRenderer());
	resources->GetSoundResources()->SetVolume(1,128);

	keyboard = NULL;
	keyboard = new CInput(10);

	walkUp = new CKeyboard_Event(SDLK_w);
	walkDown = new CKeyboard_Event(SDLK_s);
	walkLeft = new CKeyboard_Event(SDLK_a);
	walkRight = new CKeyboard_Event(SDLK_d);
	exit = new CKeyboard_Event(SDLK_ESCAPE);
	reset = new CKeyboard_Event(SDLK_r);
	devToolOn = new CKeyboard_Event(SDLK_QUOTE);

	CInput_Event* devToolOff = new CKeyboard_Event(SDLK_SEMICOLON);
	CInput_Event* jump = new CKeyboard_Event(SDLK_SPACE);

	keyboard->AddEvent(walkUp);
	keyboard->AddEvent(walkDown);
	keyboard->AddEvent(walkLeft);
	keyboard->AddEvent(walkRight);
	keyboard->AddEvent(exit);
	keyboard->AddEvent(reset);
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
	resources->PlayMusic(1, -1);
	while (csdl_setup->GetMainEvent()->type != SDL_QUIT)
	{
		game = new CGame(csdl_setup, resources, keyboard);

		game->GameLoop();

		delete game;
	}
}
