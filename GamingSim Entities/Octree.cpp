#include "Octree.h"
#include <bitset>

using std::bitset;

Octree::Octree(Vector3 size, int threshold, int maxDepth)
{
	//Set the root size to the size supplied
	root.size = size;
	//Position will always based around the origin for the purposes of
	//this coursework (info on reasons for storage this way is located in header)
	root.pos = Vector3(0,0,0) - (size * 0.5f);

	//This is the root node, it has no parent
	root.parent = NULL;

	//Create some initial nodes for the root node.
	CreateNodes(root);

	//Set the octree properties
	this->threshold = threshold;
	this->maxDepth = maxDepth;

	//Create the render object used for rendering an octree node
	cube = new RenderObject(MeshManager::Instance().GetMesh("cube.obj"), ShaderManager::Instance().GetShader("basic"), TextureManager::Instance().GetTexture("yellow.png"));
}


OctNode* Octree::CreateNode(int nodeNumber, OctNode& parent){
	//Create each node
	OctNode* o = new OctNode();

	//Set the size property for each node, (slight calc overhead here...)
	o->size = parent.size * 0.5;

	//Use bitwise operation to assign positions.
	/*
	Because an octnode always has 8 children, the numbers 1 to 7
	in binary are very helpful for assigning their positions...
	000
	001
	010
	100
	011
	101
	110
	111
	*/
	Vector3 position = parent.pos;
	bitset<32> b(nodeNumber);

	if (b.test(0)){
		//First bit (starting from the left) is on,
		//set Z correctly!
		position.z = (parent.pos.z + o->size.z );
	}

	if (b.test(1)){
		//Second bit is on, set Y correctly
		position.y = (parent.pos.y + o->size.y );
	}

	if (b.test(2)){
		//Third bit is on, set X correctly
		position.x = (parent.pos.x + o->size.x );
	}

	o->pos = position;
	o->parent = &parent;

	return o;
}

void Octree::CreateNodes(OctNode& node){
	//The size of the nodes should be 1/8th of the size of the parent.
	//(Vec3 / 2 = 1/8th size)
	for (int i=0; i<8; ++i){
		node.nodes.push_back(CreateNode(i, node));
	}
};

bool Octree::AddSphere(Sphere& e){
	//Recursively look where the Sphere should go, by looking at each node.
	return InsertSphere(root, e);
}

bool Octree::InsertSphere(OctNode& node, Sphere& e){
	//Check to see if the Sphere is within the node size, performing a
	//bounding box check

	//Check X axis
	if (e.getPos().x + e.getRadius() < node.pos.x){
		//The highest point of the sphere is lower than the lowest X. FAIL.
		return false;
	}
	if (e.getPos().x - e.getRadius() > (node.pos + node.size).x) {
		//The lowest point of the sphere is higher than the highest X. FAIL
		return false;
	}

	//Check Y axis
	if (e.getPos().y + e.getRadius() < node.pos.y){
		return false;
	}
	if (e.getPos().y - e.getRadius() > (node.pos + node.size).y) {
		return false;
	}

	//Check Z axis
	if (e.getPos().z + e.getRadius() < node.pos.z){
		return false;
	}
	if (e.getPos().z - e.getRadius() > (node.pos + node.size).z) {
		return false;
	}

	//Bounding box succeeded, do a more accurate check
	//The distance between the center of the sphere and the center of the 
	//node must be less than the radius of the sphere + the diagonal distance
	//from the center of the square to the edge (basically pretending the box 
	// is a sphere.
	if (e.getPos().GetDistance(node.pos + (node.size * 0.5)) > (e.getRadius() + node.pos.GetDistance(node.pos + (node.size *0.5)))){
		return false;
	}

	//If it is check to see whether the node has nodes for children, or
	//spheres. If nodes then, try insert the Sphere into each of the children
	if (node.nodes.size() != 0){

		//Iterate through all nodes, attempt insertion
		for (std::list<OctNode*>::const_iterator i = node.nodes.begin(); i != node.nodes.end(); ++i){
			InsertSphere(**i, e);
		}

	} else if (node.spheres.size() == threshold && NumberOfParents(node) < maxDepth){
		//Else check if the current node has reached the threshold, if so
		//make this node's children become nodes, and loop through the spheres
		//to sort them into the new nodes.
		CreateNodes(node);
		node.spheres.push_back(&e);

		while (!node.spheres.empty()){
			if (!InsertSphere(node, *node.spheres.back())) std::cout << "Fail";
			node.spheres.pop_back();
		}
	} else {
		//The node has spheres for children, and has not reached the threshold.
		//Insert this sphere into this node.
		node.spheres.push_back(&e);
	}

	//If false hasnt been returned yet, insert must have succeeeded
	return true;
}

