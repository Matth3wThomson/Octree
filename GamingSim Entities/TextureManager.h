#pragma once

#include "singleton.h"
#include <map>
#include <string>

#define TEXTURE_PATH "Resources\\Textures\\"

using std::map;
using std::string;

/**
* A Singleton used to handle the loading and deletion of textures.
*/
class TextureManager : public Singleton<TextureManager>
{
public:
	friend class Singleton<TextureManager>;
	
	unsigned int GetTexture(const string &filename);
	unsigned int AddTexture(const string &filename);

protected:
	TextureManager(void);
	~TextureManager(void);

	map<string, unsigned int> textures;
};

