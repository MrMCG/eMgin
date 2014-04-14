#pragma once
#include "StdAfx.h"
#include "Entity.h"

class CPlayer : public CEntity
{
public:
	CPlayer() {InitVar();};
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

	inline bool GetIdle() const {return idle;};
	inline void SetIdle(bool flag) {idle=flag;};

private:
	void InitVar();

	int facing;
	bool falling;
	bool jumping;
	bool idle;
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
	void InitVar();

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

class CTile : public CEntity
{
public:
	CTile(void) {debug=new CSprite();};
	CTile(CResources* resources);
	CTile(CResources* resources, const int x, const int y);
	CTile(CResources* resources, const int x, const int y, const int index);

	void DrawDebug(SDL_Renderer* pass_renderer);

	~CTile(void){delete debug;};
private:
	CSprite* debug;
};

class CBoundry : public CEntity
{
public:
	CBoundry();
	CBoundry(CResources* resources, b2World* world);

	void Draw(SDL_Renderer* pass_renderer);
	void SetBoundry(b2World* world, int x, int y, int w, int h, int size); 

	inline CEntity* GetLeft() {return leftSide;};
	inline CEntity* GetRight() {return rightSide;};
	inline CEntity* Gettop() {return topSide;};
	inline CEntity* GetBottom() {return bottomSide;};

	~CBoundry(void){delete leftSide;delete rightSide; delete topSide; delete bottomSide;};
private:
	CEntity* leftSide;
	CEntity* rightSide;
	CEntity* topSide;
	CEntity* bottomSide;
};

class CWriting : public CEntity
{
public:
	CWriting();
	CWriting(int x, int y, int w, int h);

	inline void Print(SDL_Renderer* rend, string message) {sprite->Print(rend, message);};
	inline void Print(SDL_Renderer* rend, int number) {sprite->Print(rend, number);};
	inline void SetColor(int r, int g, int b) {sprite->SetColor(r,g,b);};
	inline void SetOpacity(int a) {sprite->SetOpacity(a);};
	inline CSprite* GetBackdrop() {return backdrop;};

	~CWriting(){delete backdrop;};

private:
	CSprite* backdrop;
};

class CCrate : public CEntity
{
public:
	CCrate(CResources* resources, b2World* world, int x, int y, int w, int h);


	~CCrate(){};
};
/*
class CFloor : public CEntity
{
public:
	CCrate(CResources* resources, b2World* world, int x, int y, int w, int h);


	~CCrate(){};
};
*/