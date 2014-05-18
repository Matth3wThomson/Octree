#pragma once

#include "TextureManager.h"
#include "../soil/SOIL.h"
#include <GL/glew.h>


TextureManager::TextureManager(void)
{

}


TextureManager::~TextureManager(void)
{
	//All textures get removed at the end!
	for(map<string, unsigned int>::iterator i = textures.begin();
		i != textures.end(); ++i){
			glDeleteTextures(1, &i->second);
	}
}

unsigned int TextureManager::GetTexture(const string &filename){
	//Find the texture in the map
	map<string, unsigned int>::iterator i = textures.find(filename);

	//If its found, return its integer "name"
	if (i != textures.end()){
		return i->second;
	}
	
	//Else return null flag
	return 0;
}

unsigned int TextureManager::AddTexture(const string &filename){
	//Check texture hasnt already been added
	unsigned int tex = TextureManager::GetTexture(filename);

	//If so just return its integer "name"
	if (tex != 0){
		return tex;
	}

	//Else load it in and add it to the mapping
	tex = SOIL_load_OGL_texture((TEXTURE_PATH + filename).c_str(),
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

	textures.insert(std::pair<string, unsigned int>(filename, tex));

	return tex;
}
