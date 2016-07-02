#pragma once

#include <SFML/Graphics.hpp>

inline bool LoadTexture(const std::string& name, sf::Texture& texture)
{
	if (!texture.loadFromFile(name))
	{
		printf("Failed to load texture '%s'\n", name.c_str());

		// Fallback to a magenta texture

		const int w = 96;
		const int h = 128;

		texture.create(w, h);

		const int dim = w * h * 4;
		sf::Uint8* pixels = new sf::Uint8[dim];

		for (int i = 0; i < dim; i += 4)
		{
			pixels[i + 0] = 255;
			pixels[i + 1] = 0;
			pixels[i + 2] = 255;
			pixels[i + 3] = 255;
		}

		texture.update(pixels);

		delete[] pixels;

		return false;
	}

	return true;
}