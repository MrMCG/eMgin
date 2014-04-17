#include "Characters.h"
#include "settings.h"

using namespace settings;

const int TEX_PLAYER = 1;
const int TEX_BACKGROUND = 2;
const int TEX_BULLET = 3;
const int TEX_METALCRATE = 4;
const int TEX_METALFLOOR = 5;
const int TEX_DEBUG = 6;
const int TEX_ENEMY = 8;

// ----------------------------------------------------------
// ------------------------- PLAYER -------------------------
// ----------------------------------------------------------
CPlayer::CPlayer(SDL_Renderer* pass_renderer, CResources* resources, b2World* world) : CEntity()
{
	// Set Physics
	ADD_Physics(new CPhysics(world, 0, 0, 4, 6));

	InitVar(pass_renderer, resources);
}

CPlayer::CPlayer(SDL_Renderer* pass_renderer, CResources* resources, b2World* world, const int x, const int y, const int w, const int h) : CEntity()
{
	// Set Physics
	ADD_Physics(new CPhysics(world, x, -y, w, h));

	InitVar(pass_renderer, resources);
}

void CPlayer::InitVar(SDL_Renderer* pass_renderer, CResources* resources)
{
	alive = true;
	idle=true;
	falling = false;
	jumping=false;
	facing = 3;
	// 1 = looking up
	// 2 = looking down
	// 3 = looking left
	// 4 = looking right	

	physics->SetFixedRot(true);
	physics->GetBody()->SetLinearDamping(2);
	physics->GetBody()->SetGravityScale(0.3);

	// Set animation
	CAnimate* playerAnim = new CAnimate(pass_renderer, resources, TEX_PLAYER);
	playerAnim->SetSpriteSheet(4,2);
	playerAnim->SetSpeed(128);

	ADD_Animation(playerAnim);
}

void CPlayer::MoveLeft(int force)
{
	physics->GetBody()->ApplyForceToCenter(b2Vec2(-force,0));
	facing = 3;
}

void CPlayer::MoveRight(int force)
{
	physics->GetBody()->ApplyForceToCenter(b2Vec2(force,0));
	facing = 4;
}

void CPlayer::MoveDown(int force)
{
	physics->GetBody()->ApplyForceToCenter(b2Vec2(0,-force));
	facing = 2;
}

void CPlayer::MoveUp(int force)
{
	physics->GetBody()->ApplyForceToCenter(b2Vec2(0,force));
	facing = 1;
}

void CPlayer::Jump(int force)
{
   	if (!jumping)
	{
		MoveUp(force);
		jumping = true;
	} 
}

void CPlayer::UpdatePosition()
{
	CEntity::UpdatePosition();
	UpdateState();
}

void CPlayer::UpdateState()
{
	float num = physics->GetBody()->GetLinearVelocity().y;

	if (num <= -45)
	{
		falling = true;
	} else
	{
		falling = false;
	}
	
	if (num <= 0.001 && num >= -0.001)
	{
		jumping = false;
	}
}

// ----------------------------------------------------------
// ------------------------- BULLET -------------------------
// ----------------------------------------------------------

const int FIRE_DELAY = 300;

CBullet::CBullet(CResources* resources, b2World* world, int s) : CEntity()
{
	// Set Texture
	ADD_Sprite(new CSprite(resources->GetTex(TEX_BULLET)));
	ADD_Physics(new CPhysics(world, 0, 0, s));
	InitVar();
}

CBullet::CBullet(CResources* resources, b2World* world, CSprite* cSprite, int s) : CEntity()
{
	// Set Texture
	ADD_Sprite(cSprite);
	ADD_Physics(new CPhysics(world, 0, 0, s));
	InitVar();
}

void CBullet::InitVar()
{
	rayNumber = 32;
	amountActive = 0;
	time = 0;

	b2Body* body = physics->GetBody();

	body->SetLinearVelocity(b2Vec2(0,0));
	body->SetLinearDamping(0);
	body->SetBullet(true);
	body->SetGravityScale(0.05);
	body->SetAngularDamping(4);
	body->SetActive(false);
}

