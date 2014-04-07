#include <iostream>
#include <cmath>
#include "Vector2D.h"
using namespace std;

	Vector2D::Vector2D() 
	{
		this->x = 0.0f;
		this->y = 0.0f;
	}

	Vector2D::Vector2D(const float &x, const float &y)
	{
		this->x = x;
		this->y = y;
	}

	Vector2D::Vector2D(const float &size)
	{
		this->x = size;
		this->y = size;
	}

	Vector2D::Vector2D(const Vector2D &vecA)
	{
		this->x = vecA.x;
		this->y = vecA.y;
	}

	Vector2D::~Vector2D()
	{}

	// normalises a vector
	void Vector2D::Normalise() 
	{
		float length = Length();

		if(length != 0.0f)	
		{
			length = 1.0f / length;
			x = x * length;
			y = y * length;
		}
	}

	void Vector2D::Normal() 
	{
		x = -y;
		y = x;
	}

	// finds a unit vector
	Vector2D Vector2D::FindUnit(const Vector2D &vecA, const float &length) 
	{
		return Vector2D(vecA.x/length, vecA.y/length);
	}

	// finds distance between vectors
	float Vector2D::Distance(Vector2D &vecA, Vector2D &vecB)
	{	
		return (vecA - vecB).Length();
	}

	float Vector2D::Length() 
	{
		return sqrt(LengthSquared());	
	}

	float Vector2D::LengthSquared()
	{
		return (x*x)+(y*y);	
	}

	Vector2D Vector2D::DOT(const Vector2D &vecA)
	{
		return Vector2D(x * vecA.x, y * vecA.y);
	}

	// Arithmetic Operators

	float Vector2D::operator * (const Vector2D  &vecA)
	{
		return (x*vecA.x)+(y*vecA.y);
	}

	Vector2D Vector2D::operator * (const float &num)
	{
		return Vector2D(x * num, y * num);
	}

	Vector2D Vector2D::operator - (const Vector2D  &vecA)
	{
		return Vector2D(x - vecA.x, y - vecA.y);
	}

	Vector2D Vector2D::operator + (const Vector2D  &vecA)
	{
		return Vector2D(x + vecA.x, y + vecA.y);
	}

	Vector2D Vector2D::operator / (const Vector2D  &vecA)
	{
		float r,t;
		if(vecA.x == 0){r=0;}else{r=x/vecA.x;}
		if(vecA.y == 0){t=0;}else{t=y/vecA.y;}


		return Vector2D(r, t);
	}

	Vector2D Vector2D::operator / (const float &num)
	{
		float r,t;
		if(num == 0){r=0;}else{r=x/num;}
		if(num == 0){t=0;}else{t=y/num;}

		return Vector2D(r, t);
	}

	// Update Operators

	void Vector2D::operator += (const Vector2D  &vecA)
	{
		x += vecA.x;
		y += vecA.y;
	}

	void Vector2D::operator-=(const Vector2D  &vecA)
	{
		x -= vecA.x;
		y -= vecA.y;
	}

	void Vector2D::operator/=(const Vector2D &vecA)
	{
		x /= vecA.x;
		y /= vecA.y;
	}

	// Relational Operators

	bool Vector2D::operator == (const Vector2D &vecA)
	{
		return (vecA.x == x && vecA.y == y) ? true : false;
	}

	bool Vector2D::operator != (const Vector2D &vecA)
	{
		return (vecA.x == x && vecA.y == y) ? false : true;
	}

	std::ostream& operator << (std::ostream& os, Vector2D &vecA)
	{
		os << "(x: " << vecA.getX()
			<< " y: " << vecA.getY()
			<< ")";
		return os;
	}




