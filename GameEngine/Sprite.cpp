#include "Sprite.h"

// ----------------------------------------------------------
// ------------------------- Sprite -------------------------
// ----------------------------------------------------------
CSprite::CSprite()
{
	tex = NULL;
	crop = NULL;
	del = false;
}

CSprite::CSprite(SDL_Texture* image, bool copy)
{
	tex = NULL;
	if (!copy)
	{
		tex = image;
	} else
	{
		tex = (SDL_Texture*) malloc(sizeof(image));
		tex = image;
	}

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
	if (del)
	{
		SDL_DestroyTexture(tex);
	} 

	tex = texture;
}

SDL_Texture* CSprite::GetTEX() const
{
	return tex;
}

SDL_Texture* CSprite::GetTEXMod() 
{
	return tex;
}

SDL_Rect* CSprite::GetCROP() const
{
	return crop;
}

// ----------------------------------------------------------
// ------------------------- Text ---------------------------
// ----------------------------------------------------------
CText::CText()
{
	font = NULL;
	surf = NULL;
	color.r = 0;
	color.g = 0;
	color.b = 0;
	color.a = 255;

	font = TTF_OpenFont("SourceSansPro-Regular.ttf", 288);

	if (font == NULL)
	{
		printf("ERROR: CText - %s\n", TTF_GetError());
	}
}

CText::~CText(void)
{
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
}

void CText::SetColor(int r, int g, int b)
{
	color.r = r;
	color.g = g;
	color.b = b;
}

void CText::SetOpacity(int a)
{
	color.a = a;
}

void CText::Print(SDL_Renderer* rend, string message)
{
	SDL_FreeSurface(surf);
	surf = TTF_RenderText_Blended(font, message.c_str(), color);

	SDL_DestroyTexture(tex);
	tex = SDL_CreateTextureFromSurface(rend, surf);
}

void CText::Print(SDL_Renderer* rend, int number)
{
	string message = to_string((long long)number);

	SDL_FreeSurface(surf);
	surf = TTF_RenderText_Blended(font, message.c_str(), color);

	SDL_DestroyTexture(tex);
	tex = SDL_CreateTextureFromSurface(rend, surf);
}


