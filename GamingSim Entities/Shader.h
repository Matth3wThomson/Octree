/******************************************************************************
Class:Shader
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description:VERY simple class to encapsulate GLSL shader loading, linking,
and binding. Useful additions to this class would be overloaded functions to
replace the glUniformxx functions in external code, and possibly a map to store
uniform names and their resulting bindings. 

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////

#pragma once


#include <string>
#include <iostream>
#include <fstream>

//#include "GL/glew.h"
#include <GL\glew.h>


enum ShaderStage {
	SHADER_VERTEX = 0,
	SHADER_FRAGMENT,
	SHADER_GEOMETRY,
	SHADER_TCS,
	SHADER_TES,
	SHADER_MAX
};

using namespace std;

/**
* A Class to represent the storage of a glsl shader
*/
class Shader	{
public:
	friend class ShaderManager;

	GLuint  GetShaderProgram() { return program;}

	bool	ShaderLinked() { return !loadFailed;}
	bool	LinkProgram();

protected:
	/**
	* Constructor must be used through the shader manager
	*/
	Shader(string vertex, string fragment , string geometry = "", string tcs = "", string tes = "" );

	/**
	* Destructor must be used through the shader manager
	*/
	~Shader(void);

	/**
	* Loads the shader string in from a file
	*/
	bool	LoadShaderFile(string from, string &into);

	/**
	* Generates a shader from the given string and type.
	* @param from String containing the compilation text
	* @param type Enum containing the type of shader this text is
	*/
	GLuint	GenerateShader(string from, GLenum type);

	void	SetDefaultAttributes();

	GLuint objects[SHADER_MAX];
	GLuint program;

	bool loadFailed;
};

