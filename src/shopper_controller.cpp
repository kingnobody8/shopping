#include "shopper_controller.h"
#include "character.h"

static sf::Vector2i s_characterDeltas[] =
{
	{0, -1},	// North
	{0, 1},		// South
	{1, 0},		// East
	{-1, 0},	// West
	{0, 0},		// None
};

ShopperController::ShopperController(Character* character)
	: m_state(None)
	, m_waitTime(0.0f)
	, m_moveDir(4)
	, m_character(character)
{}

ShopperController::~ShopperController()
{}

void ShopperController::Update(float dt)
{
	if (m_waitTime > 0)
	{
		m_waitTime -= dt;
	}

	if (m_waitTime <= 0)
	{
		Think();
	}

	m_character->Move(s_characterDeltas[m_moveDir].x, s_characterDeltas[m_moveDir].y, dt);
}

void ShopperController::Think()
{
	if (m_state == State::None)
	{
		m_state = State::Idle;
	}
	else if (m_state == State::Idle)
	{
		m_state = rand() % 2 == 0 ? State::LookAround : State::Wander;
		m_waitTime = 1;
	}
	else if (m_state == State::LookAround)
	{
		m_character->SetFacing((Character::CharacterDirection) (rand() % Character::CharacterDirection::Count));
		m_waitTime = 2;
		m_state = State::Idle;
	}
	else if (m_state == State::Wander)
	{
		m_moveDir = rand() % 4;
		m_waitTime = rand() % 5;
		m_state = State::Wandering;
	}
	else if (m_state == State::Wandering)
	{
		m_moveDir = 4;
		m_state = State::Idle;
	}
}