#pragma once

#include "sprite_actor.h"

class CharacterController;
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

	static void InitializeCharacterFrameMap();

	Character();
	virtual ~Character();

	virtual void Update(float dt);

	void SetFacing(CharacterDirection direction);
	void Move(int x, int y, float dt);
	void SetSkin(short skin);

	virtual sf::IntRect GetRect();


private:
	short m_frame;
	short m_totalFrames;
	CharacterDirection m_facing;
	float m_animTimer;
	short m_skin;
	
	float m_speed;
	sf::Vector2f m_velocity;

protected:
	CharacterController* m_controller;
};