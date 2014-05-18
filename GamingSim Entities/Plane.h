#pragma once
#include "Vector3.h"
#include "Sphere.h"

class Plane
{
public:
	friend class Verlet;
	
	//Check whether a sphere has collided with this plane
	inline bool Collided(const Sphere& s){
		//No square roots involved, so no broad phase necessary
		if ( (normal.DotProduct(s.getPos())) + distance < s.getRadius() ) return true;
		return false;
	}

	//Resolve a collision between a sphere and a plane
	inline void ResolveCollision(Sphere& s, const float& time){
		//Calculate the penetration depth
		float peneDepth = s.getRadius() - (normal.DotProduct(s.getPos()) + distance);

		//Calculate the point of contact
		Vector3 contPoint = s.getPos() - normal * (s.getRadius() - peneDepth);

		//Calculate the velocity of the sphere
		Vector3 sVelo = s.getVelocity(time);

		//Calculate the velocity to the normal
		float veloNormal = -sVelo.DotProduct(normal);

		//Retreive elasticity
		float elasticity = s.getElasticity();

		//Calculate of the impulse to be applied to the sphere
		//THE ZERO INDICATES THE IMMOVABILITY OF THE PLANE
		float impulse = ( -(1 + elasticity) * veloNormal ) / normal.DotProduct(normal * ( (1 / s.getMass()) + 0));
		
		//APPLIED FORCE FROM PLANE. (Linear force addition, most like a "pinball bumper"
		impulse -= 10.0f * s.getMass();

		//Translate the sphere out of collision first
		s.translate(normal * peneDepth);

		//Work out the final velocity of the sphere
		Vector3 veloFinal = sVelo - (impulse/s.getMass()) * normal;

		//We set the spheres velocity, but leave the plane unaffected.
		s.setVelocity(veloFinal, time);
	}

	//Draws the planes associated render object.
	inline void Draw(SRenderer& r){
		r.Render(*ro);
	}
protected:
	//The normal to the plane
	Vector3 normal;
	float distance;	//The distance of the plane from the origin.

	//The renderobject for ease of drawing
	RenderObject* ro;

	//Protected (con/de)structors to prevent instantiation from outside the
	//physics engine
	Plane(void);

	//The constructor used by the physics engine. Planes are assumed static, so their
	//render objects model matrix is only ever set once.
	inline Plane(const Vector3& Plane, const float& distance, const Vector3& sizeForRender){
		normal = Plane.GetNormalised();
		this->distance = distance;

		//Planes are blue quads, even though they actually stretch to infinity, the render object will not.
		ro = new RenderObject(MeshManager::Instance().GetMesh("quad"), ShaderManager::Instance().GetShader("basic"), TextureManager::Instance().GetTexture("blue.png"));
		ro->SetModelMatrix(Matrix4::Translation(Vector3(normal.x * (-distance), normal.y * (-distance), normal.z * (-distance)))
			* Matrix4::Rotation(90, Vector3(normal.z, normal.y, normal.x)) 
			* Matrix4::Rotation(90, Vector3(1, 0, 0))
			* Matrix4::Scale(sizeForRender));
		ro->Update(0.0f);
	}

	~Plane(void){ 
		delete ro;
	};
};

