#include "StdAfx.h"
#include "Debug.h"
#include "settings.h"

using namespace settings;

CDebug::CDebug(SDL_Renderer* renderer, string level)
{
	levelName = new CWriting(10,(SCREEN_HEIGHT/30)*0,150,SCREEN_HEIGHT/32);
	levelName->Print(renderer, level);

	string message = SDL_GetVideoDriver(0);
	info = new CWriting(10,(SCREEN_HEIGHT/30)*1,message.size()*15,SCREEN_HEIGHT/32);
	info->Print(renderer, message);

	frames = new CWriting(10,(SCREEN_HEIGHT/30)*2,150,SCREEN_HEIGHT/32);
}

CDebug::~CDebug(void)
{
	delete frames;
	delete info;
	delete levelName;
}

void CDebug::Run(SDL_Renderer* renderer)
{
	info->Draw(renderer);
	levelName->Draw(renderer);
	frames->Draw(renderer);
}

void CDebug::UpdateFrames(SDL_Renderer* renderer, string msg)
{
	frames->Print(renderer, msg);
}


