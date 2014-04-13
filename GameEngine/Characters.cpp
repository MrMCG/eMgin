#include "Characters.h"
#include "settings.h"

using namespace settings;

// ----------------------------------------------------------
// ------------------------- PLAYER -------------------------
// ----------------------------------------------------------
CPlayer::CPlayer(SDL_Renderer* pass_renderer, CResources* resources, b2World* world) : CEntity(0,0,50,50)
{
	// Set animation
	CAnimate* playerAnim = new CAnimate(pass_renderer, resources, 1);
	playerAnim->SetSpriteSheet(4,4);
	playerAnim->SetSpeed(128);

	ADD_Animation(playerAnim);

	// Set Physics
	ADD_Physics(new CPhysics(world, 0, 0, 5, 5));

	physics->SetFixedRot(true);
	physics->GetBody()->SetLinearDamping(2);
	physics->GetBody()->SetGravityScale(0);

	facing = 3;
	// 1 = looking up
	// 2 = looking down
	// 3 = looking left
	// 4 = looking right

	falling = false;
	jumping=false;
}

CPlayer::CPlayer(SDL_Renderer* pass_renderer, CResources* resources, b2World* world, const int x, const int y, const int w, const int h) : CEntity(x,y,w,h)
{
	// Set animation
	CAnimate* playerAnim = new CAnimate(pass_renderer, resources, 1);
	playerAnim->SetSpriteSheet(4,4);
	playerAnim->SetSpeed(128);

	ADD_Animation(playerAnim);

	// Set Physics
	ADD_Physics(new CPhysics(world, x/TILE_PIXEL_METER, -y/TILE_PIXEL_METER, w/TILE_PIXEL_METER, h/TILE_PIXEL_METER));

	physics->SetFixedRot(true);
	physics->GetBody()->SetLinearDamping(2);
	physics->GetBody()->SetGravityScale(0);

	facing = 3;
	// 1 = looking up
	// 2 = looking down
	// 3 = looking left
	// 4 = looking right

	falling = false;
	jumping=false;
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
CBullet::CBullet(CResources* resources) : CEntity(0,0,20,20)
{
	// Set Texture
	ADD_Sprite(new CSprite(resources->GetTexResources(3)));

	rayNumber = 32;
	amountActive = 0;
}

CBullet::CBullet(CResources* resources, const int x, const int y, const int w, const int h) : CEntity(x,y,w,h)
{
	// Set Texture
	ADD_Sprite(new CSprite(resources->GetTexResources(3)));

	rayNumber = 32;
	amountActive = 0;
}

void CBullet::Fire(b2World* world, CPlayer* player, b2Vec2 destination)
{
	ADD_Physics(new CPhysics(world, 0, 0, 1));
	b2Body* body = physics->GetBody();
	b2Body* origin = player->GetPhysics()->GetBody();

	body->SetLinearVelocity(b2Vec2(0,0));
	//body->SetFixedRotation(true);
	physics->GetBody()->SetBullet(true);
	physics->GetBody()->SetGravityScale(0.3);
	physics->GetBody()->SetAngularDamping(4);

	b2Vec2 position = origin->GetPosition();
	b2Vec2 destinationCalced = b2Vec2(destination.x / TILE_PIXEL_METER, destination.y / TILE_PIXEL_METER);
	b2Vec2 direction = (destinationCalced - origin->GetPosition());

	direction.Normalize();
	direction *= 4;
	position += direction;

	physics->GetBody()->SetTransform(position, 0);

	direction = (destinationCalced - origin->GetPosition()); 
	direction.Normalize();
	direction *= 1500;

	physics->GetBody()->ApplyLinearImpulse(direction, body->GetLocalCenter());

	amountActive++;
}

void CBullet::Fire(b2World* world, CPlayer* player, bool fire2Direction)
{
	int face = player->GetFacing();
	float x = player->GetPosition()->getX();
	float y = player->GetPosition()->getY();

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
		Fire(world, player, b2Vec2(x, y+1000)); // fire up
		break;
	case 2:
		Fire(world, player, b2Vec2(x,y-1000)); // fire down
		break;
	case 3:
		Fire(world, player, b2Vec2(x-1000,y)); // fire left
		break;
	case 4:
		Fire(world, player, b2Vec2(x+1000,y)); // fire right
		break;
	default:
		Fire(world, player, b2Vec2(x+10,y)); // fire right
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

		world->DestroyBody(physics->GetBody());
		DELETE_Physics();
		amountActive--;
	}
}

// ----------------------------------------------------------
// ----------------------- BACKGROUND -----------------------
// ----------------------------------------------------------
CBackground::CBackground(CResources* resources) : CEntity(0,0,SCREEN_WIDTH,SCREEN_HEIGHT)
{
	// Set Texture
	ADD_Sprite(new CSprite(resources->GetTexResources(2)));	
}

CBackground::CBackground(CResources* resources, const int x, const int y, const int w, const int h) : CEntity(x,y,w,h)
{
	// Set Texture
	ADD_Sprite(new CSprite(resources->GetTexResources(2)));	
}