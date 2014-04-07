#pragma once
#include "stdafx.h"
#include "Sprite.h"
#include "Animate.h"
#include "Text.h"
#include "Vector2D.h"
#include "Physics.h"
#include "Collision.h"

// Every "object" in the game is an entity
// The entity has components which define the Entity in the game
// Each component has its own attributes

class CEntity
{
public:

	CEntity(void);

	// Create Entity, given position and size
	CEntity(const int x, const int y, const int w, const int h);
	~CEntity(void);

	// Add Components
	void ADD_Sprite(CSprite* csprite);
	void ADD_Text(CSprite* csprite);
	void ADD_Animation(CAnimate* canimate);
	void ADD_Physics(CPhysics* cphysics);
	void ADD_Collision(CCollision* ccollision);

	// Remove components
	void DELETE_Sprite();
	void DELETE_Animation();
	void DELETE_Physics();
	void DELETE_Collision();

	// Draw Entity
	void Draw(SDL_Renderer* pass_renderer);
	bool SolveCollisions(CEntity* e[]);
	bool HasCollided(CEntity* e);

	inline void UpdateRectPos()
		{rect.x = (int) (position->getX() - (rect.w / 2));
		 rect.y = (int) (position->getY() - (rect.h / 2));};

	inline void UpdatePosition()
		{position->setX((float)(rect.x + (rect.x + rect.w))/2);
		 position->setY((float)(rect.y + (rect.y + rect.h))/2);};

	inline void SetDebug(bool set) {debug = set;};

	inline void MoveX(int num) {rect.x += num; UpdatePosition();};
	inline void MoveY(int num) {rect.y += num; UpdatePosition();};

	// Setters
	inline void SetX(int num){position->setX((float)num);};
	inline void SetY(int num){position->setY((float)num);};
	inline void SetW(int num){rect.w = num;};
	inline void SetH(int num){rect.h = num;};
	inline void SetAll(int x, int y, int w, int h)
	{rect.x = x; rect.y = y; rect.w = w; rect.h = h; UpdatePosition();};
	inline void SetDraw(bool boolean){draw = boolean;};

	// Getters
	inline int GetX() const {return (int)position->getX();};
	inline int GetY() const {return (int)position->getY();};
	inline int GetW() const {return rect.w;};
	inline int GetH() const {return rect.h;};
	inline int GetXRect() const {return rect.x;};
	inline int GetYRect() const {return rect.y;};
	inline SDL_Rect GetRect() const {return rect;};
	inline CSprite* GetSprite() const {return sprite;};
	inline CAnimate* GetAnimation() const {return animate;};
	inline CPhysics* GetPhysics() const {return physics;};
	inline CCollision* GetCollision() const {return collision;};

	inline Vector2D* GetPosition() const {return position;};
	
private:
	SDL_Rect rect;
	bool draw;
	bool debug;
	Vector2D* position;

	// components
	CSprite* sprite;
	CAnimate* animate;
	CPhysics* physics;
	CCollision* collision;
};

