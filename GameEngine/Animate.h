#pragma once
#include "StdAfx.h"
#include "Resources.h"

class CAnimate
{
public:
	CAnimate(
		SDL_Renderer* pass_renderer, 
		CResources* resources, 
		int index);

	~CAnimate(void);

	// type represents the animation for a row in the sprite sheet
	// starting with 0
	void PerformAnimation(int animType);

	// display only one frame from the sprite sheet
	// coor use top left of square
	void UseFrame(int column, int row);
	void UpdateDelay();

	// set sprite sheet size
	inline void SetSpriteSheet(int columns, int rows) {columnSize=columns;rowSize=rows;};

	// set animation speed
	inline void SetSpeed(int cSpeed) {speed=cSpeed;};

	// set frame reset
	inline void SetReset(bool flag) {resetFrame=flag;};

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
	int currentFrame;
	int delay;	
	int prevAnimType;

	// reset frame start on change of animation
	bool resetFrame;

	// how often to run the animation
	int speed;

	// size of spritesheet
	int rowSize;
	int columnSize;
};

