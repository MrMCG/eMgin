#pragma once
#include "stdafx.h"
#include "Vector2D.h"
#include "Sprite.h"

class CCollision
{
public:

	CCollision(SDL_Renderer* pass_renderer);
	CCollision(SDL_Renderer* pass_renderer, int x, int y);
	virtual ~CCollision(void);

	inline CSprite* GetColTex() const {return collisionBoxTex;};
	inline SDL_Rect GetColBox() const {return collisionBox;};

	inline void SetColBox(int x, int y, int w, int h) 
		{collisionBox.x = x; collisionBox.y = y; collisionBox.w = w; collisionBox.h = h;};

	inline void SetColBoxSize(int w, int h) 
		{collisionBox.w = w; collisionBox.h = h; 
		 SetColBoxPos(Vector2D((float)collisionBox.x, (float)collisionBox.y));};

	inline void SetColBoxPos(int x, int y) 
		{collisionBox.x = x; collisionBox.y = y;};

	inline void SetColBoxPos(Vector2D pos) 
		{collisionBox.x = (int) (pos.getX() - (collisionBox.w / 2));
		 collisionBox.y = (int) (pos.getY() - (collisionBox.h / 2));};

	inline int GetX() const {return collisionBox.x;};
	inline int GetY() const {return collisionBox.y;};
	inline int GetW() const {return collisionBox.w;};
	inline int GetH() const {return collisionBox.h;};
	inline bool GetInsCol() const {return insideCollision;};
	inline bool GetBoundry() const {return solidBoundries;};
	
	inline void InitCollisionBox(SDL_Renderer* pass_renderer);

	inline void SetInsideCol(bool set) {insideCollision = set;};
	inline void SetSolBoundry(bool set) {solidBoundries = set;};

private:
	SDL_Rect collisionBox;
	CSprite* collisionBoxTex;
	bool insideCollision;
	bool solidBoundries;
};

