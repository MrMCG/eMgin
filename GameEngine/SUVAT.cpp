#include "using.h"

// Units for dampening and gravity
const float GRAVITY_ACCEL = -9.8f;
const float DAMPENING_COEF = 0.99f;
const float MIN_VEL = 0.1f;

// v1 = v0 + at
Vector3D SUVAT::VFromUAT(Vector3D &u, Vector3D &a, float &t)
{
	return u+(a*t);
}

// s = ut + 0.5at^2
Vector3D SUVAT::SFromUTAT(Vector3D &u, Vector3D &a, float &t)
{
	return (u*t) + ( (a*(t*t)) * 0.5 );
}

// s1 = s0 + v1t
Vector3D SUVAT::SFromSVT(Vector3D &s, Vector3D &v, float &t)
{
	return s + (v*t);
}

// symplectic euler.
// ONLY applys a force to entity, changing its velocity.
// no change in displacement
void SUVAT::ApplyExternalForce(Entity &e, Vector3D &f)
{
	// time here is set to 1 as this simply applys a force to a given entity
	// simulation calculations are used in ApplyBackgrounfForce and implement change in time
	float time = 1.0f;

	// set velocity based on v=u+at getting a from force/mass
	e.setVelocity(VFromUAT(e.getVelocity(), f/e.getMass(), time));
}

// symplectic euler.
// acts on Entity e between time t
void SUVAT::ApplyBackgroundForce(Entity &e, float &t)
{

	// set velocity based on v=u+at while using dampening and gravity
	e.setVelocity(VFromUAT(e.getVelocity()*DAMPENING_COEF, Vector3D(0,GRAVITY_ACCEL,0), t));

	

	// set new position vector (displacement) based on velocity and previous displacement
	e.setVector(SFromSVT(e.getVector(), e.getVelocity(), t));
}

void SUVAT::CheckVel(Entity &e)
{
	Vector3D temp = e.getVelocity();

	if (temp.x < MIN_VEL && temp.x > -MIN_VEL)
	{
		e.setXv(0.0f);
	}

	if (temp.y < MIN_VEL && temp.y > -MIN_VEL)
	{
		e.setYv(0.0f);
	}

	if (temp.z < MIN_VEL && temp.z > -MIN_VEL)
	{
		e.setZv(0.0f);
	}
}
