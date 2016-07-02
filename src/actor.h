#pragma once

#include <SFML/Graphics.hpp>

class Actor
{
public:
	Actor() {}
	virtual ~Actor() {}

	virtual void Update(float dt) {}
	virtual void Draw(sf::RenderWindow& window) {}
};