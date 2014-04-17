#include "Input.h"

CInput::CInput()
{
	printf("CREATING - CInput - BEGIN \n");
	keys = new vector<KEYPress>;
	printf("CREATING - CInput - SUCCESS \n");
}

CInput::~CInput(void)
{
	keys->clear();
	delete keys;
}

bool CInput::Add(CInput_Event* evnt, string name)
{
	printf("CInput_Add \t%i\n", evnt->GetKey());

	KEYPress file;
	file.key = evnt;
	if (file.key == NULL)
	{
		printf("ERROR: CInput - Event NULL");
		return false;
	} 

	file.name = name;
	keys->push_back(file);
	keys->back().index = keys->size()-1;

	return true;
}

CInput_Event* CInput::GetKey(int index) const
{
	for (vector<KEYPress>::iterator it = keys->begin(); it < keys->end(); it++)
	{	
		if ((*it).index == index)
		{
			return (*it).key;
		}
	}
	printf("ERROR: CMSC_Files - FILE_NOT_FOUND -%d", index);
}

CInput_Event* CInput::GetKey(string name) const
{
	for (vector<KEYPress>::iterator it = keys->begin(); it < keys->end(); it++)
	{	
		if ((*it).name == name)
		{
			return (*it).key;
		}
	}
	printf("ERROR: CMSC_Files - FILE_NOT_FOUND -%d", name);
}

void CInput::Poll(SDL_Event* mainEvent)
{
	SDL_Keycode key = mainEvent->key.keysym.sym;
	
	switch (mainEvent->type)
	{	
	case SDL_KEYDOWN:
	
		for (vector<KEYPress>::iterator it = keys->begin(); it < keys->end(); it++)
		{
			if ((*it).key->GetKey() == key)
			{
				(*it).key->Enable(); 		
				break;
			}
		}
		break;

	case SDL_KEYUP:

		for (vector<KEYPress>::iterator it = keys->begin(); it < keys->end(); it++)
		{
			if ((*it).key->GetKey() == key)
			{
				(*it).key->Disable(); 		
				break;
			}
		}
		break;

	case SDL_MOUSEBUTTONDOWN:
		mouseX = mainEvent->button.x;
		mouseY = mainEvent->button.y;
		mouseClick = true;
		break;
	case SDL_MOUSEBUTTONUP:
		mouseClick = false;
		break;
	//case SDL_MOUSEMOTION:
		//mouseX = mainEvent->button.x;
		//mouseY = mainEvent->button.y;
		//break;
		
	}
}


