#pragma once

#include "Singleton.h"
#include "Shader.h"
#include <string>
#include <map>

#define SHADER_PATH "Resources\\Shaders\\"

using std::string;
using std::map;

/**
* A singleton used to handle loading and retrieval of shaders.
*/
class ShaderManager : public Singleton<ShaderManager>
{
public:
	friend class Singleton<ShaderManager>;

	Shader* GetShader(const string &filename);
	Shader* AddShader(const string &filename, const string &vert, const string &frag, const string &tcs = "", const string &tes = "", const string & geom = "");
protected:
	ShaderManager(void);
	~ShaderManager(void);

	bool	LoadShaderFile(string from, string &into);

	map<string, Shader*> shaders;
};

