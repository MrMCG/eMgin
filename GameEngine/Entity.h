#pragma once
#include "stdafx.h"
#include "Sprite.h"
#include "Animate.h"
#include "Text.h"
#include "Vector2D.h"
#include "Physics.h"


// Every "object" in the game is an entity
// The entity has components which define the Entity in the game
// Each component has its own attributes

class CEntity
{
public:

	CEntity(void);

	// Create Entity, given position and size
	CEntity(const int x, const int y, const int w, const int h);
	virtual ~CEntity(void);

	// Add Components
	void ADD_Sprite(CSprite* csprite);
	void ADD_Text(CSprite* csprite);
	void ADD_Animation(CAnimate* canimate);
	void ADD_Physics(CPhysics* cphysics, bool flag=true);

	// Remove components
	void DELETE_Sprite();
	void DELETE_Animation();
	void DELETE_Physics();

	// Draw Entity
	void Draw(SDL_Renderer* pass_renderer);
	bool SolveCollisions(CEntity* e[]);

	//   -----  NOTE  -----
	// this is quite inefficinet
	// event listner should be used
	// listener can be found in Physics.h
	bool CollidesWith(CEntity* entity);
	void SetRectFromPos();
		
	inline void SetPosToCentre()
		{position->setX((float)(rect.x + (rect.x + rect.w))/2);
		 position->setY((float)(rect.y + (rect.y + rect.h))/2);};

	void UpdatePosition();
	void UpdateImageSize(float scale=1);
	void UpdateImageSize(float scaleX, float scaleY);

	void* GetPhysicsData() const;

	inline void SetDebug(bool set) {debug = set;};

	// Setters
	inline void SetX(int num){position->setX((float)num);};
	inline void SetY(int num){position->setY((float)num);};
	inline void SetW(int num){rect.w = num;};
	inline void SetH(int num){rect.h = num;};
	inline void SetAll(int x, int y, int w, int h)
	{rect.x = x; rect.y = y; rect.w = w; rect.h = h; SetPosToCentre();};
	inline void SetDraw(bool boolean){draw = boolean;};
	inline void setAngle(float rad){angle = (rad * 180) / 3.142;}

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
	inline Vector2D* GetPosition() const {return position;};
	
protected:
	SDL_Rect rect;
	bool draw;
	bool debug;
	Vector2D* position;

	// components
	CSprite* sprite;
	CAnimate* animate;
	CPhysics* physics;

	float scale;
	float angle;
};

