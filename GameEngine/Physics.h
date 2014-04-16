#pragma once
#include "StdAfx.h"
#include "Sprite.h"

// Box2D works in the opposite y axis to SDL
// meaning all y values MUST flip signs if conversion to pixels
// is to be correct. 

// ^ CAUSE OF MANY ERRORS!

class CCollisionListener : public b2ContactListener
{
public:
	CCollisionListener() {};
	~CCollisionListener() {bodyA.clear();bodyB.clear();collision.clear();};
	
	int ADD_Collision(void* body1);
	int ADD_Collision(void* body1, void* body2);

	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);

	inline bool HasCollided(int index) const {return collision[index];};

private:
	vector<void*> bodyA;
	vector<void*> bodyB;

	vector<int> collision;
};

class CPhysics
{
public:
	// x y coor are form top left of square
	CPhysics(b2World* world, int x, int y, int w, int h, bool dyn=true);
	CPhysics(b2World* world, int x, int y, int r, bool dyn=true);
	~CPhysics();

	inline b2Body* GetBody() {return body;};
	inline void SetFixedRot(bool flag) {body->SetFixedRotation(flag);};
	inline b2PolygonShape GetPoly() const {return polygon;};
	inline b2CircleShape GetCircle() const {return circle;};
	inline int GetWidth() const {return width;};
	inline int GetHeight() const {return height;};
	inline CSprite* GetDebugSprite() const {return colTex;};

	void EnableDebugTex(SDL_Renderer* pass_renderer, b2World* world, CCollisionListener* colLis);
	inline SDL_Rect GetColBox() const {return collisionBox;};

	// sets debug rect from Box2D coor to pixel location
	void UpdateDebugPos(); 
	void UpdateDebugBox(CCollisionListener* colLis);

	static void ApplyBlastImpulse(b2Body* body, b2Vec2 blastCenter, b2Vec2 applyPoint, float blastPower);

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
	SDL_Rect collisionBox;
	int colNum;
};

class RaysCastCallback : public b2RayCastCallback
{
public:
    RaysCastCallback() : m_fixture(NULL) {
    }
 
    float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {        
        m_fixture = fixture;        
        m_point = point;        
        m_normal = normal;        
        m_fraction = fraction;        
        return fraction;     
    }    
 
    b2Fixture* m_fixture;    
    b2Vec2 m_point;    
    b2Vec2 m_normal;    
    float32 m_fraction;
 
};

