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

void CPlayer::Jump(int force)
{
	float num = physics->GetBody()->GetLinearVelocity().y;
   	if (num <= 0.001 && num >= -0.001)
	{
		physics->GetBody()->ApplyForceToCenter(b2Vec2(physics->GetBody()->GetLinearVelocity().x,force));
	} 
}

void CBullet::Fire(b2Body* origin, b2Vec2 destination)
{
	physics->GetBody()->SetLinearVelocity(b2Vec2(0,0));
	physics->GetBody()->SetGravityScale(2);
	b2Vec2 position = origin->GetPosition();
	b2Vec2 destinationCalced = b2Vec2(destination.x / TILE_PIXEL_METER, destination.y / TILE_PIXEL_METER);
	b2Vec2 direction = (destinationCalced - origin->GetPosition());

	direction.Normalize();
	direction *= 4;
	position += direction;

	physics->GetBody()->SetTransform(position, 0);
	direction = (destinationCalced - origin->GetPosition());
	direction.Normalize();
	direction *= 800;

	physics->GetBody()->ApplyLinearImpulse(direction, physics->GetBody()->GetLocalCenter());
	physics->GetBody()->SetFixedRotation(true);
}

void CBullet::Explode(b2World* world, float radius, float force)
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
}