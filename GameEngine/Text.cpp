#include "Text.h"

CText::CText()
{
	font = NULL;
	surf = NULL;

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

void CText::Print(SDL_Renderer* rend, string message, int r, int g, int b)
{
	SDL_Color color = {r,g,b};

	SDL_FreeSurface(surf);
	surf = TTF_RenderText_Blended(font, message.c_str(), color);

	SDL_DestroyTexture(tex);
	tex = SDL_CreateTextureFromSurface(rend, surf);
}

void CText::Print(SDL_Renderer* rend, int number, int r, int g, int b)
{
	SDL_Color color = {r,g,b};
	
	string message = to_string((long long)number);

	SDL_FreeSurface(surf);
	surf = TTF_RenderText_Blended(font, message.c_str(), color);

	SDL_DestroyTexture(tex);
	tex = SDL_CreateTextureFromSurface(rend, surf);
}

