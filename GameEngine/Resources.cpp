#include "StdAfx.h"
#include "Resources.h"

CResources::CResources()
{
	TEX_resources = NULL;

	SND_resources = NULL;
}

CResources::CResources(SDL_Renderer* renderer)
{
	TEX_resources = new CIMG_Files(renderer, 6);
	
	TEX_resources->AddTEX(renderer, "player.png"); // 1 = player sprite
	TEX_resources->AddTEX(renderer, "landscape.bmp"); // 2 = background
	TEX_resources->AddTEX(renderer, "bullet.png"); // 3 = bullet tex
	TEX_resources->AddTEX(renderer, "metalCrate.png"); // 4 = crate Tex
	TEX_resources->AddTEX(renderer, "sheetMetal.png"); 
	TEX_resources->AddTEX(renderer, "sprite.png");
	

	SND_resources = new CSND_Files(4);

	SND_resources->AddSND("good.wav");
}


CResources::~CResources(void)
{
	delete TEX_resources;
	delete SND_resources;
}
