#pragma once
#include "StdAfx.h"
#include "Entity.h"

class CPlayer : public CEntity
{
public:
	CPlayer(void) 
	{facing = true;};
	CPlayer(const int x, const int y, const int w, const int h) : CEntity(x,y,w,h)
	{facing = true;};
	~CPlayer(void){};

	void MoveLeft(int force);
	void MoveRight(int force);
	void MoveDown(int force);
	void MoveUp(int force);

	void Jump();
private:
	bool facing;
};

class CBullet : public CEntity
{
public:
	CBullet(void) 
	{isActive = false;};
	CBullet(const int x, const int y, const int w, const int h) : CEntity(x,y,w,h)
	{isActive = false;};
	~CBullet(void){};

	void Fire(b2Body* origin, b2Vec2 destination);
private:
	bool isActive;
};

