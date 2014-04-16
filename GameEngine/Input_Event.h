#pragma once
#include "StdAfx.h"

class CInput_Event
{
protected:
	bool ENABLED;
	bool USED;

public:
	CInput_Event() {ENABLED=false;USED=false;};
	virtual ~CInput_Event(void) {};

	virtual bool IsEnabled() const {return ENABLED;};
	virtual bool IsUsed() const {return USED;};
	
	virtual void Enable() {ENABLED = true;};
	virtual void Disable() {ENABLED = false;};

	virtual void Used() {USED = true;};
	virtual void NotUsed() {USED = false;};

	virtual void SetKey(SDL_Keycode key) = 0;
	virtual SDL_Keycode GetKey() const = 0;
	
};

