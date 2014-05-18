#pragma once 

#include "Vector3.h"
#include <random>

/**
Creates a vector 3 at the origin.
*/
Vector3::Vector3(void)
{
	x = 0;
	y = 0;
	z = 0;
}

/**
Copy constructor for Vector3 Class
*/
Vector3::Vector3(const Vector3& rhs){
	//Copy constructor doesnt work!?
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
}

/**
Creates a vector at the points supplied
*/
Vector3::Vector3(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z;
}

/**
Destructor for vector
*/
Vector3::~Vector3(void)
{
}

