/******************************************************************************
Class:Mesh
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description:Wrapper around OpenGL primitives, geometry and related 
OGL functions.

There's a couple of extra functions in here that you didn't get in the tutorial
series, to draw debug normals and tangents. 


-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////
#pragma once

//#include "../nclgl/OGLRenderer.h"
#include <vector>
#include <string>
#include <fstream>
#include <vector>
#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"
#include "Common.h"
#include <gl/glew.h>


using std::vector;

using std::ifstream;
using std::string;

//A handy enumerator, to determine which member of the bufferObject array
//holds which data
enum MeshBuffer {
	VERTEX_BUFFER	=0,
	COLOUR_BUFFER	=1, 
	TEXTURE_BUFFER	,
	INDEX_BUFFER	,
	MAX_BUFFER
};

/**
 * A class to represent a Mesh in openGL. Vertex data is buffered to VRAM
 * then deleted from local CPU memory
 */
class Mesh	{
public:
	friend class MeshManager;

	virtual void Draw();

	//Sets the Mesh's diffuse map. Takes an OpenGL texture 'name'
	void	SetTexture(GLuint tex)	{texture = tex;}

	//Gets the Mesh's diffuse map. Returns an OpenGL texture 'name'
	GLuint  GetTexture()			{return texture;}

	GLuint	type;

protected:
	/**
	 * A Quad generation method. Creates a 2x2 square
	 */
	static Mesh* GenerateQuad();

	/**
	 * A circle generation method. Creates a radius = 1 circle triangle fan mesh.
	 */
	static Mesh* GenerateCircle();

	//Buffers all VBO data into graphics memory. Required before drawing!
	void	BufferData(const vector<Vector3>& vertices, const vector<Vector2>& textureCoords = vector<Vector2>(), const vector<Vector3>& normals = vector<Vector3>(), const vector<Vector4>& colours = vector<Vector4>());

	//Protected constructor so that only the meshManager can make meshes!
	Mesh(void);

	//Protected destructor so that only the meshManager can delete meshes.
	virtual ~Mesh(void);

	//VAO for this mesh
	GLuint	arrayObject;
	//VBOs for this mesh
	GLuint	bufferObject[MAX_BUFFER];

	//Number of vertices for this mesh
	GLuint	numVertices;

	//Primitive type for this mesh (GL_TRIANGLES...etc)
	//OpenGL texture name for the diffuse map
	GLuint	texture;
	
	//Number of indices for this mesh
	GLuint			numIndices;
};

