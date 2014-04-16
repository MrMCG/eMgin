#pragma once
#include "stdafx.h"

// This is a singleton file system for sounds
// Files are loaded in here and are refrenced to by other objects

class CSND_Files
{
public:
	CSND_Files(int num);
	~CSND_Files(void);

	void AddSND(string file_path);
	bool INIT_SND();

	Mix_Chunk* GetSND(string file_name) const;
	Mix_Chunk* GetSND(int index) const;

	inline void SetVolume(int index, int vol) {Mix_VolumeChunk(GetSND(index), vol);};

	int SearchEmpty();
	int Search(string file_path) const;

	void print();

private: 
	Mix_Chunk** sounds;
	string* store;
	unsigned short size;
};

class CMSC_Files
{
public:
	CMSC_Files(int num);
	~CMSC_Files(void);

	void AddMSC(string file_path);
	bool INIT_MSC();

	Mix_Music* GetMSC(string file_name) const;
	Mix_Music* GetMSC(int index) const;

	inline void SetVolume(int vol) {Mix_VolumeMusic(vol);};

	int SearchEmpty();
	int Search(string file_path) const;

	void print();

private: 
	Mix_Music** music;
	string* store;
	unsigned short size;
};
