#pragma once

#include <memory>
#include <iostream>
#include <map>
#include <string>

#include "Texture2DArray.h"

#define MAIN_TEXTURE_ARRAY "main"

using namespace std;

class TextureManager
{
private:
	map<string, shared_ptr<Texture2DArray>> textureArrays;

	// Prevent other classes from instantiating this class
	TextureManager() {};
public:
	static shared_ptr<TextureManager> getInstance();

	shared_ptr<Texture2DArray> loadTextureArray(const GLchar* file, GLboolean alpha, GLboolean flip, string key);

	shared_ptr<Texture2DArray> getTextureArray(string);

	void cleanUp();
};

