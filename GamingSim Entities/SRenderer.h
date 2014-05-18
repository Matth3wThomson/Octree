#pragma once

#include "RenderObject.h"
#include "Matrix4.h"

/**
* A Custom renderer class to handle the rendering of render objects
*/
class SRenderer
{
public:
	SRenderer();

	/**
	* Constructor for the Renderer Creation.
	* @param orthographic Whether or not this renderer is for orthographic elements
	*/
	SRenderer(int width, int height, bool orthgraphic = false);
	~SRenderer(void);

	/**
	* Renders all objects this renderer holds references to
	*/
	virtual void RenderScene();

	/**
	* Render a given renderObject
	*/
	virtual void Render(const RenderObject &o);

	/**
	* Update RenderObjects
	*/
	virtual void UpdateScene(float msec);

	inline void SetProjectionMatrix(Matrix4 m){	projMatrix = m;	}
	inline void SetViewMatrix(Matrix4 m){ viewMatrix = m; }

	void AddRenderObject(RenderObject &r){
		renderObjects.push_back(&r);
	}

	//TODO: TEST THIS PROPERLY
	void RemoveRenderObject(const RenderObject &r){
		for (vector<RenderObject*>::const_iterator i; i != renderObjects.end(); ++i){
			if ((*i) == &r){
				renderObjects.erase(i);
			}
		}
	};

protected:
	void UpdateShaderMatrices(GLuint program);



	vector<RenderObject*> renderObjects;
	Matrix4 modelMatrix;
	Matrix4 viewMatrix;
	Matrix4 projMatrix;
	Matrix4 textureMatrix;
};

