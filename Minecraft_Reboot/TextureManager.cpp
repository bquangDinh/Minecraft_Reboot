#include "TextureManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

shared_ptr<TextureManager> TextureManager::getInstance()
{
	static shared_ptr<TextureManager> instance(new TextureManager());	// Guaranteed to be destroyed.
																		// Instantiated on first use.

	return instance;
}

shared_ptr<Texture2DArray> TextureManager::loadTextureArray(const GLchar* file, GLboolean alpha, GLboolean flip, string key) {
	// Check if the texture is already loaded
	if (textureArrays.find(key) != textureArrays.end())
	{
		cout << "Texture already loaded: " << key << endl;

		return textureArrays[key];
	}

	// Load the texture
	shared_ptr<Texture2DArray> texture = make_shared<Texture2DArray>();

	int width, height, nrChannels;

	if (flip)
		stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

	if (data)
	{
		printf("Loaded texture: %s\n", file);

		printf("Width: %d, Height: %d, Channels: %d\n", width, height, nrChannels);

		texture->Generate(width, height, 16, 16, nrChannels, data);

		textureArrays[key] = texture;

		stbi_image_free(data);

		data = nullptr;

		return texture;
	}
	else
	{
		cout << "Failed to load texture: " << file << endl;

		stbi_image_free(data);

		data = nullptr;

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