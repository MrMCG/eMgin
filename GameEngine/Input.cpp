#include "Input.h"

CInput::CInput(int num)
{
	printf("CREATING - CInput - BEGIN \n");

	size = num;
	keys = new CInput_Event*[size];
	store = new Sint32[size];

	INIT_INPUT();

	printf("CREATING - CInput - SUCCESS \n");
}

CInput::~CInput(void)
{
	for (int i = 0; i < size; i++)
	{
		delete keys[i];
	}

	delete [] keys;
	delete store;
}

void CInput::INIT_INPUT()
{
	for (int i = 0; i < size; i++)
	{
		keys[i] = NULL;
		store[i] = -1;
	}
}

void CInput::AddEvent(CInput_Event* evnt)
{
	printf("CInput_AddEvent \t%i\n", evnt->GetKey());

	int add = SearchEmpty();
	if (add == -1)
	{
		printf("ERROR: CInput - CInput_IOOB %s\n");
	} else
	{
		keys[add] = evnt;

		if (keys[add] == NULL)
		{
			printf("ERROR: CInput - Failed to add CInput_Event \n");
		} else
		{
			store[add] = evnt->GetKey();
		}
	}
}

void CInput::RemoveEvent(CInput_Event* evnt)
{
	for (int i = 0; i < size; i++)
	{
		if (store[i] == evnt->GetKey())
		{
			keys[i] = NULL;
			delete keys[i];	
			store[i] = -1;
			break;
		}
	}
}

void CInput::Poll(SDL_Event* mainEvent)
{
	SDL_Keycode key = mainEvent->key.keysym.sym;
	switch (mainEvent->type)
	{	
	case SDL_KEYDOWN:
	
		for (int i = 0; i < size; i++)
		{
			if (store[i] == key)
			{
				keys[i]->Enable(); 
				break;
			}
		}
		break;

	case SDL_KEYUP:

		for (int i = 0; i < size; i++)
		{
			if (store[i] == key)
			{		
				keys[i]->Disable(); 
				break;
			}
		}
		break;
	}

}

int CInput::SearchEmpty()
{
	for (int i = 0; i < size; i++)
	{
		if (store[i] == -1)
		{
			return i;
		}
	}

	return -1;
}

void CInput::print()
{
	for (int i = 0; i < size; i++)
	{
		cout << "INPUT : " << i << " - " << store[i] << endl;
	}
}




