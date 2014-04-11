#pragma once
#include "IMG_Files.h"
#include "SND_Files.h"

class CResources
{
public:
	CResources();
	CResources(SDL_Renderer* renderer);
	~CResources(void);

	SDL_Texture* GetTexResources(int index) const {return TEX_resources->GetTEX(index);};
	Mix_Chunk* GetSoundResources(int index) const {return SND_resources->GetSND(index);};

private:
	CIMG_Files* TEX_resources;
	CSND_Files* SND_resources;
};

