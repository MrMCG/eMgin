#pragma once
#include "StdAfx.h"

// Initialises SDL

class CSDL_Setup
{
private:
	SDL_Renderer* renderer;
	SDL_Window* window;
	SDL_Event* mainEvent;
	int res[11][2];

	void SetScreen(int flags);
	void LoadSettings();
	void CalcSettings();

public:
	CSDL_Setup();
	~CSDL_Setup(void);

	SDL_Renderer* GetRenderer() const;
	SDL_Event* GetMainEvent() const;

	bool ResetScreen();
	
	void Begin();
	void End();
};

