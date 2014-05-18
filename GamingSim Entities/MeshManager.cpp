#include "MeshManager.h"


MeshManager::~MeshManager(void){
	for (map<string, Mesh*>::iterator i = meshes.begin();
		i!= meshes.end(); ++i){
			delete i->second;
	}
};

Mesh* MeshManager::GetMesh(const string& filename){
	//Find the mesh in the map
	map<string, Mesh*>::iterator i = meshes.find(filename);

	//If its found, return it
	if (i != meshes.end()){
		return i->second;
	}

	//Else return a null flag
	return NULL;
}

Mesh* MeshManager::AddMesh(const string& filename){
	//Check mesh hasnt already been added
	Mesh* m = MeshManager::GetMesh(filename);

	//If so just return a pointer to the mesh
	if (m != NULL){
		return m;
	}

	if (filename == "quad"){
		m = Mesh::GenerateQuad();
	} else if (filename == "circle"){
		m = Mesh::GenerateCircle();
	} else {
		//Else load in the mesh
		m = LoadObjFile((MESH_PATH + filename).c_str());
	}

	meshes.insert(std::pair<string, Mesh*>(filename, m));

	return m;
}


Mesh* MeshManager::LoadObjFile(const char* filename){
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<Vector3> temp_vertices;
	std::vector<Vector2> temp_uvs;
	std::vector<Vector3> temp_normals;

	std::vector<Vector3> out_vertices;
	std::vector<Vector2> out_uvs;
	std::vector<Vector3> out_normals;

	Mesh* m = new Mesh();
	std::vector<Vector2> uvOut;

	FILE* file = fopen(filename, "r");

	if (file == NULL){
		printf("Impossible to open the file !\n");
		return false;
	}

	while ( 1 ){
		char lineHeader[128];

		//read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;

		if ( strcmp(lineHeader, "v" ) == 0 ){
			Vector3 v;
			fscanf(file, "%f %f %f\n", &v.x, &v.y, &v.z);
			temp_vertices.push_back(v);
		} else if ( strcmp(lineHeader, "vt" ) == 0 ){
			Vector2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			temp_uvs.push_back(uv);
		} else if ( strcmp( lineHeader, "vn" ) == 0 ){
			Vector3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		} else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );

			if (matches != 9){
				printf("File can't be read by parser!");
				return NULL;
			}

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);

			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);

			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}

	}

	m->numVertices = vertexIndices.size();

	//For each vertex
	for (unsigned int i =0; i<vertexIndices.size(); ++i){
		unsigned int vertexIndex = vertexIndices[i];

		Vector3 vertex = temp_vertices[vertexIndex-1];

		out_vertices.push_back(vertex);
	}


	//For each Texture Coord (SORT THIS OUT)
	for (unsigned int i=0; i<uvIndices.size(); ++i){
		unsigned int uvIndex = uvIndices[i];

		Vector2 uv = temp_uvs[uvIndex-1];

		out_uvs.push_back(uv);
	}

	//For each normal
	for (unsigned int i=0; i<normalIndices.size(); ++i){
		unsigned int normalIndex = normalIndices[i];

		Vector3 normal = temp_normals[normalIndex-1];

		out_normals.push_back(normal);
	}

	m->BufferData(out_vertices, out_uvs, out_normals);

	return m;
}