//Collapses a node that contains nodes for children, and assigns all
//those children nodes spheres to this node.
void Octree::CollapseNode(OctNode& node){
	set<Sphere*> toBeMoved;

	//For every node in this node
	for (std::list<OctNode*>::const_iterator i = node.nodes.begin(); i != node.nodes.end(); ++i){
		//while each nodes spheres are not empty.
		while ( !(*i)->spheres.empty() ){
			//Add each sphere to a set (sets only allow unique elements)
			toBeMoved.insert((*i)->spheres.back());
			(*i)->spheres.pop_back();
		}
	}

	//Then push the set into this node. This node has now been collapsed.
	for (set<Sphere*>::const_iterator i = toBeMoved.begin(); i != toBeMoved.end(); ++i){
		node.spheres.push_back(*i);
	}

	//Then remove the old node children
	while (!(node.nodes.empty())){
		//Delete each node
		delete node.nodes.back();

		//Then remove them
		node.nodes.pop_back();
	}
}

int Octree::RemoveAwake(OctNode& node, set<Sphere*>& removed){

	//If the node supplied has spheres as children...
	if (node.spheres.size() != 0){

		//...scan through to remove awake ones.
		for (list<Sphere*>::iterator i = node.spheres.begin(); i != node.spheres.end();){
			if ( (*i)->getAwake() ){

				//Add them to a set of removed spheres
				removed.insert(*i);

				//Then remove from node
				node.spheres.erase(i++);
			} else { i++; }
		}

		return node.spheres.size();
	}
	//If the node supplied has nodes for children...
	else {
		//Create variable to store the number of spheres in this nodes children.
		int x = 0;

		//Remove all awake nodes from this nodes children, and count the number of spheres
		//the children have
		for (list<OctNode*>::iterator i = node.nodes.begin(); i != node.nodes.end(); ++i){
			x += RemoveAwake((**i), removed);
		}

		//Check to see if x is below the threshold, if so collapse the node.
		if (x < threshold || x == 0){
			CollapseNode(node);
			return node.spheres.size();
		}

		//Else just return how many spheres are in the node
		return x;
	}
}

void Octree::DrawNode(SRenderer& r, OctNode& node){
	//Set the appropriate model matrix
	cube->SetModelMatrix(Matrix4::Translation(node.pos + (node.size /2)) *
		Matrix4::Scale(node.size / 2));
	//Perform a ghost update (To set world transform == modelMatrix)
	cube->Update(0.0f);

	//Then call render
	r.Render(*cube);

	//If this has nodes for children
	if (node.nodes.size() != 0){
		//Render all its children
		for (list<OctNode*>::const_iterator i = node.nodes.begin(); i != node.nodes.end(); ++i){
			DrawNode(r, **i);
		}
	}
}

void Octree::Update(){
	//Find all the awake nodes in the octree
	set<Sphere*> awakeNodes;

	//Remove the awake spheres that cross boundaries of the node they are in, and collapse
	//nodes if they are below a threshold
	RemoveAwake(root, awakeNodes);

	//Reinsert the removed nodes into the octree
	for (set<Sphere*>::const_iterator i = awakeNodes.begin(); i != awakeNodes.end(); ++i){
		InsertSphere(root, **i);
	}
}

void Octree::CollisionResolve(OctNode& node, float& msec, set<pair<Sphere*, Sphere*>>& toBeResolved){
	//This node has nodes for children, resolve nodes
	if (node.nodes.size() != 0){
		for (list<OctNode*>::const_iterator i = node.nodes.begin(); i != node.nodes.end(); ++i){
			CollisionResolve(**i, msec, toBeResolved);
		}
	}
	//This node has spheres for children, resolve sphere collisions.
	else {
		//HERE WE START THE n^2 check
		for (list<Sphere*>::const_iterator i = node.spheres.begin(); i != node.spheres.end(); ++i){
			for (list<Sphere*>::const_iterator j = i; j != node.spheres.end(); ++j){
				if (*j != *i){
					if (j == node.spheres.end()) break;

					//Testing j against i always
					if ((*j)->getAwake()){
						if ((*j)->CheckCollision(**i)){
							//Add the sphere pairing to the set of sphere pairings that
							//must be resolved.
							toBeResolved.insert(pair<Sphere*, Sphere*>(*i, *j));
						};
					}
				}
			}
		}
	}
}