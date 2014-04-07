#pragma once
#include "StdAfx.h"

class CInput_Event
{
protected:
	bool ENABLED;

public:
	CInput_Event() {ENABLED=false;};
	virtual ~CInput_Event(void) {};

	virtual bool IsEnabled() const {return ENABLED;};
	
	virtual void Enable() {ENABLED = true;};
	virtual void Disable() {ENABLED = false;};

	virtual void SetKey(SDL_Keycode key) = 0;
	virtual SDL_Keycode GetKey() const = 0;
	
};

