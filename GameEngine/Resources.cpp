#include "StdAfx.h"
#include "Resources.h"

CResources::CResources()
{
	TEX_resources = NULL;

	SND_resources = NULL;
}

CResources::CResources(SDL_Renderer* renderer)
{
	TEX_resources = new CIMG_Files(renderer, 8);
	
	TEX_resources->AddTEX(renderer, "player.png"); // 1 = player sprite
	TEX_resources->AddTEX(renderer, "landscape.bmp"); // 2 = background
	TEX_resources->AddTEX(renderer, "bullet.png"); // 3 = bullet tex
	TEX_resources->AddTEX(renderer, "metalCrate.png"); // 4 = crate Tex
	TEX_resources->AddTEX(renderer, "sheetMetal.png"); // 5 = floor tex
	TEX_resources->AddTEX(renderer, "debug/tileSize.png"); // 6 = tile debug tex
	TEX_resources->AddTEX(renderer, "backdrop.png"); // 7 = writing backdrop 1
	TEX_resources->AddTEX(renderer, "enemy.png"); // 8 = enemy tex
	

	SND_resources = new CSND_Files(4);

	SND_resources->AddSND("explosion.wav"); // 1 = explosion
	SND_resources->AddSND("laser.wav"); // 2 = laser
	SND_resources->AddSND("thruster.wav"); // 3 = thruster

	MSC_resources = new CMSC_Files(1);

	MSC_resources->AddMSC("level1.mp3"); // 1 = level1 music
}


CResources::~CResources(void)
{
	delete TEX_resources;
	delete SND_resources;
}

void CResources::PlayMusic(int index, int loops)
{
	if (Mix_PlayingMusic() != 0)
	{
		StopMusic();
	} 

	Mix_PlayMusic(MSC_resources->GetMSC(index), loops);
}

void CResources::StopMusic()
{
	Mix_HaltMusic();
}

void CResources::PauseMusic()
{
	Mix_PauseMusic();
}

void CResources::StopSound(int channel)
{
	Mix_HaltChannel(channel);
}

void CResources::ResumeMusic()
{
	if (Mix_PausedMusic() == 1)
	{
		Mix_ResumeMusic();
	}
}

int CResources::PlaySound(int index, int loops, int channel)
{
	return Mix_PlayChannel(channel, SND_resources->GetSND(index), loops);
}
	
