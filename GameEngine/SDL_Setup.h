#pragma once
#include "StdAfx.h"

// Initialises SDL

class CSDL_Setup
{
private:
	SDL_Renderer* renderer;
	SDL_Window* window;
	SDL_Event* mainEvent;

public:
	CSDL_Setup();
	~CSDL_Setup(void);

	SDL_Renderer* GetRenderer() const;
	SDL_Event* GetMainEvent() const;
	void SetRenderer(int flags);

	inline int GetScreenWidth()
	{return mainEvent->window.data1;}

	void Begin();
	void End();



	
};

