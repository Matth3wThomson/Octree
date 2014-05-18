#pragma once

#include <list>
#include <set>
#include "Sphere.h"

#include "MeshManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"

using std::set;
using std::list;
using std::pair;

//An "OctNode" represents one node in an octree
struct OctNode {
	//A pointer to the parent of each node
	OctNode* parent;

	//Size is always a positive vector that represents the height, width and depth of the node
	Vector3 size;

	//Position always denotes the lowest x, y and z coords of the node. Its highest points are
	//position + size
	Vector3 pos;

	//We store a list of spheres if this node contains a number of spheres below the threshold.
	//We use a list because they are always checked sequentially.
	list<Sphere*> spheres;

	//We store a list of nodes if this node contains a number of nodes above the threshold.
	//We use a list because they are always check sequentially.
	list<OctNode*> nodes;
};

class Octree
{
public:
	//Creates a Octree from - 1/2 size to 1/2 size
	Octree(Vector3 size, int threshold, int maxDepth);

	//Collapse the root node.
	Octree::~Octree(void){ CollapseNode(root); }

	//This is added to the correct octNode depending on its x, y, and z coords of each face
	bool AddSphere(Sphere& e);


	//OStream method for an Octree.
	inline friend std::ostream& operator<<(std::ostream& o, const Octree& ot){
		ot.printNode(o, ot.root);
		return o;
	}

	//Pass the Octree a renderer to have it render itself
	inline void Draw(SRenderer& r){
		DrawNode(r, root);
	}

	//Update an octree to check that all nodes in it are consistent.
	//(Basically a resort of all awake nodes, more efficient ways are
	//beyond the scope of this assignment)
	void Update();

	//Resolve all the collisions of SPHERES in an octree
	inline void ResolveCollisions(float msec){
		//Create a set of pairs to add overlapping spheres too
		set<pair<Sphere*, Sphere*>> toBeResolved;

		//Find all nodes that overlap
		CollisionResolve(root, msec, toBeResolved);

		//Resolve each pair that collides
		for (set<pair<Sphere*, Sphere*>>::const_iterator i = toBeResolved.begin(); i != toBeResolved.end(); ++i){
			i->first->ResolveCollision(*i->second, msec);
		}
	};

protected:
	//The root node of the octree
	OctNode root;
	int threshold; //The number of spheres added to cause a split
	int maxDepth; //The number of parents a node is allowed.

	//A RenderObject for easy rendering of the octree (Would not be present
	// in a fully fledged physics engine).
	RenderObject* cube;

	//Create a node given its node number (denotes its position within its parent)
	OctNode* CreateNode(int nodeNumber, OctNode& parent);

	//Use this to create 8 nodes at the supplied node
	void CreateNodes(OctNode& node);

	//Recursive method to insert a sphere into an octnode
	bool InsertSphere(OctNode& node, Sphere& e);

	//A collapse node method, used when a node contains a nodes for children,
	// whose total number of spheres is less than the threshold, and will collapse it.
	void CollapseNode(OctNode& node);

	//Recursive method to search through an octnode and remove all awake nodes from 
	//it, adding them to a set supplied to it.
	int RemoveAwake(OctNode& node, set<Sphere*>& removed);

	//Recursive method to draw an oct node, and its children (if present).
	//NOTE, DOES NOT DRAW SPHERES
	void DrawNode(SRenderer& r, OctNode& node);

	//Recursively search for a node with spheres for children, then perform narrow phase
	//check for collision. If colliding, adds to a set of sphere pairs to have their
	//collisions resolved at a later date.
	void CollisionResolve(OctNode& node, float& msec, set<pair<Sphere*, Sphere*>>& toBeResolved);

	//Calculates the number of parents of a node. This is used to block the recursion.
	//It may be more efficient to store the number of parents a node has when it is created,
	//but ultra efficiency is beyond the scope of this assignment.
	int NumberOfParents(OctNode& node){
		OctNode* s = node.parent;

		if (s == NULL){
			return 0;
		}

		int x = 1;

		while (s->parent != NULL){
			s = s->parent;
			x++;
		}

		return x;
	};

	//A print method for a node
	void printNode(std::ostream& where, const OctNode& node) const{
		where << "NODE\nPosition: " << node.pos << std::endl;
		where << "Size: " << node.size << std::endl;

		if (node.spheres.size() != 0){
			where << "SPHERE LIST: " << std::endl;

			for (std::list<Sphere*>::const_iterator i = node.spheres.begin(); i != node.spheres.end(); ++i){
				where << **i << std::endl;
			}
		} else {

			where << "NODE LIST: " << std::endl;

			for (std::list<OctNode*>::const_iterator i = node.nodes.begin(); i != node.nodes.end(); ++i){
				printNode(where, **i);
			}
		}
	}


private:
	//Cant declare an octree without a defined size!
	Octree(void);
};

