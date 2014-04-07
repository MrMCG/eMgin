#pragma once

#include "using.h"

class SUVAT
{
	public:

	// v1 = v0 + at
	static Vector3D VFromUAT(Vector3D &u, Vector3D &a, float &t);

	// s = ut + 0.5at^2
	static Vector3D SFromUTAT(Vector3D &u, Vector3D &a, float &t);

	// s1 = s0 + v1t
	static Vector3D SFromSVT(Vector3D &s, Vector3D &v, float &t);

	// symplectic euler.
	// Applys a force on an entity changing its velocity.
	static void ApplyExternalForce(Entity &e, Vector3D &f);

	// Applys total background forces
	static void ApplyBackgroundForce(Entity &e, float &t);

	static void CheckVel(Entity &e);
};