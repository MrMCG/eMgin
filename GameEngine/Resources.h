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
	Mix_Music* GetMusicResources(int index) const {return MSC_resources->GetMSC(index);};
	CIMG_Files* GetTexResources() const {return TEX_resources;};
	CSND_Files* GetSoundResources() const {return SND_resources;};
	CMSC_Files* GetMusicResources() const {return MSC_resources;};

	// if loop = -1, music plays until manually stopped
	void PlayMusic(int index, int loop);
	void PauseMusic();
	void ResumeMusic();
	void StopMusic();

	int PlaySound(int index, int loops, int channel=-1);
	void StopSound(int channel);

private:
	CIMG_Files* TEX_resources;
	CSND_Files* SND_resources;
	CMSC_Files* MSC_resources;
};

