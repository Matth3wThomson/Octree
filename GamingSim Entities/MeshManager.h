#pragma once
#include "singleton.h"
#include "Mesh.h"
#include <string>
#include <map>

#define MESH_PATH "Resources\\Meshes\\"

using std::map;
using std::string;

/**
* A singleton used to represent the loading and deletion of meshes.
*/
class MeshManager :
	public Singleton<MeshManager>
{
	friend class Singleton<MeshManager>;
public:

	Mesh* GetMesh(const string& filename);
	Mesh* AddMesh(const string& filename);

private:

	Mesh* LoadObjFile(const char* filename);

protected:
	MeshManager(void){};
	~MeshManager(void);

	map<string, Mesh*> meshes;
};

