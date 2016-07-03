#include "character.h"
#include "character_controller.h"
#include "gfx_util.h"

// [direction][frame]
static sf::IntRect s_characterFrames[][4] =
{
	// North
	{
		{64, 0, 32, 32},
		{0, 0, 32, 32},
		{32, 96, 32, 32},
		{0, 0, 32, 32},
	},

	// South
	{
		{64, 96, 32, 32},
		{64, 32, 32, 32},
		{64, 64, 32, 32},
		{64, 32, 32, 32},
	},

	// East
	{
		{32, 64, 32, 32},
		{32, 0, 32, 32},
		{32, 32, 32, 32},
		{32, 0, 32, 32},
	},

	// West
	{
		{0, 96, 32, 32},
		{0, 64, 32, 32},
		{0, 32, 32, 32},
		{0, 64, 32, 32},
	},
};

Character::Character()
	: m_frame(0)
	, m_totalFrames(4)
	, m_animTimer(0.0f)
	, m_speed(32.0f)
	, m_controller(nullptr)
{
	LoadTexture("assets/textures/prof.png", m_texture);
	m_sprite.setTexture(m_texture);

	SetFacing(CharacterDirection::North);
}

Character::~Character()
{
	delete m_controller;
}

void Character::Update(float dt)
{
	m_controller->Update(dt);

	m_sprite.move(m_velocity);

	if (m_velocity.x == 0 && m_velocity.y == 0)
	{
		m_frame = 1;
		SetFacing(m_facing);
	}
	else
	{
		m_animTimer += dt;
		if (m_animTimer > 0.16f)
		{
			m_animTimer = 0;
			++m_frame;
			if (m_frame == m_totalFrames)
			{
				m_frame = 0;
			}
		}
	}
}

void Character::SetFacing(CharacterDirection direction)
{
	m_sprite.setTextureRect(s_characterFrames[direction][m_frame]);
	m_facing = direction;
}

void Character::Move(int x, int y, float dt)
{
	m_velocity.x = x * m_speed * dt;
	m_velocity.y = y * m_speed * dt;

	if (x != 0)
	{
		SetFacing(x < 0 ? CharacterDirection::West : CharacterDirection::East);
	}
	else if (y != 0)
	{
		SetFacing(y < 0 ? CharacterDirection::North : CharacterDirection::South);
	}
}