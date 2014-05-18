#include "Verlet.h"


Verlet::Verlet(Vector3 worldSize, int threshold, int maxDepth)
{
	//Create the octree this physics engine will use.
	o = new Octree(worldSize, threshold, maxDepth);
}


Verlet::~Verlet(void)
{
	//Delete all spheres
	while (!(spheres.empty())){
		//Delete spheres
		delete spheres.back();

		//Then pop them from the back of the list
		spheres.pop_back();
	}

	//Delete all the planes
	while (!(planes.empty())){
		delete planes.back();
		planes.pop_back();
	}
}
