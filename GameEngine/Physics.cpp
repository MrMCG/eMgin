#include "Physics.h"
#include <Box2D\Box2D.h>
#include "settings.h"

using namespace settings;

CPhysics::CPhysics(b2World* world, int x, int y, int w, int h, bool dyn)
{
	//bodydef.fixedRotation = true;
	// x y are co or for top left corner, box2d requires centre
	bodydef.position.Set(x+(x+w)/2,y+(y+h)/2);
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

	width = r;
	height = r;

	polygon = b2PolygonShape();

    fixturedef.shape=&polygon;
    fixturedef.density=1;
    body->CreateFixture(&fixturedef);

	colTex = NULL;
	isCircle = true;
}

CPhysics::~CPhysics()
{
	// actual deletion is done by the box2d world
	body = NULL;
}

void CPhysics::EnableDebugTex(SDL_Renderer* pass_renderer)
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
}