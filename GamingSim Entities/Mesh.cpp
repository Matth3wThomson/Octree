#include "Mesh.h"

Mesh::Mesh(void)	{
	glGenVertexArrays(1, &arrayObject);

	for(int i = 0; i < MAX_BUFFER; ++i) {
		bufferObject[i] = 0;
	}

	texture		 = 0;
	numVertices  = 0;
	type		 = GL_TRIANGLES;

	numIndices    = 0;
}

Mesh::~Mesh(void)	{
	glDeleteVertexArrays(1, &arrayObject);			//Delete our VAO
	glDeleteBuffers(MAX_BUFFER, bufferObject);		//Delete our VBOs
}

void Mesh::Draw()	{
	glBindVertexArray(arrayObject);
	if(bufferObject[INDEX_BUFFER]) {
		glDrawElements(type, numIndices, GL_UNSIGNED_INT, 0);
	}
	else{
		glDrawArrays(type, 0, numVertices);	//Draw the triangles!
	}
	glBindVertexArray(0);	
}

void	Mesh::BufferData(const vector<Vector3>& vertices, const vector<Vector2>& textureCoords, const vector<Vector3>& normals, const vector<Vector4>& colours)	{
	glBindVertexArray(arrayObject);

	//Buffer vertex data
	glGenBuffers(1, &bufferObject[VERTEX_BUFFER]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[VERTEX_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(VERTEX_BUFFER);



	////Buffer texture data
	if(!textureCoords.empty()) {
		glGenBuffers(1, &bufferObject[TEXTURE_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TEXTURE_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(Vector2), &textureCoords[0], GL_STATIC_DRAW);
		glVertexAttribPointer(TEXTURE_BUFFER, 2, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(TEXTURE_BUFFER);
	}

	//buffer colour data
	if (!colours.empty())	{
		glGenBuffers(1, &bufferObject[COLOUR_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[COLOUR_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, colours.size() * sizeof(Vector4), &colours[0], GL_STATIC_DRAW);
		glVertexAttribPointer(COLOUR_BUFFER, 4, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(COLOUR_BUFFER);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//CREATES A 2X2 SQUARE (-1 to 1 on each axis)
Mesh* Mesh::GenerateQuad()	{
	Mesh* m = new Mesh();

	m->numVertices = 4;
	m->type = GL_TRIANGLE_STRIP;

	std::vector<Vector3> out_vertices;
	std::vector<Vector2> out_uvs;
	std::vector<Vector3> out_normals;
	std::vector<Vector4> out_colours;

	out_vertices.push_back(Vector3(-1.0f, -1.0f, 0.0f));
	out_vertices.push_back(Vector3(-1.0f,	1.0f, 0.0f));
	out_vertices.push_back(Vector3(1.0f, -1.0f, 0.0f));
	out_vertices.push_back(Vector3(1.0f,  1.0f, 0.0f));

	out_uvs.push_back(Vector2(0.0f,	0.0f));
	out_uvs.push_back(Vector2(0.0f,	1.0f));
	out_uvs.push_back(Vector2(1.0f,	0.0f));
	out_uvs.push_back(Vector2(1.0f,	1.0f));

	for (int i=0; i<4; i++){
		out_colours.push_back(Vector4(1,1,1,1));
	}

	m->BufferData(out_vertices, out_uvs, out_normals, out_colours);

	return m;
}

Mesh* Mesh::GenerateCircle(){
	Mesh* m = new Mesh();

	m->numVertices = 20;
	m->type = GL_TRIANGLE_FAN;

	std::vector<Vector3> out_vertices;
	std::vector<Vector2> out_uvs;
	std::vector<Vector3> out_normals;
	std::vector<Vector4> out_colours;

	//The center Vertex
	out_vertices.push_back(Vector3(0,0,0));
	out_colours.push_back(Vector4(1,1,1,1));
	out_uvs.push_back(Vector2(0.5f, 0.5f));

	float radius = 0.5f;

	//The vertex to the far right
	out_vertices.push_back(Vector3(0.5, 0, 0));
	out_colours.push_back(Vector4(1,1,1,1));
	out_uvs.push_back(Vector2(1.0f, 0.5f));

	for (unsigned int i=2; i<m->numVertices; ++i){
		out_vertices.push_back(Vector3((float) (radius * cos(DegToRad((float) (i - 1) * 360 / (m->numVertices - 1) )) ),
			(float) (radius * sin(DegToRad((float) ((i - 1) * 360 / (m->numVertices - 1)) ))), 0));

		out_colours.push_back(Vector4(1,1,1,1));
		out_uvs.push_back( Vector2((float) ((cos( DegToRad((float) (i - 1) * 360 / (m->numVertices - 1))) + 1) * 0.5), 
			(float) ((sin( DegToRad((float) (i - 1) * 360 / (m->numVertices - 1)) )+ 1 ) * 0.5 )));
	}

	m->numVertices++;

	//Another vertex to the far right (to close pacmans mouth)
	out_vertices.push_back(Vector3(0.5, 0, 0));
	out_colours.push_back(Vector4(1,1,1,1));
	out_uvs.push_back(Vector2(1.0f, 0.5f));

	m->BufferData(out_vertices, out_uvs, out_normals, out_colours);

	return m;
}