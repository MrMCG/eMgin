#pragma once
#include "StdAfx.h"
#include "Input_Event.h"

class CInput
{
public:
	CInput(int num);
	~CInput(void);

	void INIT_INPUT();
	void AddEvent(CInput_Event* evnt);
	void RemoveEvent(CInput_Event* evnt);
	//void UpdateEvent(CInput_Event evnt);

	CInput_Event* GetKey(int num) {return keys[num];};
	void Poll(SDL_Event* mainEvent);

	int SearchEmpty();
	void print();

private:
	CInput_Event** keys;
	Sint32* store;
	unsigned short size;
};

