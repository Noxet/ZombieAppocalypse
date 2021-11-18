#pragma once

#include <map>

/*
 * A singleton class for storing textures
 */
class TextureHolder
{
public:
	static TextureHolder& getInstance()
	{
		static TextureHolder th;
		return th;
	}

private:
	TextureHolder() = default;

	std::map<std::string, sf::Texture> m_textures;

public:
	// prevent the compiler from generating these constructors
	TextureHolder(TextureHolder const&) = delete;
	void operator=(TextureHolder const&) = delete;

	static sf::Texture& getTexture(const std::string& filename);
};