#pragma once
#include "stdafx.h"
	
// This is a singleton file system for images (textures)
// Files are loaded in here and are refrenced to by other objects

class CIMG_Files
{
public:
	CIMG_Files(SDL_Renderer* pass_renderer, int num);
	~CIMG_Files(void);

	void AddTEX(SDL_Renderer* pass_renderer, string file_path);
	bool INIT_TEX(SDL_Renderer* pass_renderer);

	SDL_Texture* GetTEX(string file_name) const;
	SDL_Texture* GetTEX(int index) const;

	int SearchEmpty();
	int Search(string file_path) const;

	void print();

private:
	SDL_Texture** texs;
	string* store;
	unsigned short size;
};

