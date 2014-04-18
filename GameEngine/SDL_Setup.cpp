#include "SDL_Setup.h"
#include "settings.h"

using namespace settings;

namespace settings
{
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	int SCREEN;

	int TILE_COLUMN_CALC;
	int TILE_ROW_CALC;
	int TILE_COLUMN_CALC_R;
	int TILE_ROW_CALC_R;
	int TILE_PIXEL_SIZE;
	int TILE_PIXEL_METER;
}

CSDL_Setup::CSDL_Setup()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	renderer = NULL;
	window = NULL;

	LoadSettings();

	SetScreen(SCREEN);

	if (window == NULL)
	{
		printf("ERROR: SDL_Setup WINDOW - %s\n", SDL_GetError());
		system("pause");
	}

	Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 );

	mainEvent = new SDL_Event();


	if (TTF_Init() == -1)
	{
		printf("ERROR: SDL_Setup TTF - %s\n", TTF_GetError());
		system("pause");
	}

	// set acceptable resolution
	// above HD
	res[10][0] = 1920;
	res[10][1] = 1080;
	res[9][0] = 1792;
	res[9][1] = 1008;
	res[8][0] = 1664;
	res[8][1] = 936;
	res[7][0] = 1536;
	res[7][1] = 864;
	res[6][0] = 1408;
	res[6][1] = 792;
	res[5][0] = 1280;
	res[5][1] = 720;

	// below HD
	res[4][0] = 1152;
	res[4][1] = 648;
	res[3][0] = 1024;
	res[3][1] = 576;
	res[2][0] = 896;
	res[2][1] = 504;
	res[1][0] = 768;
	res[1][1] = 432;
	res[0][0] = 640;
	res[0][1] = 360;

}

void CSDL_Setup::LoadSettings()
{
	ifstream file(SETTINGS_LOCATION + "settings.txt");
	size_t pos = 0;
	string line;
	string current;

	// line 1 = screen width
	getline(file, line);
	SCREEN_WIDTH = stoi(line);
	// line 1 = screen height
	getline(file, line);
	SCREEN_HEIGHT = stoi(line);
	// line 1 = vsync
	getline(file, line);
	SCREEN = stoi(line);

	CalcSettings();
}

void CSDL_Setup::CalcSettings()
{
	TILE_COLUMN_CALC = SCREEN_WIDTH/TILE_COLUMN;
	TILE_ROW_CALC = SCREEN_HEIGHT/TILE_ROW;
	TILE_COLUMN_CALC_R = SCREEN_WIDTH%TILE_COLUMN;
	TILE_ROW_CALC_R = SCREEN_HEIGHT%TILE_ROW;
	TILE_PIXEL_SIZE = SCREEN_HEIGHT/TILE_ROW;
	TILE_PIXEL_METER = TILE_PIXEL_SIZE/TILE_SCALE;
}

bool CSDL_Setup::ResetScreen()
{
	if (mainEvent->window.event == SDL_WINDOWEVENT_RESIZED)
	{	
		int width = mainEvent->window.data1; 
		int height = mainEvent->window.data2;
		
		for (int i = 1; i < 11; i++)
		{
			if (width <= res[i][0])
			{
				width = res[i-1][0];
				height = res[i-1][1];
				break;
			}		
		}

		SDL_SetWindowSize(window, width, height);
		SCREEN_WIDTH = width;
		SCREEN_HEIGHT = height;
		CalcSettings();
		return true;
	}

	return false;
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

void CSDL_Setup::SetScreen(int flags)
{
	if (window != NULL)
	{
		SDL_DestroyWindow(window);
		window = NULL;
	}

	if (renderer != NULL)
	{
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}

	switch (flags)
	{
	case 0: window = SDL_CreateWindow("eMgin", 100 ,100 , SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			
		break;
	case 1: window = SDL_CreateWindow("eMgin", 100 ,100 , SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			
		break;
	case 2: window = SDL_CreateWindow("eMgin", 100 ,100 , SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			
		break;
	case 3: window = SDL_CreateWindow("eMgin", 100 ,100 , SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			
		break;
	default: window = SDL_CreateWindow("eMgin", 100 ,100 , SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			 
		break;
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