#pragma once

#include "sprite_actor.h"

class Character : public SpriteActor
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

	Character();
	virtual ~Character();

	virtual void Update(float dt);

	void SetFacing(CharacterDirection direction);
	
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
};