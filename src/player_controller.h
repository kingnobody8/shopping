#pragma once

#include "character_controller.h"
#include "character.h"

class PlayerController : public CharacterController
{
public:
	PlayerController(Character* player);
	virtual ~PlayerController();

	virtual void Update(float dt);

private:
	void CheckKey(sf::Keyboard::Key key, int i, float dt);

	Character* m_player;
	float m_keyTimes[GridEntity::EDirection::ED_COUNT];

	bool m_bIsInputDisabled;
};