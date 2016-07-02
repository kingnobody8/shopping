#pragma once

#include "actor.h"

class SpriteActor : public Actor
{
public:
	enum CharacterDirection
	{
		North,
		South,
		East,
		West
	};

	SpriteActor();
	virtual ~SpriteActor();

	virtual void Update(float dt);
	virtual void Draw(sf::RenderWindow& window);

	void SetFacing(CharacterDirection direction);

private:
	sf::Sprite m_sprite;
	sf::Texture m_texture;
};