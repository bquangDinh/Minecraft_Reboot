#include "TextureManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureManager& TextureManager::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new TextureManager();
	}

	return *_instance;
}

TextureManager* TextureManager::_instance = nullptr;

shared_ptr<Texture2DArray> TextureManager::loadTextureArray(const GLchar* file, GLboolean alpha, string key) {
	// Check if the texture is already loaded
	if (textureArrays.find(key) != textureArrays.end())
	{
		cout << "Texture already loaded: " << key << endl;

		return textureArrays[key];
	}

	// Load the texture
	shared_ptr<Texture2DArray> texture = make_shared<Texture2DArray>();

	int width, height, nrChannels;

	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

	if (data)
	{
		texture->Generate(width, height, 16, 16, data);

		textureArrays[key] = texture;

		stbi_image_free(data);

		return texture;
	}
	else
	{
		cout << "Failed to load texture: " << file << endl;

		stbi_image_free(data);

		return nullptr;
	}
}

shared_ptr<Texture2DArray> TextureManager::getTextureArray(string key) {
	if (textureArrays.find(key) != textureArrays.end())
	{
		return textureArrays[key];
	}

	return nullptr;
}

void TextureManager::cleanUp() {
	for (auto& texture : textureArrays)
	{
		glDeleteTextures(1, &texture.second->ID);
	}

	textureArrays.clear();
}