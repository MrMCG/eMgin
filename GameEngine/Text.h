#pragma once
#include "stdafx.h"
#include "Sprite.h"

class CText : public CSprite
{
public:
	CText();
	virtual ~CText(void);

	virtual void Print(SDL_Renderer* rend, string mssage, int r, int g, int b);
	virtual void Print(SDL_Renderer* rend, int number, int r, int g, int b);
	
protected:
	TTF_Font* font;
	SDL_Surface* surf;
};

