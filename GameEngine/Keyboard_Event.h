#pragma once
#include "stdafx.h"
#include "Input_Event.h"

class CKeyboard_Event : public CInput_Event
{
protected:
	SDL_Keycode KEY;
public:
	CKeyboard_Event(SDL_Keycode key) {KEY=key;};
	virtual ~CKeyboard_Event(void) {};

	void SetKey(SDL_Keycode key) {KEY = key;};
	SDL_Keycode GetKey() const {return KEY;};
	
};