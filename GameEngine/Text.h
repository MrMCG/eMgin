#pragma once
#include "stdafx.h"
#include "Sprite.h"

class CText : public CSprite
{
public:
	CText();
	~CText();

	virtual void Print(SDL_Renderer* rend, string message);
	virtual void Print(SDL_Renderer* rend, int number);
	virtual void SetColor(int r, int g, int b);
	virtual void SetOpacity(int a);
	
protected:
	TTF_Font* font;
	SDL_Surface* surf;
	SDL_Color color;
};

