#pragma once

#include "Matrix4.h"
#include "Mesh.h"
#include "Shader.h"

/**
 * A Class to store objects related to the rendering of an object.
 */
class RenderObject	{
public:
	RenderObject(void);
	RenderObject(Mesh*m, Shader*s, GLuint t = 0);
	~RenderObject(void);

	Mesh*	GetMesh()	const			{return mesh;}
	void	SetMesh(Mesh*m)				{mesh = m;}

	Shader* GetShader()		const		{return shader; }
	void	SetShader(Shader*s)			{shader = s;}

	GLuint	GetTexture()		const	{return texture;}
	void	SetTexture(GLuint tex)		{texture = tex;}

	void	SetModelMatrix(Matrix4 mat) {modelMatrix = mat;}
	Matrix4 GetModelMatrix()	const	{return modelMatrix;}

	virtual void Update(float msec);

	virtual void Draw() const;

	void	AddChild(RenderObject &child) {
		children.push_back(&child);
		child.parent = this;
	}

	Matrix4 GetWorldTransform() const {
		return worldTransform;
	}

	const vector<RenderObject*>& GetChildren() const  {
		return children;
	}

protected:
	Mesh*	mesh;
	Shader*	shader;

	GLuint	texture;

	Matrix4 modelMatrix;
	Matrix4 worldTransform;

	RenderObject*			parent;
	vector<RenderObject*>	children;
};

