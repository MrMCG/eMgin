#include "Text.h"

CText::CText()
{
	font = NULL;
	surf = NULL;
	color.r = 0;
	color.g = 0;
	color.b = 0;
	color.a = 255;

	font = TTF_OpenFont("SourceSansPro-Regular.ttf", 72);

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

