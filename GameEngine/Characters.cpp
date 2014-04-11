#include "Characters.h"
#include "settings.h"

using namespace settings;

void CPlayer::MoveLeft(int force)
{
	physics->GetBody()->ApplyForceToCenter(b2Vec2(-force,0));
}

void CPlayer::MoveRight(int force)
{
	physics->GetBody()->ApplyForceToCenter(b2Vec2(force,0));
}

void CPlayer::MoveDown(int force)
{
	physics->GetBody()->ApplyForceToCenter(b2Vec2(0,-force));
}

void CPlayer::MoveUp(int force)
{
	physics->GetBody()->ApplyForceToCenter(b2Vec2(0,force));
}

void CPlayer::Jump()
{
	float num = physics->GetBody()->GetLinearVelocity().y;
   	if (num <= 0.001 && num >= -0.001)
	{
		physics->GetBody()->ApplyForceToCenter(b2Vec2(physics->GetBody()->GetLinearVelocity().x,200));
	} 
}

void CBullet::Fire(b2Body* origin, b2Vec2 destination)
{
	physics->GetBody()->SetLinearVelocity(b2Vec2(0,0));
	physics->GetBody()->SetGravityScale(0);
	b2Vec2 position = origin->GetPosition();
	b2Vec2 destinationCalced = b2Vec2(destination.x / TILE_PIXEL_METER, destination.y / TILE_PIXEL_METER);
	b2Vec2 direction = (destinationCalced - origin->GetPosition());

	direction.Normalize();
	direction *= 3;
	position += direction;

	physics->GetBody()->SetTransform(position, 0);
	direction = (destinationCalced - origin->GetPosition());
	direction.Normalize();
	direction *= 50;
	//direction.y *= 10;
	physics->GetBody()->ApplyLinearImpulse(direction, physics->GetBody()->GetLocalCenter());
	physics->GetBody()->SetAngularDamping(6);
}