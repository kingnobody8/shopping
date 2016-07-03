#include "shopper_controller.h"
#include "character.h"

ShopperController::ShopperController(Character* character)
	: m_state(None)
	, m_waitTime(0.0f)
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
}

void ShopperController::Think()
{
	if (m_state == State::None)
	{
		m_state = State::Idle;
	}
	else if (m_state == State::Idle)
	{
		m_state = State::LookAround;
		m_waitTime = 1;
	}
	else if (m_state == State::LookAround)
	{
		m_character->SetFacing((Character::CharacterDirection) (rand() % Character::CharacterDirection::Count));
		m_waitTime = 5;
		m_state = State::Idle;
	}
}