bool CBullet::Fire(b2World* world, CPlayer* player, b2Vec2 destination)
{
	if (time+FIRE_DELAY < SDL_GetTicks())
	{
		b2Body* body = physics->GetBody();
		body->SetActive(true);
		b2Body* origin = player->GetPhysics()->GetBody();
		body->SetLinearVelocity(b2Vec2(0,0));

		b2Vec2 position = origin->GetPosition();
		b2Vec2 destinationCalced = b2Vec2(destination.x / TILE_PIXEL_METER, destination.y / TILE_PIXEL_METER);
		b2Vec2 direction = (destinationCalced - origin->GetPosition());

		direction.Normalize();
		//direction *= 2;
		position += direction;

		body->SetTransform(position, 0);

		direction = (destinationCalced - origin->GetPosition()); 
		direction.Normalize();
		direction *= 1000;

		body->ApplyLinearImpulse(direction, body->GetLocalCenter());

		amountActive++;
		time = SDL_GetTicks();

		return true;
	}

	return false;
}

bool CBullet::Fire(b2World* world, CPlayer* player, bool fire2Direction)
{
	int face = player->GetFacing();
	float x = player->GetPosition()->getX();
	float y = -player->GetPosition()->getY();

	if(fire2Direction)
	{
		// 0 = right
		// 1 = left
		face %= 2;

		if (face)
		{
			face = 3;
		} else
		{
			face = 4;
		}
	}
	
	switch (face)
	{
	case 1:
		return Fire(world, player, b2Vec2(x, y+100)); // fire up
		break;
	case 2:
		return Fire(world, player, b2Vec2(x,y-100)); // fire down
		break;
	case 3:
		return Fire(world, player, b2Vec2(x-100,y)); // fire left
		break;
	case 4:
		return Fire(world, player, b2Vec2(x+100,y)); // fire right
		break;
	default:
		return Fire(world, player, b2Vec2(x+100,y)); // fire right
		break;
	}
}

void CBullet::Explode(b2World* world, float radius, float force)
{
	if (amountActive)
	{
		b2Vec2 bulletPosition = physics->GetBody()->GetPosition();
		for (int i = 0; i < rayNumber; i++) 
		{
			float angle = (i / (float)rayNumber) * 360 * (3.142/180);
			b2Vec2 rayDir( sinf(angle), cosf(angle) );
			b2Vec2 rayEnd = bulletPosition;
			rayEnd += radius * rayDir;

			//check what this ray hits
			RaysCastCallback callback;//callback to record body and hit point
			world->RayCast(&callback, bulletPosition, rayEnd);
			if ( callback.m_fixture ) 
			{
				CPhysics::ApplyBlastImpulse(callback.m_fixture->GetBody(), bulletPosition, callback.m_point, force);	  
			}
		}

		physics->GetBody()->SetActive(false);

		//world->DestroyBody(physics->GetBody());
		//DELETE_Physics();
		amountActive--;
	}
}

// ----------------------------------------------------------
// ----------------------- BACKGROUND -----------------------
// ----------------------------------------------------------
CBackground::CBackground(CResources* resources) : CEntity(0,0,SCREEN_WIDTH,SCREEN_HEIGHT)
{
	// Set Texture
	ADD_Sprite(new CSprite(resources->GetTex(2)));	
}

CBackground::CBackground(CResources* resources, const int x, const int y, const int w, const int h) : CEntity(x,y,w,h)
{
	// Set Texture
	ADD_Sprite(new CSprite(resources->GetTex(2)));	
}

// ----------------------------------------------------------
// -------------------------- TILE --------------------------
// ----------------------------------------------------------
CTile::CTile(CResources* resources) : CEntity(0,0,TILE_COLUMN_CALC,TILE_ROW_CALC)
{
	// Set Texture
	debug = new CSprite(resources->GetTex(TEX_DEBUG));
}

CTile::CTile(CResources* resources, const int x, const int y) : CEntity(x,y,TILE_COLUMN_CALC,TILE_ROW_CALC)
{
	// Set Texture
	debug = new CSprite(resources->GetTex(TEX_DEBUG));
}

CTile::CTile(CResources* resources, const int x, const int y, const int index) : CEntity(x,y,TILE_COLUMN_CALC,TILE_ROW_CALC)
{
	// Set Texture
	ADD_Sprite(new CSprite(resources->GetTex(index)));
	debug = new CSprite(resources->GetTex(TEX_DEBUG));
}

void CTile::SetSolid(b2World* world)
{
	ADD_Physics(new CPhysics(world, rect.x/TILE_PIXEL_METER, -rect.y/TILE_PIXEL_METER, TILE_SCALE, TILE_SCALE, false));
}

void CTile::DrawDebug(SDL_Renderer* pass_renderer)
{
	SDL_RenderCopy (pass_renderer, debug->GetTEX(), debug->GetCROP(), &rect);
}

