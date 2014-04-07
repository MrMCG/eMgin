#pragma once
#include "StdAfx.h"

class CAnimate
{
public:
	CAnimate(
		SDL_Renderer* pass_renderer, 
		SDL_Texture* image, 
		SDL_Texture* errorTex);

	~CAnimate(void);

	void SetALL();

	void Animation(int type);
	void PerformAnimation(int animType);
	void UpdateDelay();

	SDL_Texture* GetTEX() const;
	SDL_Rect GetCROP() const;
	int GetRow() const;
	int GetFrame() const;

private:
	SDL_Texture* tex;
	SDL_Rect crop;
	int texWidth;
	int texHeight;
	bool error;
	int ANIM_currentFrame;

	int IDLE_beginFrame;
	int IDLE_endFrame;

	int ANIM_DELAY;	
	int ANIM_SPEED;	
	int ANIM_ROW_SIZE;
	int ANIM_COLUMN_SIZE;
};

