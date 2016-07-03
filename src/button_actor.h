#pragma once

#include <SFML/Graphics.hpp>

class ButtonActor : public Actor
{
public:
	ButtonActor();
	virtual ~ButtonActor();

	virtual void Update(float dt);
	virtual void Draw(sf::RenderWindow& window);

private:
	sf::RectangleShape m_hitbox;
	sf::Sprite m_sprite;
};