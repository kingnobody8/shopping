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

	if (!m_character->IsMoving() && m_moveDir > GridEntity::EDirection::ED_INVALID && m_moveDir < GridEntity::EDirection::ED_COUNT)
	{
		GridEntity::EDirection eDir = GridEntity::EDirection(m_moveDir);
		if (m_character->CanMove(eDir))
		{
			m_character->Move(eDir);
		}
		else
		{
			m_character->SetFacing(eDir);
		}
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
		m_state = rand() % 2 == 0 ? State::LookAround : State::Wander;
		m_waitTime = 1;
	}
	else if (m_state == State::LookAround)
	{
		m_character->SetFacing((GridEntity::EDirection) (rand() % GridEntity::EDirection::ED_COUNT));
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