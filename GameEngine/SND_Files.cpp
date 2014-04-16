#include "SND_Files.h"
#include "settings.h"

using namespace settings;

// ----------------------------------------------------------
// -------------------------- SOUNDS ------------------------
// ----------------------------------------------------------
CSND_Files::CSND_Files(int num)
{
	printf("CREATING - CSND - BEGIN \n");
	size = num+1;
	sounds = new Mix_Chunk*[size];
	store = new string[size];

	if(!(INIT_SND()))
	{
		printf("ERROR: CSND_Files - INIT FAIL - %s\n", SDL_GetError());
		system("pause");
	} else
	{
		printf("CREATING - CSND - SUCCESS \n");
	}	
}

CSND_Files::~CSND_Files(void)
{
	for (int i = 0; i < size; i++)
	{
		if (store[i] != "")
		{
			Mix_FreeChunk(sounds[i]);
		}
	}
	delete [] sounds;
	delete [] store;
}

void CSND_Files::AddSND(string file_path)
{
	string path = SOUND_LOCATION + file_path;
	printf("CSND_AddSND - \t\t%s\n", path.c_str());

	int add = SearchEmpty();
	if (add == -1)
	{
		printf("ERROR: CSND_Files - CSND_FILE_IOOB \n");
	} else
	{
		sounds[add] = Mix_LoadWAV(path.c_str());

		if (sounds[add] == NULL)
		{
			printf("ERROR: CSND_Files - %s\n", SDL_GetError());
			sounds[add] = sounds[0];
			store[add] = store[0];
		} else
		{
			store[add] = file_path;
		}
	}
}

bool CSND_Files::INIT_SND()
{
	string path = SOUND_LOCATION + "debug/error.wav";

	printf("CSND_AddSND_INIT - \t%s\n", path.c_str());

	for (int i = 0; i < size; i++)
	{
		store[i] = "";
		sounds[i] = NULL;
	}

	Mix_Chunk* error = Mix_LoadWAV(path.c_str());
	sounds[0] = error;

	if (sounds[0] == NULL)
	{
		printf("ERROR: CSND_Files - %s\n", SDL_GetError());
		return false;
	} 
	
	error = NULL;
	Mix_FreeChunk(error);

	store[0] = "error.wav";

	return true;
}

Mix_Chunk* CSND_Files::GetSND(string file_name) const
{
	return GetSND(Search(file_name));
}

Mix_Chunk* CSND_Files::GetSND(int index) const
{
	if (index > size || sounds[index] == NULL || index == -1)
	{
		printf("ERROR: CSND_Files - resource NULL %s\n", index);
		printf("ERROR: CSND_Files - using error sound \n");
		return sounds[0];
	} else
	{
		return sounds[index];
	}
}

int CSND_Files::SearchEmpty()
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

int CSND_Files::Search(string file_path) const
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

void CSND_Files::print()
{
	for (int i = 0; i < size; i++)
	{
		cout << "SND : " << i << " - " << store[i] << endl;
	}
}

// ----------------------------------------------------------
// -------------------------- MUSIC -------------------------
// ----------------------------------------------------------
CMSC_Files::CMSC_Files(int num)
{
	printf("CREATING - CMSC - BEGIN \n");
	size = num+1;
	music = new Mix_Music*[size];
	store = new string[size];

	if(!(INIT_MSC()))
	{
		printf("ERROR: CMSC_Files - INIT FAIL - %s\n", SDL_GetError());
		system("pause");
	} else
	{
		printf("CREATING - CMSC - SUCCESS \n");
	}	
}

CMSC_Files::~CMSC_Files(void)
{
	for (int i = 0; i < size; i++)
	{
		if (store[i] != "")
		{
			Mix_FreeMusic(music[i]);
		}
	}
	delete [] music;
	delete [] store;
}

void CMSC_Files::AddMSC(string file_path)
{
	string path = MUSIC_LOCATION + file_path;
	printf("CSND_AddMSC - \t\t%s\n", path.c_str());

	int add = SearchEmpty();
	if (add == -1)
	{
		printf("ERROR: CMSC_Files - CSND_FILE_IOOB \n");
	} else
	{
		music[add] = Mix_LoadMUS(path.c_str());

		if (music[add] == NULL)
		{
			printf("ERROR: CMSC_Files - %s\n", SDL_GetError());
			music[add] = music[0];
			store[add] = store[0];
		} else
		{
			store[add] = file_path;
		}
	}
}

bool CMSC_Files::INIT_MSC()
{
	string path = SOUND_LOCATION + "debug/error.wav";

	printf("CMSC_AddMSC_INIT - \t%s\n", path.c_str());

	for (int i = 0; i < size; i++)
	{
		store[i] = "";
		music[i] = NULL;
	}

	Mix_Music* error = Mix_LoadMUS(path.c_str());
	music[0] = error;

	if (music[0] == NULL)
	{
		printf("ERROR: CMSC_Files - %s\n", SDL_GetError());
		return false;
	} 
	
	error = NULL;
	Mix_FreeMusic(error);

	store[0] = "error.wav";

	return true;
}

Mix_Music* CMSC_Files::GetMSC(string file_name) const
{
	return GetMSC(Search(file_name));
}

Mix_Music* CMSC_Files::GetMSC(int index) const
{
	if (index > size || music[index] == NULL || index == -1)
	{
		printf("ERROR: CMSC_Files - resource NULL %s\n", index);
		printf("ERROR: CMSC_Files - using error sound \n");
		return music[0];
	} else
	{
		return music[index];
	}
}

int CMSC_Files::SearchEmpty()
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

int CMSC_Files::Search(string file_path) const
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

void CMSC_Files::print()
{
	for (int i = 0; i < size; i++)
	{
		cout << "MSC : " << i << " - " << store[i] << endl;
	}
}