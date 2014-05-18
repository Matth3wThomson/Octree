#pragma once

#include "Sphere.h"
#include <list>
#include "Octree.h"
#include "Plane.h"

using std::list;

//This IS the physics engine, done using verlet integration
class Verlet
{
public:
	//Constructor for the physics engine
	Verlet(Vector3, int threshold = 2, int maxDepth = 3);
	~Verlet(void);

	//Takes in an Sphere and updates it, with a supplied time interval
	static void update(Sphere& e, const float& time){
		if (e.awake){
			Vector3 tempPos = e.position;
			e.position += ((e.position - e.lastPos) + e.accel * (time * time)) * e.drag;
			e.lastPos = tempPos;

			//Put this object to sleep if it is not moving.
			if ((e.lastPos - e.position).absolute() < 0.00001f){
				e.lastPos = e.position;
				e.awake = false;
				//The sphere turns brown if it is asleep!
				e.ro->SetTexture(TextureManager::Instance().GetTexture("brown.png"));
			}
		}
	}

	//The method to be called every step to update the physics engine.
	inline void update(const float& msec){
		//Loop through list, updating objects
		for (list<Sphere*>::const_iterator i = spheres.begin(); i != spheres.end(); ++i){
			//Only update the awake objects
			update(**i, msec);
		}

		//Update the octree
		o->Update();

		//Perform collision detection and resolution for sphere v sphere
		o->ResolveCollisions(msec);

		//Loop through the list and check for plane collisions.
		//This currently checks every sphere with every plane. This is only a 6n = O(n) check.
		//Not a huge deal, but given more time this check would be neatened.
		for (list<Sphere*>::const_iterator i = spheres.begin(); i != spheres.end(); ++i){
			//Check all awake spheres for plane collision
			if ((*i)->getAwake()){

				//Loop through the list of planes
				for (list<Plane*>::const_iterator j = planes.begin(); j != planes.end(); ++j){
					//Check if plane has collided with sphere
					if ((*j)->Collided(**i)){
						//If so resolve collision
						(*j)->ResolveCollision(**i, msec);
					}
				}
			}
		}
	};

	//Method that takes in a renderer and draws its contents.
	//A boolean property has been provided to prevent drawing of 
	//the octree itself
	inline void Draw(SRenderer& r, bool OctreeBoundaries = false){

		//Set the polygon mode to line, so you can see inside the "cube"
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//Draw the octree bounds
		if (OctreeBoundaries){	
			o->Draw(r);
		}

		//Draw all of the planes
		for (list<Plane*>::const_iterator i = planes.begin(); i != planes.end(); ++i){
			(*i)->Draw(r);
		}

		//Draw all of the spheres as actual shapes again, using
		//gl fill
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		for (list<Sphere*>::const_iterator i = spheres.begin(); i != spheres.end(); ++i){
			(*i)->Draw(r);
		}
	}

	//Method for creating and inserting a sphere into the physics engine for updating.
	Sphere* CreateSphere(const Vector3& position, const float& radius, const float& mass, const float& drag = 1.0f, const float& elasticity = 0.3f){
		//Create the sphere
		Sphere* s = new Sphere(position, radius, mass, drag, elasticity);

		//If the sphere was not successfully inserted, return false
		if (!o->AddSphere(*s)){
			std::cout << "FAILED TO INSERT:\n" << *s << std::endl;
			delete s;
			return NULL;
		}

		//Add the sphere to the sequential list.
		spheres.push_back(s);

		//Return a pointer to the newly created shape.
		return s;
	}

	//Create a plane given supplied properties
	inline void CreatePlane(const Vector3& plane, const float& distance, const Vector3& sizeForRender){
		//Add to the list of stored planes
		planes.push_back(new Plane(plane, distance, sizeForRender));
	}

	//Apply gravity to all spheres in the engine.
	inline void ApplyGravity(){
		for (list<Sphere*>::const_iterator i = spheres.begin(); i != spheres.end(); ++i){
			(*i)->setAcceleration(Vector3(0,-9.81f,0));
		}
	}

	//Remove's any acceleration from all of the objects in the engine.
	inline void RemoveAccelFromAll(){
		for (list<Sphere*>::const_iterator i = spheres.begin(); i != spheres.end(); ++i){
			(*i)->setAcceleration(Vector3(0,0,0));
		}
	}
	
protected:
	//Protected constructor to prevent instantiation of class without using specified constructor.
	Verlet(void);

	//This octree contains a reference to all of the spheres in the simulation!
	//We use this for geographical collision detection
	Octree* o;

	//This list contains a reference to all of the spheres in the engine
	//We use this for sequential access (i.e updating all objects), 
	//rather than doing a needless, and more inefficent iterate through
	//the octree
	list<Sphere*> spheres;

	//This list contains a reference to all of the planes in the engine
	//to be tested against.
	list<Plane*> planes;


};

