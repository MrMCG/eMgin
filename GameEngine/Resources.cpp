#include "StdAfx.h"
#include "Resources.h"
#include "settings.h"

using namespace settings;

// ----------------------------------------------------------
// ---------------------- Resources -------------------------
// ----------------------------------------------------------
CResources::CResources(SDL_Renderer* renderer)
{
	TEX_resources = new CIMG_Files(renderer);
	SND_resources = new CSND_Files();
	MSC_resources = new CMSC_Files();
}


CResources::~CResources(void)
{
	delete TEX_resources;
	delete SND_resources;
	delete MSC_resources;
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

// ----------------------------------------------------------
// ------------------------- IMG_Files ----------------------
// ----------------------------------------------------------
CIMG_Files::CIMG_Files(SDL_Renderer* pass_renderer)
{
	printf("CREATING - CIMG - BEGIN \n");
	textures = new vector<IMGFile>;

	if(!(INIT_TEX(pass_renderer)))
	{
		printf("ERROR: CIMG_Files - INIT FAIL");
		system("pause");
	} else
	{
		printf("CREATING - CIMG - SUCCESS \n");
	}
}

CIMG_Files::~CIMG_Files(void)
{
	for (vector<IMGFile>::iterator it = textures->begin(); it < textures->end(); it++)
	{	
		SDL_DestroyTexture((*it).file);
	}

	textures->clear();
	delete textures;
}

bool CIMG_Files::Add(SDL_Renderer* pass_renderer, string file_path)
{
	string path = IMAGE_LOCATION + file_path;
	printf("CSND_AddTEX - \t\t%s\n", path.c_str());

	bool success = true;
	IMGFile file;
	file.file = IMG_LoadTexture(pass_renderer, path.c_str());
	if (file.file == NULL)
	{
		printf("ERROR: CIMG_Files - %s\n", SDL_GetError());
		file.file = textures->front().file;
		success = false;
	} 

	file.path = file_path;
	textures->push_back(file);
	textures->back().index = textures->size()-1;

	return success;
}

bool CIMG_Files::Add(SDL_Renderer* pass_renderer, SDL_Texture* tex, string name)
{
	string path = IMAGE_LOCATION + name;
	printf("CSND_AddTEX - \t\t%s\n", path.c_str());

	bool success = true;
	IMGFile file;
	file.file = tex;
	if (file.file == NULL)
	{
		printf("ERROR: CIMG_Files - %s\n", SDL_GetError());
		file.file = textures->front().file;
		success = false;
	} 

	file.path = name;
	textures->push_back(file);
	textures->back().index = textures->size()-1;

	return success;
}

bool CIMG_Files::INIT_TEX(SDL_Renderer* pass_renderer)
{
	string path = "debug/error.png";

	if (!Add(pass_renderer, path))
	{
		printf("ERROR: CIMG_Files - INIT FAIL - %s\n", SDL_GetError());
		SDL_Texture* error = SDL_CreateTexture(pass_renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, 50, 50);
		return Add(pass_renderer, error, "error.png");
	} 

	return true;
}

SDL_Texture* CIMG_Files::GetTEX(string file_name) const
{
	for (vector<IMGFile>::iterator it = textures->begin(); it < textures->end(); it++)
	{	
		if ((*it).path == file_name)
		{
			return (*it).file;
		}
	}
	printf("ERROR: CIMG_Files - FILE_NOT_FOUND -%s", file_name);
	return textures->front().file;
}

SDL_Texture* CIMG_Files::GetTEX(int index) const 
{
	for (vector<IMGFile>::iterator it = textures->begin(); it < textures->end(); it++)
	{	
		if ((*it).index == index)
		{
			return (*it).file;
		}
	}
	printf("ERROR: CIMG_Files - FILE_NOT_FOUND -%s", index);
	return textures->front().file;
}

// ----------------------------------------------------------
// -------------------------- SOUNDS ------------------------
// ----------------------------------------------------------
CSND_Files::CSND_Files()
{
	printf("CREATING - CSND - BEGIN \n");
	sounds = new vector<SNDFile>;

	if(!(INIT_SND()))
	{
		printf("ERROR: CSND_Files - INIT FAIL");
		system("pause");
	} else
	{
		printf("CREATING - CSND - SUCCESS \n");
	}
}

CSND_Files::~CSND_Files(void)
{
	for (vector<SNDFile>::iterator it = sounds->begin(); it < sounds->end(); it++)
	{	
		Mix_FreeChunk((*it).file);
	}

	sounds->clear();
	delete sounds;
}

bool CSND_Files::Add(string file_path)
{
	string path = SOUND_LOCATION + file_path;
	printf("CSND_AddSND - \t\t%s\n", path.c_str());

	bool success = true;
	SNDFile file;
	file.file = Mix_LoadWAV(path.c_str());
	if (file.file == NULL)
	{
		printf("ERROR: CSND_Files - %s\n", SDL_GetError());
		file.file = sounds->front().file;
		success = false;
	} 

	file.path = file_path;
	sounds->push_back(file);
	sounds->back().index = sounds->size()-1;

	return success;
}

bool CSND_Files::INIT_SND()
{
	string path = "debug/error.wav";

	return Add(path);
}

Mix_Chunk* CSND_Files::GetSND(string file_name) const
{
	for (vector<SNDFile>::iterator it = sounds->begin(); it < sounds->end(); it++)
	{	
		if ((*it).path == file_name)
		{
			return (*it).file;
		}
	}
	printf("ERROR: CSND_Files - FILE_NOT_FOUND -%s", file_name);
	return sounds->front().file;
}

Mix_Chunk* CSND_Files::GetSND(int index) const
{
	for (vector<SNDFile>::iterator it = sounds->begin(); it < sounds->end(); it++)
	{	
		if ((*it).index == index)
		{
			return (*it).file;
		}
	}
	printf("ERROR: CMSC_Files - FILE_NOT_FOUND -%d", index);
	return sounds->front().file;
}

void CSND_Files::SetVolume(int index, int vol)
{
	Mix_VolumeChunk(GetSND(index), vol);
}

void CSND_Files::SetVolume(string file_name, int vol)
{
	Mix_VolumeChunk(GetSND(file_name), vol);
}

// ----------------------------------------------------------
// -------------------------- MUSIC -------------------------
// ----------------------------------------------------------
CMSC_Files::CMSC_Files()
{
	printf("CREATING - CMSC - BEGIN \n");
	music = new vector<MSCFile>;

	if(!(INIT_MSC()))
	{
		printf("ERROR: CMSC_Files - INIT FAIL");
		system("pause");
	} else
	{
		printf("CREATING - CMSC - SUCCESS \n");
	}	
}

CMSC_Files::~CMSC_Files(void)
{
	for (vector<MSCFile>::iterator it = music->begin(); it < music->end(); it++)
	{	
		Mix_FreeMusic((*it).file);
	}

	music->clear();
	delete music;
}

bool CMSC_Files::Add(string file_path)
{
	string path = MUSIC_LOCATION + file_path;
	printf("CMSC_AddMSC - \t\t%s\n", path.c_str());

	bool success = true;
	MSCFile file;
	file.file = Mix_LoadMUS(path.c_str());
	if (file.file == NULL)
	{
		printf("ERROR: CMSC_Files - %s\n", SDL_GetError());
		file.file = music->front().file;
		success = false;
	} 

	file.path = file_path;
	music->push_back(file);
	music->back().index = music->size()-1;

	return success;
}

bool CMSC_Files::INIT_MSC()
{
	string path = "debug/error.wav";

	return Add(path);
}

Mix_Music* CMSC_Files::GetMSC(string file_name) const
{
	for (vector<MSCFile>::iterator it = music->begin(); it < music->end(); it++)
	{	
		if ((*it).path == file_name)
		{
			return (*it).file;
		}
	}
	printf("ERROR: CMSC_Files - FILE_NOT_FOUND -%s", file_name);
	return music->front().file;
}

Mix_Music* CMSC_Files::GetMSC(int index) const
{
	for (vector<MSCFile>::iterator it = music->begin(); it < music->end(); it++)
	{	
		if ((*it).index == index)
		{
			return (*it).file;
		}
	}
	printf("ERROR: CMSC_Files - FILE_NOT_FOUND -%d", index);
	return music->front().file;
}
