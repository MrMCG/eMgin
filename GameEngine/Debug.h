#pragma once
#include "Entity.h"
#include "Characters.h"

/* handles basic debug info */

class CDebug
{
public:
	CDebug(SDL_Renderer* renderer, string level);
	~CDebug(void);

	void Run(SDL_Renderer* renderer);
	void UpdateFrames(SDL_Renderer* renderer, string msg); // print frame rate (approximate)

private:
	CWriting* frames; // frame rate
	CWriting* info; // system info
	CWriting* levelName; // current level
};

