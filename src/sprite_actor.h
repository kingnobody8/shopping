#pragma once

#include "actor.h"

class SpriteActor : public Actor
{
public:
	SpriteActor();
	virtual ~SpriteActor();

	virtual void Update(float dt);
	virtual void Draw(sf::RenderWindow& window);

	const sf::Vector2f& GetPosition() { return m_sprite.getPosition(); }
	void SetPosition(const sf::Vector2f pos) { m_sprite.setPosition(pos); }
	virtual sf::IntRect GetRect();

protected:
	sf::Sprite m_sprite;
	sf::Texture m_texture;
};