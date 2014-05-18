#pragma once

#include "Vector3.h"
#include "SRenderer.h"
#include "MeshManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"

class Sphere
{
public:

	friend class Verlet;

	//Get Methods
	inline float getX() const{ return position.x; }
	inline float getY() const{ return position.y; }
	inline float getZ() const{ return position.z; }

	inline Vector3 getPos() const{ return position; }
	inline Vector3 getLastPos() const{ return lastPos; }

	inline float getRadius() const{ return radius; }

	//Returns the current position - the last position all over time
	//v = s/t
	inline Vector3 getVelocity(const float& time) const{
		return (this->position - this->lastPos) / time;
	}

	inline float getMass() const { return mass; }
	inline float getElasticity() const{ return elasticity; }

	//Update/Setting methods
	inline void updateRadius(float &x) { radius = x; }
	inline void translate(const Vector3& s){ position += s; lastPos += s; }

	//NOTE, All methods below that alter the physics properties of a sphere
	//also wake the spheres, and change its texture to green.
	
	//Sets the spheres velocity by moving its previous position back
	//by veloctiy * time. (s = v*t)
	inline void setVelocity(const Vector3& v, const float& time){
		awake = true;
		ro->SetTexture(TextureManager::Instance().GetTexture("green.png"));

		this->lastPos = position - (v * time);
	}

	//Sets the acceleration on an sphere (remains constant)
	inline void setAcceleration(const Vector3& a){
		awake = true;
		ro->SetTexture(TextureManager::Instance().GetTexture("green.png"));
		accel = a;
	}

	//Applies a force to an sphere
	inline void applyForce(const Vector3& n){
		awake = true;
		ro->SetTexture(TextureManager::Instance().GetTexture("green.png"));
		accel += n / mass;
	}

	//Assignment operator
	inline Sphere operator=(const Sphere& rhs){
		position = rhs.position;
		lastPos = rhs.lastPos;
		radius = rhs.radius;
		mass = rhs.mass;

		return *this;
	}
	
	//Checks if two spheres are colliding.
	bool CheckCollision(const Sphere& rhs) const {
		float r = this->radius + rhs.radius;
		float b = this->position.GetDistanceNSq(rhs.position);
		return (b < (r*r));
	}

	//Resolves a collision between two spheres
	void ResolveCollision(Sphere& rhs, const float& time);

	//Returns whether or not this shape is awake
	inline bool getAwake(){
		return awake;
	}

	//Prints the contents of a sphere to console.
	virtual inline void print(std::ostream& where) const{
		where << "Sphere: \nposition: " << position << "\nRadius: " << radius;
	}

	//OStream method
	inline friend std::ostream& operator<<(std::ostream& o, const Sphere& e){
		e.print(o);
		return o;
	}

	//Given a renderer draws this sphere
	inline void Draw(SRenderer& r){
		ro->SetModelMatrix(Matrix4::Translation(position) *
			Matrix4::Scale(Vector3(radius, radius, radius)));
		ro->Update(0.0f);
		r.Render(*ro);
	}

protected:
	//Protected (con/de)structors as spheres should not be constructed outside
	//the verlet physics engine
	Sphere(void){ };
	Sphere(const Vector3& position, const float& radius, const float& mass, float drag = 1.0f, float elasticty = 0.3f);
	~Sphere(void){ delete ro; }

	//Physics properties
	Vector3 position, lastPos, accel;
	float mass, drag, elasticity, radius;

	//Used to reduce the number of collisions checks necessary.
	bool awake;

	//A Render object to make drawing of this object simple
	RenderObject* ro;

};

