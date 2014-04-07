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

	int SearchEmpty();
	int Search(string file_path) const;

	void print();

private: 
	Mix_Chunk** sounds;
	string* store;
	unsigned short size;
};

