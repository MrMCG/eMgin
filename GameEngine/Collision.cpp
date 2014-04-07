#include "Collision.h"
#include "settings.h"

using namespace settings;

CCollision::CCollision(SDL_Renderer* pass_renderer)
{
	SetColBox(0,0,0,0);
	collisionBoxTex = NULL;
	InitCollisionBox(pass_renderer);
	insideCollision = false;
	solidBoundries = false;
}

CCollision::CCollision(SDL_Renderer* pass_renderer, int x, int y)
{
	SetColBox(0,0,0,0);
	collisionBoxTex = NULL;
	InitCollisionBox(pass_renderer);
	insideCollision = false;
	solidBoundries = false;
}

CCollision::~CCollision(void)
{
	delete collisionBoxTex;
}

// create new texture for each physics component
void CCollision::InitCollisionBox(SDL_Renderer* pass_renderer)
{
	string path = IMAGE_LOCATION + "debug/col.png";
	SDL_Texture* col = NULL;	
	col = IMG_LoadTexture(pass_renderer, path.c_str());
	collisionBoxTex = new CSprite(col);
	collisionBoxTex->ToggleDel();
}

