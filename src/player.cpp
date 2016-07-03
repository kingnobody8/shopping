#include "player.h"
#include "player_controller.h"

Player::Player()
{
	m_controller = new PlayerController(this);
}

Player::~Player()
{}