// ----------------------------------------------------------
// ------------------------- BOUNDRY ------------------------
// ----------------------------------------------------------
CBoundry::CBoundry() : CEntity()
{
	topSide = new CEntity();
	bottomSide = new CEntity();
	leftSide = new CEntity();
	rightSide = new CEntity();
}

CBoundry::CBoundry(CResources* resources, b2World* world) : CEntity()
{
	topSide = new CEntity();
	bottomSide = new CEntity();
	leftSide = new CEntity();
	rightSide = new CEntity();

	topSide->ADD_Sprite(new CSprite(resources->GetTex(TEX_METALFLOOR)));
	bottomSide->ADD_Sprite(new CSprite(resources->GetTex(TEX_METALFLOOR)));
	leftSide->ADD_Sprite(new CSprite(resources->GetTex(TEX_METALFLOOR)));
	rightSide->ADD_Sprite(new CSprite(resources->GetTex(TEX_METALFLOOR)));

	topSide->ADD_Physics(new CPhysics(world, 0, 2, TILE_COLUMN*TILE_SCALE, 2, false));
	rightSide->ADD_Physics(new CPhysics(world, TILE_COLUMN*TILE_SCALE, 0, 2, 72, false));
	leftSide->ADD_Physics(new CPhysics(world, -2, 0, 2, TILE_ROW*TILE_SCALE, false));
	bottomSide->ADD_Physics(new CPhysics(world, 0, -TILE_ROW*TILE_SCALE, TILE_COLUMN*TILE_SCALE, 2, false));

	topSide->UpdatePosition();
	bottomSide->UpdatePosition();
	leftSide->UpdatePosition();
	rightSide->UpdatePosition();
}

void CBoundry::Draw(SDL_Renderer* pass_renderer)
{
	leftSide->Draw(pass_renderer);
	rightSide->Draw(pass_renderer);
	bottomSide->Draw(pass_renderer);
	topSide->Draw(pass_renderer);	
}

// ----------------------------------------------------------
// ------------------------- WRITING ------------------------
// ----------------------------------------------------------
CWriting::CWriting() : CEntity()
{
	ADD_Text(new CText());
	backdrop = NULL;
}

CWriting::CWriting(int x, int y, int w, int h) : CEntity(x,y,w,h)
{
	ADD_Text(new CText());
	backdrop = NULL;
}

void CWriting::Draw(SDL_Renderer* pass_renderer)
{
	if (backdrop != NULL)
	{
		SDL_RenderCopyEx (pass_renderer, backdrop->GetTEX(), backdrop->GetCROP(), &rect, abs(fmod(angle,360)), NULL, SDL_FLIP_NONE);
	}

	CEntity::Draw(pass_renderer);
}

// ----------------------------------------------------------
// -------------------------- CRATE -------------------------
// ----------------------------------------------------------
CCrate::CCrate(CResources* resources, b2World* world, int x, int y, int s) : CEntity()
{
	ADD_Sprite(new CSprite(resources->GetTex(TEX_METALCRATE)));	
	ADD_Physics(new CPhysics(world, x, -y, s, s));
}

CCrate::CCrate(CResources* resources, b2World* world, CSprite* csprite, int x, int y, int s) : CEntity()
{
	ADD_Sprite(csprite);	
	ADD_Physics(new CPhysics(world, x, -y, s, s));
}

// ----------------------------------------------------------
// -------------------------- ENEMY -------------------------
// ----------------------------------------------------------
CEnemy::CEnemy(SDL_Renderer* pass_renderer, CResources* resources, b2World* world) : CEntity()
{
	// Set Physics
	ADD_Physics(new CPhysics(world, 0, 0, 6, 6));

	InitVar(pass_renderer, resources);
}

CEnemy::CEnemy(SDL_Renderer* pass_renderer, CResources* resources, b2World* world, const int x, const int y, const int w, const int h) : CEntity()
{
	// Set Physics
	ADD_Physics(new CPhysics(world, x, -y, w, h));

	InitVar(pass_renderer, resources);
}

void CEnemy::InitVar(SDL_Renderer* pass_renderer, CResources* resources)
{
	// Set animation
	CAnimate* EnemyAnim = new CAnimate(pass_renderer, resources, TEX_ENEMY);
	EnemyAnim->SetSpriteSheet(4,1);
	EnemyAnim->SetSpeed(128);

	ADD_Animation(EnemyAnim);

	physics->SetFixedRot(true);
	physics->GetBody()->SetGravityScale(0);

	alive = true;
}