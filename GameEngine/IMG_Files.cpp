#include "IMG_Files.h"
#include "settings.h"

using namespace settings;

CIMG_Files::CIMG_Files(SDL_Renderer* pass_renderer, int num)
{
	printf("CREATING - CIMG - BEGIN \n");
	size = num+1;
	texs = new SDL_Texture*[size];
	store = new string[size];

	if(!(INIT_TEX(pass_renderer)))
	{
		printf("ERROR: CIMG_Files - INIT FAIL - %s\n", SDL_GetError());
		system("pause");
	} else
	{
		printf("CREATING - CIMG - SUCCESS \n");
	}	
}

CIMG_Files::~CIMG_Files(void)
{
	for (int i = 0; i < size; i++)
	{
		if (store[i] != "")
		{
			SDL_DestroyTexture(texs[i]);
		}
	}

	delete [] texs;
	delete [] store;
}

void CIMG_Files::AddTEX(SDL_Renderer* pass_renderer, string file_path)
{
	string path = IMAGE_LOCATION + file_path;

	printf("CIMG_AddTEX - \t\t%s\n", path.c_str());

	int add = SearchEmpty();
	if (add == -1)
	{
		printf("ERROR: CIMG_Files - CIMG_FILE_IOOB %s\n");
	} else
	{
		texs[add] = IMG_LoadTexture(pass_renderer, path.c_str());

		if (texs[add] == NULL)
		{
			printf("ERROR: CIMG_Files - %s\n", SDL_GetError());
			texs[add] = texs[0];
			store[add] = store[0];
		} else
		{
			store[add] = file_path;
		}
	}
}

bool CIMG_Files::INIT_TEX(SDL_Renderer* pass_renderer)
{
	string path = IMAGE_LOCATION + "debug/error.png";

	printf("CIMG_AddTEX_INIT - \t%s\n", path.c_str());

	for (int i = 0; i < size; i++)
	{
		store[i] = "";
		texs[i] = NULL;
	}

	SDL_Texture* error = NULL;	
	error = IMG_LoadTexture(pass_renderer, path.c_str());

	if (error == NULL)
	{
		printf("ERROR: CIMG_Files - INIT FAIL - %s\n", SDL_GetError());
		error = SDL_CreateTexture(pass_renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, 50, 50);
	}

	texs[0] = error;

	if (texs[0] == NULL)
	{
		printf("ERROR: CIMG_Files - %s\n", SDL_GetError());
		return false;
	} 

	error = NULL;
	SDL_DestroyTexture(error);
	
	store[0] = "error.png";

	return true;
}

SDL_Texture* CIMG_Files::GetTEX(string file_name) const
{
	return GetTEX(Search(file_name));
}

SDL_Texture* CIMG_Files::GetTEX(int index) const 
{
	// if incorrect index/texture return error tex
	if (index > size || texs[index] == NULL || index == -1)
	{
		printf("ERROR: CIMG_Files - resource NULL %i\n", index);
		printf("ERROR: CIMG_Files - using error tex \n");
		return texs[0];
	} else
	{
		return texs[index];
	}
}

int CIMG_Files::SearchEmpty()
{
	for (int i = 0; i < size; i++)
	{
		if (store[i] == "")
		{
			return i;
		}
	}

	return -1;
}

int CIMG_Files::Search(string file_path) const
{
	for (int i = 0; i < size; i++)
	{
		if (store[i] == file_path)
		{
			return i;
		}
	}

	return -1;
}

void CIMG_Files::print()
{
	for (int i = 0; i < size; i++)
	{
		cout << "IMG : " << i << " - " << store[i] << endl;
	}
}

