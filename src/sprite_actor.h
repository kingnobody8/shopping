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
		West,
		Count,
	};

	SpriteActor();
	virtual ~SpriteActor();

	virtual void Update(float dt);
	virtual void Draw(sf::RenderWindow& window);

	void SetFacing(CharacterDirection direction);
	sf::Vector2f GetPosition() { return m_sprite.getPosition(); }
	void SetPosition(const sf::Vector2f pos) { m_sprite.setPosition(pos); }
	sf::IntRect GetRect();

private:
	void CheckKey(sf::Keyboard::Key key, int i, float dt);
	void Move(int x, int y, float dt);

	short m_frame;
	short m_totalFrames;
	CharacterDirection m_facing;
	float m_animTimer;
	float m_keyTimes[CharacterDirection::Count];
	float m_speed;
	sf::Vector2f m_velocity;

	sf::Sprite m_sprite;
	sf::Texture m_texture;
};