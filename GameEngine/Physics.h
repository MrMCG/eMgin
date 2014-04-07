#pragma once
#include "stdafx.h"
#include "Vector2D.h"

const float MIN_VEL = 0.5f;

class CPhysics
{
public:

	CPhysics();

	virtual ~CPhysics(void);

	// v1 = v0 + at
	Vector2D VFromUAT(Vector2D &u, Vector2D &a, float &t);

	// s1 = s0 + v1t
	Vector2D SFromSVT(Vector2D &s, Vector2D &v, float &t);

	// symplectic euler.
	// Applys a force on an entity changing its velocity.
	void ApplyExternalForce(Vector2D &f);

	// Applys total background forces
	void ApplyBackgroundForce(Vector2D* position, float &t);

	// update entity position
	void UpdatePosition(Vector2D* position, float &t);

	// function for objects to "rest"
	void CheckVel();

	// determines which entities from segments are needed for collision detection
	void SolveCollisions(){};

	// Checks and handles collisions between spheres
	bool Collision_S(){};

	// Checks collisions between spheres and planes 
	bool Collision_P(){};

	// handles collisions between spheres and planes 
	void Collision_P_T(){};
	
	// getters
	Vector2D* GetVelocity() const {return velocity;};
	float GetMass() const {return mass;};
	float GetGravity() const {return gravity;};
	float GetDamper() const {return dampening;};
	float GetElas() const {return elasticity;};
	
	// setters
	void SetMass(float num) {mass = num;};
	void SetGravity(float num) {gravity = num;};
	void SetDamper(float num) {dampening = num;};
	void SetElas(float num) {elasticity = num;};

private:
	Vector2D* velocity;
	float mass;

	float gravity;
	float dampening; 
	float elasticity;
};