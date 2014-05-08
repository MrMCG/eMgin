#pragma once

#include <iostream>

/* used for entity position */

class Vector2D
{
public:
	
	// Constructors

	Vector2D();
	Vector2D(const float &x, const float &y);
	Vector2D(const float &size);
	Vector2D(const Vector2D &vecA); // copy constructor
	~Vector2D(); // destructor

	// Functions

	static Vector2D FindUnit(const Vector2D &vecA, const float &length);
	float LengthSquared();
	float Length();
	static float Distance(Vector2D &vecA, Vector2D &vecB);
	void Normalise();
	void Normal();
	Vector2D DOT(const Vector2D &vecB);

	float getX() const {return x;};
	float getY() const {return y;};

	void setX(const float &num) {x=num;};
	void setY(const float &num) {y=num;};
	void setVec(const Vector2D &vec) {x=vec.x,y=vec.y;};

	// Arithmetic Operators

	float operator * (const Vector2D &vecA);	
	Vector2D operator * (const float &num);

	Vector2D operator - (const Vector2D &vecA);

	Vector2D operator + (const Vector2D  &vecA);

	Vector2D operator / (const Vector2D  &vecA);
	Vector2D operator / (const float &num);

	// Update Operators

	void operator += (const Vector2D &vecA);
	void operator -= (const Vector2D &vecA);
	void operator /= (const Vector2D &vecA);

	// Relational Operators

	bool operator == (const Vector2D &vecA);
	bool operator != (const Vector2D &vecA);

private:
	float x;
	float y;
};

// kept outside as it is not class specific
std::ostream& operator << (std::ostream& os, Vector2D &vecA);
