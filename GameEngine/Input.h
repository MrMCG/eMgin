#pragma once
#include "StdAfx.h"

/* basic Key states */ 
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

/* use SDL keys */
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

// key structure stored in CInput
struct KEYPress
{
	CInput_Event* key;
	int index;
	string name;
};

/* input class stores all created keys and updates their state
* based on poll calls */
class CInput
{
public:
	CInput();
	~CInput(void);

	bool Add(CInput_Event* evnt, string name="");

	CInput_Event* GetKey(int index) const;
	CInput_Event* GetKey(string name) const;
	void Poll(SDL_Event* mainEvent);

	int GetMouseX() const {return mouseX;};
	int GetMouseY() const {return mouseY;};
	bool GetMouseClick() const {return mouseClick;};

private:
	vector<KEYPress>* keys;

	// mouse info
	int mouseX;
	int mouseY;
	bool mouseClick;
};

