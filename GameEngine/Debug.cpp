#include "StdAfx.h"
#include "Debug.h"
#include "settings.h"

using namespace settings;

CDebug::CDebug(SDL_Renderer* renderer)
{
	videoCard = new CEntity();
	string message = SDL_GetVideoDriver(0);
	videoCard->SetAll(10,(SCREEN_HEIGHT/30)*0,message.size()*15,SCREEN_HEIGHT/32);
	videoCard->ADD_Text(new CText());
	
	videoCard->GetSprite()->Print(renderer, message);

	Frames = new CEntity();
	Frames->SetAll(10,(SCREEN_HEIGHT/30)*1,150,SCREEN_HEIGHT/32);
	Frames->ADD_Text(new CText());
}

CDebug::~CDebug(void)
{
	delete Frames;
	delete videoCard;
}

void CDebug::Run(SDL_Renderer* renderer)
{
	videoCard->Draw(renderer);
	
	Frames->Draw(renderer);
}

void CDebug::UpdateFrames(SDL_Renderer* renderer, string msg)
{
	Frames->GetSprite()->Print(renderer, msg);
}


