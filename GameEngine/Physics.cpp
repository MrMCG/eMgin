#include "Physics.h"
#include "settings.h"

using namespace settings;

CPhysics::CPhysics()
{
	velocity = new Vector2D(0,0);
	mass = 0;

	gravity = 0.0f;
	dampening = 0.9f; 
	elasticity = 1.0f;
}

CPhysics::~CPhysics(void)
{
	delete velocity;
}

// v1 = v0 + at
Vector2D CPhysics::VFromUAT(Vector2D &u, Vector2D &a, float &t)
{
	return u+(a*t);
}

// s1 = s0 + v1t
Vector2D CPhysics::SFromSVT(Vector2D &s, Vector2D &v, float &t)
{
	return s + (v*t);
}

// symplectic euler.
// ONLY applys a force to entity, changing its velocity.
// no change in displacement
void CPhysics::ApplyExternalForce(Vector2D &f)
{
	float time = 1;
	// set velocity based on v=u+at getting a from force/mass
	velocity->setVec(VFromUAT(*velocity, f/mass, time));
}

// symplectic euler.
// acts on Entity e between time t
void CPhysics::ApplyBackgroundForce(Vector2D* position, float &t)
{
	// set new velocity (Vx+1) based on v=u+at and previous velocity(Vx). also adds dampening and gravity
	velocity->setVec(VFromUAT((*velocity)*dampening, Vector2D(0,gravity), t));

	// check for low velocity
	CheckVel();
}

// sets new position of entity
void CPhysics::UpdatePosition(Vector2D* position, float &t)
{
	// set new position vector (displacement: Sx+1) based on velocity(Vx+1) and previous displacement(Sx)
	position->setVec(SFromSVT(*position, *velocity, t));
}

// determines if entities is at rest
void CPhysics::CheckVel()
{
	if (velocity->getX() < MIN_VEL && velocity->getX() > -MIN_VEL)
	{
		velocity->setX(0);
	}

	if (velocity->getY() < MIN_VEL && velocity->getY() > -MIN_VEL)
	{
		velocity->setY(0);
	}
}