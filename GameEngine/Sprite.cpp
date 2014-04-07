#include "StdAfx.h"
#include "Sprite.h"

CSprite::CSprite()
{
	tex = NULL;
	crop = NULL;
	del = false;
}

CSprite::CSprite(SDL_Texture* image)
{
	tex = NULL;
	tex = image;

	if (tex == NULL)
	{
		printf("ERROR: CSprite - %s\n", SDL_GetError());
	}

	crop = NULL;
	del = false;
}


void CSprite::SetTile(int x, int y, int w, int h)
{
	crop = new SDL_Rect();
	crop->x = x;
	crop->y = y;
	crop->w = w;
	crop->h = h;
}

CSprite::~CSprite(void)
{
	if (del)
	{
		SDL_DestroyTexture(tex);
	} 
	tex = NULL;
	delete crop;
}

void CSprite::SetTEX(SDL_Texture* texture)
{
	tex = texture;
}

SDL_Texture* CSprite::GetTEX() const
{
	return tex;
}

SDL_Rect* CSprite::GetCROP() const
{
	return crop;
}

void CSprite::ToggleDel()
{
	del = !del;
}

