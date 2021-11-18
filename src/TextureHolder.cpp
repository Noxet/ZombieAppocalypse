#include "zpch.hpp"

#include "TextureHolder.hpp"


sf::Texture& TextureHolder::GetTexture(const std::string& filename)
{
	/*
	 * Get a texture from a filename.
	 * If the texture is not loaded, then load it and store
	 */
	auto textures = getInstance().m_textures;

	const auto keyValuePair = textures.find(filename);

	if (keyValuePair != textures.end())
	{
		// match found, return the texture
		return keyValuePair->second;
	}
	
	auto& texture = textures[filename];
	texture.loadFromFile(filename);
	return texture;
}
