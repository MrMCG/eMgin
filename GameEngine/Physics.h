#pragma once
#include "StdAfx.h"
#include "Sprite.h"

class CPhysics
{
public:
	// x y coor are form top left of square
	CPhysics(b2World* world, int x, int y, int w, int h, bool dyn=true);
	CPhysics(b2World* world, int r, int w, int h, bool dyn=true);
	~CPhysics();

	inline b2Body* GetBody() {return body;};
	inline void SetFixedRot(bool flag) {body->SetFixedRotation(flag);};
	inline b2PolygonShape GetPoly() const {return polygon;};
	inline b2CircleShape GetCircle() const {return circle;};
	inline int GetWidth() const {return width;};
	inline int GetHeight() const {return height;};
	inline CSprite* GetDebugSprite() const {return colTex;};

	inline void EnableDebugTex(SDL_Renderer* pass_renderer);

private:
	b2BodyDef bodydef;
	b2PolygonShape polygon;
	b2CircleShape circle;
	b2FixtureDef fixturedef;
	b2Body* body;

	int width;
	int height;

	bool isCircle;

	// used for debug, not neccesary
	CSprite* colTex;
};

