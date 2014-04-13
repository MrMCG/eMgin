#include "SDL_Setup.h"
#include "settings.h"

using namespace settings;

CSDL_Setup::CSDL_Setup()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	window = NULL;
	window = SDL_CreateWindow("eMgin",100 , 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("ERROR: SDL_Setup WINDOW - %s\n", SDL_GetError());
		system("pause");
	}

	renderer = NULL;

	Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 );

	mainEvent = new SDL_Event();

	if (TTF_Init() == -1)
	{
		printf("ERROR: SDL_Setup TTF - %s\n", TTF_GetError());
		system("pause");
	}
}

CSDL_Setup::~CSDL_Setup(void)
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	delete mainEvent;
	Mix_CloseAudio();
	SDL_Quit();
	TTF_Quit();
}

SDL_Renderer* CSDL_Setup::GetRenderer() const
{
	return renderer;
}

SDL_Event* CSDL_Setup::GetMainEvent() const
{
	return mainEvent;
}

void CSDL_Setup::SetRenderer(int flags)
{
	if(renderer != NULL)
	{
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}

	switch (flags)
	{
	case 1: renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		break;
	case 2: renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		break;
	default: renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		break;
	}

	if (renderer == NULL)
	{
		printf("ERROR: SDL_Setup RENDERER - %s\n", SDL_GetError());
		system("pause");
	}
}

void CSDL_Setup::Begin()
{
	SDL_RenderClear(renderer);
	SDL_PollEvent(mainEvent);
}

void CSDL_Setup::End()
{
	SDL_RenderPresent(renderer);
}