#pragma once

#include "character_controller.h"

class Character;
class ShopperController : public CharacterController
{
public:
	ShopperController(Character* character);
	virtual ~ShopperController();

	virtual void Update(float dt);

private:
	void Think();

	enum State
	{
		None,
		Idle,
		LookAround,
	};

	State m_state;
	float m_waitTime;
	Character* m_character;
};