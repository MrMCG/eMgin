#pragma once
#include "StdAfx.h"

// Initialises SDL

class CSDL_Setup
{
public:
	CSDL_Setup();
	~CSDL_Setup(void);

	SDL_Renderer* GetRenderer() const;
	SDL_Event* GetMainEvent() const;
	void SetRenderer(int flags);

	void Begin();
	void End();

private:
	SDL_Renderer* renderer;
	SDL_Window* window;
	SDL_Event* mainEvent;

	
};

