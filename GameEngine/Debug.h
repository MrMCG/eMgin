#pragma once
#include "Entity.h"

class CDebug
{
public:
	CDebug(SDL_Renderer* renderer);
	~CDebug(void);
	void Run(SDL_Renderer* renderer);

	bool IsEnabled();
	void ToggleEnabled();
	void UpdateFrames(SDL_Renderer* renderer, string msg);

private:
	CEntity* Frames;
	CEntity* videoCard;
};

