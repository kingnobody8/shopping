#include "sprite_actor.h"
#include "gfx_util.h"

// [direction][frame]
static sf::IntRect s_characterFrames[][3] =
{
	// North
	{
		{0, 0, 32, 32},
		{64, 0, 32, 32},
		{32, 96, 32, 32},
	},

	// South
	{
		{64, 32, 32, 32},
		{64, 96, 32, 32},
		{64, 64, 32, 32},
	},
	
	// East
	{
		{32, 0, 32, 32},
		{32, 64, 32, 32},
		{32, 32, 32, 32},
	},

	// West
	{
		{0, 64, 32, 32},
		{0, 96, 32, 32},
		{0, 32, 32, 32},
	},
};

SpriteActor::SpriteActor()
	: m_frame(0)
	, m_totalFrames(3)
	, m_animTimer(0.0f)
{
	LoadTexture("assets/textures/prof.png", m_texture);
	
	m_sprite.setTexture(m_texture);
	SetFacing(CharacterDirection::North);
}

SpriteActor::~SpriteActor()
{}

void SpriteActor::Update(float dt)
{
	float speed = 32.0f;
	bool moving = false;
	sf::Vector2f vel;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		vel.x = -speed * dt;
		vel.y = 0;
		moving = true;
		SetFacing(CharacterDirection::West);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		vel.x = speed * dt;
		vel.y = 0;
		moving = true;
		SetFacing(CharacterDirection::East);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		vel.x = 0;
		vel.y = -speed * dt;
		moving = true;
		SetFacing(CharacterDirection::North);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		vel.x = 0;
		vel.y = speed * dt;
		moving = true;
		SetFacing(CharacterDirection::South);
	}

	m_sprite.move(vel);

	if (!moving)
	{
		m_frame = 0;
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

void SpriteActor::Draw(sf::RenderWindow& window)
{
	window.draw(m_sprite);
}

void SpriteActor::SetFacing(CharacterDirection direction)
{
	m_sprite.setTextureRect(s_characterFrames[direction][m_frame]);
	m_facing = direction;
}