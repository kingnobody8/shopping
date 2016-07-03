#include "player_controller.h"

static sf::Vector2i s_characterDeltas[] =
{
	{0, -1},	// North
	{0, 1},		// South
	{1, 0},		// East
	{-1, 0},	// West
	{0, 0},		// None
};

PlayerController::PlayerController(Character* player)
	: m_player(player)
{
	for (int i = 0; i < 4; ++i)
	{
		m_keyTimes[i] = 0;
	}
}

PlayerController::~PlayerController()
{}

void PlayerController::Update(float dt)
{
	CheckKey(sf::Keyboard::Left, Character::CharacterDirection::West, dt);
	CheckKey(sf::Keyboard::Right, Character::CharacterDirection::East, dt);
	CheckKey(sf::Keyboard::Up, Character::CharacterDirection::North, dt);
	CheckKey(sf::Keyboard::Down, Character::CharacterDirection::South, dt);

	float best = 10000.0f;
	int b = Character::CharacterDirection::Count;
	for (int i = 0; i < Character::CharacterDirection::Count; ++i)
	{
		if (m_keyTimes[i] > 0 && m_keyTimes[i] < best)
		{
			best = m_keyTimes[i];
			b = i;
		}
	}

	m_player->Move(s_characterDeltas[b].x, s_characterDeltas[b].y, dt);
}

void PlayerController::CheckKey(sf::Keyboard::Key key, int i, float dt)
{
	if (sf::Keyboard::isKeyPressed(key))
	{
		m_keyTimes[i] += dt;
	}
	else
	{
		m_keyTimes[i] = 0;
	}
}