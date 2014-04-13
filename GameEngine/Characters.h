#pragma once
#include "StdAfx.h"
#include "Entity.h"

class CPlayer : public CEntity
{
public:
	CPlayer() {facing=3;falling=false;jumping=false;};
	CPlayer(SDL_Renderer* pass_renderer, CResources* resources, b2World* world);
	CPlayer(SDL_Renderer* pass_renderer, 
			CResources* resources, 
			b2World* world, 
			const int x, const int y, const int w, const int h);

	~CPlayer(void){};

	void MoveLeft(int force);
	void MoveRight(int force);
	void MoveDown(int force);
	void MoveUp(int force);
	void Jump(int force);

	void UpdatePosition();
	void UpdateState();

	inline int GetFacing() const {return facing;};
	inline bool GetFalling() const {return falling;};

	

private:
	int facing;
	bool falling;
	bool jumping;
};

class CBullet : public CEntity
{
public:
	CBullet() {rayNumber=32;amountActive=0;};
	CBullet(CResources* resources); 
	CBullet(CResources* resources, const int x, const int y, const int w, const int h);

	~CBullet(void){};

	inline void SetRays(int num){rayNumber = num;};
	inline bool IsActive() const {return amountActive;};

	void Fire(b2World* world, CPlayer* player, b2Vec2 destination);
	void Fire(b2World* world, CPlayer* player, bool fire2Direction=true);

	void Explode(b2World* world, float radius, float force);
private:
	int rayNumber;
	int amountActive;
};

class CBackground : public CEntity
{
public:
	CBackground(void) {};
	CBackground(CResources* resources);
	CBackground(CResources* resources, const int x, const int y, const int w, const int h);

	~CBackground(void){};
};

/*
class CCrate : public CEntity
{
public:
	CBackground(void) {};
	CBackground(CResources* resources);
	CBackground(CResources* resources, const int x, const int y, const int w, const int h);

	~CBackground(void){};
};
*/
