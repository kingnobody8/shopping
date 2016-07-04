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
	, m_bIsInputDisabled(false)
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
	if (!m_bIsInputDisabled)
	{
		CheckKey(sf::Keyboard::Left, GridEntity::EDirection::ED_WEST, dt);
		CheckKey(sf::Keyboard::Right, GridEntity::EDirection::ED_EAST, dt);
		CheckKey(sf::Keyboard::Up, GridEntity::EDirection::ED_NORTH, dt);
		CheckKey(sf::Keyboard::Down, GridEntity::EDirection::ED_SOUTH, dt);

		float best = 10000.0f;
		int b = GridEntity::EDirection::ED_COUNT;
		for (int i = 0; i < GridEntity::EDirection::ED_COUNT; ++i)
		{
			if (m_keyTimes[i] > 0 && m_keyTimes[i] < best)
			{
				best = m_keyTimes[i];
				b = i;
			}
		}

		// Hack to run fast (for testing)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		{
			m_player->m_speedMultiplier = 10;
		}
		else
		{
			m_player->m_speedMultiplier = 1;
		}

		if (!m_player->IsMoving() && b > GridEntity::EDirection::ED_INVALID && b < GridEntity::EDirection::ED_COUNT)
		{
			GridEntity::EDirection eDir = GridEntity::EDirection(b);
			if (m_player->CanMove(eDir))
			{
				m_player->Move(eDir);
			}
			else
			{
				m_player->SetFacing(eDir);
			}
		}
	}
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

