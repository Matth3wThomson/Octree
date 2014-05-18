#include "ShaderManager.h"


ShaderManager::ShaderManager(void)
{

}


ShaderManager::~ShaderManager(void)
{
	for (map<string, Shader*>::iterator i = shaders.begin();
		i!= shaders.end(); ++i){
			delete i->second;
	}
}

Shader* ShaderManager::GetShader(const string& filename){
	//Find the shader in the mapping
	map<string, Shader*>::iterator i = shaders.find(filename);

	//If found, return it
	if (i != shaders.end()){
		return i->second;
	}

	//Else return null flag
	return NULL;
}

Shader* ShaderManager::AddShader(const string& filename, const string &vert, const string &frag, const string & geom, const string &tcs, const string &tes){
	//Check the shader hasnt already been loaded in
	Shader* s = GetShader(filename);

	if (s != NULL){
		return s;
	}

	string geomt = "";
	string tcst = "";
	string test = "";

	if (geom != ""){
		geomt = SHADER_PATH + geom;
		if (tcs != ""){
			tcst = SHADER_PATH + tcs;
		}
		if (tes != ""){
			test = SHADER_PATH + tes;
		}
	}

	//Else load in the Shader
	s = new Shader(SHADER_PATH + vert, SHADER_PATH + frag, geomt, tcst, test);

	//Add the shader to the mapping
	shaders.insert(std::pair<string, Shader*>(filename, s));

	//Return the shader
	return s;

}
