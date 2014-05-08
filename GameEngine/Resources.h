#pragma once
#include "stdafx.h"

/* resources follow singleton pattern
* loaded once into memory and refrenced in game */

struct MSCFile
{
	Mix_Music* file;
	int index;
	string path;
};

struct SNDFile
{
	Mix_Chunk* file;
	int index;
	string path;
};

struct IMGFile
{
	SDL_Texture* file;
	int index;
	string path;
};

// image/texture storage
class CIMG_Files
{
public:
	CIMG_Files(SDL_Renderer* pass_renderer);
	~CIMG_Files(void);

	bool Add(SDL_Renderer* pass_renderer, string file_path);
	bool Add(SDL_Renderer* pass_renderer, SDL_Texture* tex, string name);
	bool INIT_TEX(SDL_Renderer* pass_renderer);

	SDL_Texture* GetTEX(string file_name) const;
	SDL_Texture* GetTEX(int index) const;

private:
	vector<IMGFile>* textures;
};

// sound files
class CSND_Files
{
public:
	CSND_Files();
	~CSND_Files(void);

	bool Add(string file_path);
	bool INIT_SND();

	Mix_Chunk* GetSND(string file_name) const;
	Mix_Chunk* GetSND(int index) const;

	void SetVolume(int index, int vol);
	void SetVolume(string file_name, int vol);

private: 
	vector<SNDFile>* sounds;
};

// music files
class CMSC_Files
{
public:
	CMSC_Files();
	~CMSC_Files(void);

	bool Add(string file_path);
	bool INIT_MSC();

	Mix_Music* GetMSC(string file_name) const;
	Mix_Music* GetMSC(int index) const;

	inline void SetVolume(int vol) {Mix_VolumeMusic(vol);};

private: 
	vector<MSCFile>* music;
};

// group all resources together
class CResources
{
public:
	CResources(SDL_Renderer* renderer);
	~CResources(void);


	inline bool AddTEX(SDL_Renderer* pass_renderer, string file_path) {return TEX_resources->Add(pass_renderer, file_path);};
	inline bool AddTEX(SDL_Renderer* pass_renderer, SDL_Texture* tex, string name) {return TEX_resources->Add(pass_renderer, tex, name);};
	inline bool AddMSC(string file_path) {return MSC_resources->Add(file_path);};
	inline bool AddSND(string file_path) {return SND_resources->Add(file_path);};

	SDL_Texture* GetTex(int index) const {return TEX_resources->GetTEX(index);};
	Mix_Chunk* GetSound(int index) const {return SND_resources->GetSND(index);};
	Mix_Music* GetMusic(int index) const {return MSC_resources->GetMSC(index);};
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



