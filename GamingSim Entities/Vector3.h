#pragma once

#include <string>
#include <math.h>
#include <iostream>
#include "Common.h"

class Vector3
{
public:
	//x, y, and z components
	float x;
	float y;
	float z;

	//Constructors
	Vector3(void);
	Vector3(float x, float y, float z);

	//Copy Constructor
	Vector3(const Vector3& rhs);

	//Deconstructors
	~Vector3(void);

	//Returns the magnitude of a vector
	inline float GetMagnitude() const { return sqrt( (x * x) + (y * y) + (z * z) ); }

	//Returns the distance from this vector to another
	inline float GetDistance(const Vector3& rhs){
		return sqrt( pow( (rhs.x - x), 2 ) + pow( (rhs.y - y), 2 ) + pow (rhs.z - z, 2) );	
	};

	//Returns the distance between a vector and another without square rooting
	inline float GetDistanceNSq(const Vector3& rhs) const{
		return pow((rhs.x - x), 2) + pow((rhs.y - y), 2) + pow (rhs.z - z, 2);
	}

	//Translates the current vector by the supplied amounts
	inline void Translate(float x, float y, float z){ 
		x += x;
		y += y;
		z += z;
	}

	//Translates the current vector by the supplied vector
	inline void Translate(const Vector3& rhs){
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
	}

	//Dot Product
	inline float DotProduct(Vector3& rhs){
		return this->x * rhs.x + this->y * rhs.y + this->z * rhs.z;
	};

	//Cross Product
	inline Vector3 CrossProduct(const Vector3& rhs){
		return Vector3((y * rhs.z - z * rhs.y),
						(z * rhs.x - x * rhs.z),
						(x * rhs.y - y * rhs.x));

	}

	//Returns a normalized version of itself *IS CHECKING A BOOL QUICKER THAN SQRT IN CYCLES?*
	inline Vector3 GetNormalised() const {
		float magnitude = this->GetMagnitude();
		Vector3 temp = (*this);
		return temp / magnitude;
	}

	//Returns a vector that is absoluted
	inline Vector3 absolute() const {
		return Vector3(abs(x), abs(y), abs(z));
	}

	//Operator overload for subtraction of vectors
	inline Vector3 operator-(const Vector3& rhs) const{
		Vector3 temp(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
		return temp;
	}

	//Operator overload for subtraction of a float
	inline Vector3 operator-(const float& rhs) const{
		return Vector3(x- rhs, y - rhs, z - rhs);
	}

	//A less than operator overload for floats
	inline friend bool operator<(const Vector3& lhs, float rhs){
		if ( (lhs.x < rhs) && (lhs.y < rhs) && (lhs.z < rhs) ){
			return true;
		}
		return false;
	};
	
	//A less than operator overload for Vector3s
	inline friend bool operator<(const Vector3& lhs, const Vector3& rhs){
		if ( (lhs.x < rhs.x) && (lhs.y < rhs.y) && (lhs.z < rhs.z) ){
			return true;
		}
		return false;
	}

	//A greater than operator overload for floats
	inline friend bool operator>(const Vector3& lhs, float rhs){
		if ( (lhs.x > rhs) && (lhs.y > rhs) && (lhs.z > rhs) ){
			return true;
		}

		return false;
	};

	//Operator overload for suctract assign float
	inline Vector3 operator-=(const float& rhs){
		x -= rhs;
		y -= rhs;
		z -= rhs;

		return *this;
	}

	//Operator overload for subtract assign
	inline Vector3 operator-=(const Vector3& rhs){
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;

		return *this;
	}

	//Operator overload for addition of vectors
	inline Vector3 operator+(const Vector3& rhs){
		Vector3 temp(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
		return temp;
	}

	//Operator overload for addition of a float
	inline Vector3 operator+(const float& rhs){
		return Vector3(x + rhs, y + rhs, z + rhs);
	}

	//Operator overload for add assign
	inline Vector3 operator+=(const Vector3& rhs){
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;

		return *this;
	}

	//Operator overload for add assign float
	inline Vector3 operator+=(const float& rhs){
		x += rhs;
		y += rhs;
		z += rhs;

		return *this;
	}

	//Operator overload for Multiply by vector
	inline Vector3 operator*(const Vector3& rhs){
		Vector3 temp(this->x * rhs.x, this->y * rhs.y, this->z * rhs.z);
		return temp;
	}

	//Operator overload for Multiply assign by vector
	inline Vector3 operator*=(const Vector3& rhs){
		this->x *= rhs.x;
		this->y *= rhs.y;
		this->z *= rhs.z;

		return *this;
	}

	//Operator overload for division by a float
	inline Vector3 operator*(const float& rhs) const {
		Vector3 temp(this->x * rhs, this->y * rhs, this->z * rhs);
		return temp;
	}

	//Operator overload for divide assign
	inline Vector3 operator *=(const float& rhs){
		this->x *= rhs;
		this->y *= rhs;
		this->z *= rhs;

		return *this;
	}
	
	//Operator overload for subtracting a vector from a float
	friend Vector3 operator- (float lhs, const Vector3& rhs){
		Vector3 temp(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z);
		return temp;
	}

	//Operator overload for multiplication of a float by a vector
	friend Vector3 operator* (float lhs, Vector3 rhs){
		return rhs * lhs;
	};

	//Operator overload for divide
	inline Vector3 operator /(const Vector3& rhs){
		Vector3 temp(this->x / rhs.x, this->y / rhs.y, this->z / rhs.z);
		return temp;
	}

	//Operator overload for divide assign by Vector
	inline Vector3 operator /=(const Vector3& rhs){
		this->x /= rhs.x;
		this->y /= rhs.y;
		this->z /= rhs.z;

		return *this;
	}

	//Operator overload for division by a float
	inline Vector3 operator/(const float& rhs) const {
		Vector3 temp(this->x / rhs, this->y / rhs, this->z / rhs);
		return temp;
	}

	//Operator overload for divide assign
	inline Vector3 operator /=(const float& rhs){
		this->x /= rhs;
		this->y /= rhs;
		this->z /= rhs;

		return *this;
	}
	//Operator overload for equality
	inline bool operator==(const Vector3& rhs){
		if (abs(this->x - rhs.x) < 0.001){
			if (abs(this->y - rhs.y) < 0.001){
				if (abs(this->z - rhs.z) < 0.001){
					return true;
				}
			}
		}
		return false;
	}

	//The assignment operator for a stack variable
	inline Vector3 operator=(Vector3 rhs){
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;

		return *this;
	}

	//Outputs a vectors properties to console.
	inline friend std::ostream& operator<<(std::ostream& o, const Vector3& v){
		o << "Vector3(" << v.x << "," << v.y << "," << v.z << ")";
		return o;
	}
	
};

