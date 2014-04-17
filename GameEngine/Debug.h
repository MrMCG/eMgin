#pragma once
#include "Entity.h"
#include "Characters.h"

class CDebug
{
public:
	CDebug(SDL_Renderer* renderer, string level);
	~CDebug(void);
	void Run(SDL_Renderer* renderer);

	bool IsEnabled();
	void ToggleEnabled();
	void UpdateFrames(SDL_Renderer* renderer, string msg);

private:
	CWriting* frames;
	CWriting* info;
	CWriting* levelName;
};

