#include "Physics.h"
#include <Box2D\Box2D.h>
#include "settings.h"

using namespace settings;

// ----------------------------------------------------------
// ------------------------- PHYSICS ------------------------
// ----------------------------------------------------------
CPhysics::CPhysics(b2World* world, int x, int y, int w, int h, bool dyn)
{
	//bodydef.fixedRotation = true;
	// x y are co or for top left corner, box2d requires centre
	bodydef.position.Set( x+(w/2), y-(h/2) );
    if(dyn)
	{
		bodydef.type=b2_dynamicBody;
	}
    body = world->CreateBody(&bodydef);

	// box2d requies dimensions from centre
	polygon.SetAsBox(w/2,h/2);

	width = w;
	height = h;

	circle = b2CircleShape();

    fixturedef.shape=&polygon;
    fixturedef.density=1;
    body->CreateFixture(&fixturedef);

	colTex = NULL;
	isCircle = false;
	collisionBox = SDL_Rect();
	colNum = -1;

	body->SetUserData(this);
}

CPhysics::CPhysics(b2World* world, int x, int y, int r, bool dyn)
{
	//bodydef.fixedRotation = true;
	// x y are co or for top left corner, box2d requires centre
	bodydef.position.Set(x+(x+(r*2))/2,y+(y+(r*2))/2);
    if(dyn)
	{
		bodydef.type=b2_dynamicBody;
	}
    body = world->CreateBody(&bodydef);

	circle.m_radius = r;

	width = r*2;
	height = r*2;

	polygon = b2PolygonShape();

    fixturedef.shape=&circle;
    fixturedef.density=1;
    body->CreateFixture(&fixturedef);

	colTex = NULL;
	isCircle = true;
	collisionBox = SDL_Rect();
	colNum = -1;

	body->SetUserData(this);
}

CPhysics::~CPhysics()
{
	// actual deletion is done by the box2d world
	body = NULL;
	delete colTex;
}

void CPhysics::EnableDebugTex(SDL_Renderer* pass_renderer, b2World* world, CCollisionListener* colLis)
{
	string path;
	if (isCircle)
	{
		path = IMAGE_LOCATION + "debug/colCircle.png";
	} else
	{
		path = IMAGE_LOCATION + "debug/colBox.png";
	}

	printf("Loading Physics Debug Tex - \t%s\n", path.c_str());

	SDL_Texture* tex = NULL;	
	tex = IMG_LoadTexture(pass_renderer, path.c_str());

	if (tex == NULL)
	{
		printf("ERROR: CPhysics - Loading debugTex Fail - %s\n", SDL_GetError());
		tex = SDL_CreateTexture(pass_renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, 50, 50);
	}

	colTex = new CSprite(tex);
	colTex->ToggleDel();

	collisionBox.w = width * TILE_PIXEL_METER;
	collisionBox.h = height * TILE_PIXEL_METER;
	UpdateDebugPos();

	colNum = colLis->ADD_Collision(body->GetUserData());
}

void CPhysics::UpdateDebugPos()
{
	collisionBox.x = (int) ((body->GetPosition().x * TILE_PIXEL_METER) - (collisionBox.w / 2));
	collisionBox.y = (int) -((body->GetPosition().y * TILE_PIXEL_METER) + (collisionBox.h / 2)); // involves flipping y axis
}

void CPhysics::UpdateDebugBox(CCollisionListener* colLis)
{
	if (colNum != -1)
	{
		if (colLis->HasCollided(colNum))
		{
			SDL_SetTextureColorMod(colTex->GetTEX(),255,150,150);
		} else
		{
			SDL_SetTextureColorMod(colTex->GetTEX(),150,150,255);
		}
	}
}

void CPhysics::ApplyBlastImpulse(b2Body* body, b2Vec2 blastCenter, b2Vec2 applyPoint, float blastPower)
{
	 b2Vec2 blastDir = applyPoint - blastCenter;
      float distance = blastDir.Normalize();
      //ignore bodies exactly at the blast point - blast direction is undefined
      if ( distance == 0 )
	  {
          return;
	  }
      float invDistance = 1 / distance;
      float impulseMag = blastPower * invDistance * invDistance;
      body->ApplyLinearImpulse( impulseMag * blastDir, applyPoint );
}

// ----------------------------------------------------------
// ------------------- CCOLLISIONLISTENER -------------------
// ----------------------------------------------------------
int CCollisionListener::ADD_Collision(void* body1) 
{
	bodyA.push_back(body1);
	bodyB.push_back(body1);
	
	collision.push_back(0);

	return collision.size()-1;
}

int CCollisionListener::ADD_Collision(void* body1, void* body2) 
{
	bodyA.push_back(body1);
	bodyB.push_back(body2);
	
	collision.push_back(0);

	return collision.size()-1;
}

void CCollisionListener::BeginContact(b2Contact* contact)
{
	void* firstBody = contact->GetFixtureA()->GetBody()->GetUserData();
	void* secondBody = contact->GetFixtureB()->GetBody()->GetUserData();

	for (int i = 0; i < bodyA.size(); i++)
	{
		if (bodyA[i] == bodyB[i])
		{
			if (firstBody == bodyA[i] || secondBody == bodyA[i])
			{
				collision[i]++;
			}
		} else
		{
			if ((firstBody == bodyA[i] && secondBody == bodyB[i]) ||
				(firstBody == bodyB[i] && secondBody == bodyA[i]))
			{
				collision[i]++;
			}
		}
	}
}

void CCollisionListener::EndContact(b2Contact* contact)
{
	void* firstBody = contact->GetFixtureA()->GetBody()->GetUserData();
	void* secondBody = contact->GetFixtureB()->GetBody()->GetUserData();

	for (int i = 0; i < bodyA.size(); i++)
	{
		if (bodyA[i] == bodyB[i])
		{
			if (firstBody == bodyA[i] || secondBody == bodyA[i])
			{
				collision[i]--;
			}
		} else
		{
			if ((firstBody == bodyA[i] && secondBody == bodyB[i]) ||
				(firstBody == bodyB[i] && secondBody == bodyA[i]))
			{
				collision[i]--;
			}
		}
	}
}

