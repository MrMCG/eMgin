#pragma once
#include "stdafx.h"

// Holds a texture (sprite) for simple image drawing

class CSprite
{
public:
	CSprite(SDL_Texture* image, bool copy=false);
	CSprite();

	virtual ~CSprite(void);

	void SetTile(int x, int y, int w, int h);
	void SetTEX(SDL_Texture* texture);
	inline void ToggleDel(){del=!del;};

	SDL_Texture* GetTEX() const;
	SDL_Texture* GetTEXMod();
	SDL_Rect* GetCROP() const;

	// used for CText
	virtual void Print(SDL_Renderer* rend, string mssage) {};
	virtual void Print(SDL_Renderer* rend, int number) {};
	virtual void SetColor(int r, int g, int b) {};
	virtual void SetOpacity(int a) {};

protected:
	SDL_Texture* tex;
	SDL_Rect* crop;
	bool del;
};

// hold custom re-writeable texture
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

