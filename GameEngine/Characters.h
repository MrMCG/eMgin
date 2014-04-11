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

	void Jump(int force);
private:
	bool facing;
};

class CBullet : public CEntity
{
public:
	CBullet(void) 
	{isActive = false;rayNumber=20;};
	CBullet(const int x, const int y, const int w, const int h) : CEntity(x,y,w,h)
	{isActive = false;rayNumber=20;};
	~CBullet(void){};

	inline void SetRays(int num){rayNumber = num;};

	void Fire(b2Body* origin, b2Vec2 destination);
	void Explode(b2World* world, float radius, float force);
private:
	bool isActive;
	int rayNumber;
};

