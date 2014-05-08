#pragma once
#include "StdAfx.h"

// Initialises SDL and settings

class CSDL_Setup
{
private:
	SDL_Renderer* renderer;
	SDL_Window* window;
	SDL_Event* mainEvent;
	int res[11][2]; // list of acceptable resolutions

	void SetScreen(int flags); // create window/renderer
	void LoadSettings(); // load settings file
	void CalcSettings(); // calc ingame settings from settings file

public:
	CSDL_Setup();
	~CSDL_Setup(void);

	SDL_Renderer* GetRenderer() const;
	SDL_Event* GetMainEvent() const;

	bool ResetScreen(); // screen resize
	
	void Begin(); // begin frame
	void End(); // end frame
};

