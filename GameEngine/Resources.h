#pragma once
#include "IMG_Files.h"
#include "SND_Files.h"

class CResources
{
public:
	CResources();
	CResources(SDL_Renderer* renderer);
	~CResources(void);

	CIMG_Files* GetTexResources() const {return TEX_resources;};
	CSND_Files* GetSoundResources() const {return SND_resources;};

private:
	CIMG_Files* TEX_resources;
	CSND_Files* SND_resources;
};

