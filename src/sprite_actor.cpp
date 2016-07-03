#include "sprite_actor.h"
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

static sf::Vector2i s_characterDeltas[] =
{
	{0, -1},	// North
	{0, 1},		// South
	{1, 0},		// East
	{-1, 0},	// West
	{0, 0},		// None
};

SpriteActor::SpriteActor()
	: m_frame(0)
	, m_totalFrames(4)
	, m_animTimer(0.0f)
	, m_speed(32.0f)
{
	LoadTexture("assets/textures/prof.png", m_texture);
	
	m_sprite.setTexture(m_texture);
	SetFacing(CharacterDirection::North);

	for (int i = 0; i < 4; ++i)
	{
		m_keyTimes[i] = 0;
	}
}

SpriteActor::~SpriteActor()
{}

void SpriteActor::Update(float dt)
{
	CheckKey(sf::Keyboard::Left, CharacterDirection::West, dt);
	CheckKey(sf::Keyboard::Right, CharacterDirection::East, dt);
	CheckKey(sf::Keyboard::Up, CharacterDirection::North, dt);
	CheckKey(sf::Keyboard::Down, CharacterDirection::South, dt);

	float best = 10000.0f;
	int b = CharacterDirection::Count;
	for (int i = 0; i < CharacterDirection::Count; ++i)
	{
		if (m_keyTimes[i] > 0 && m_keyTimes[i] < best)
		{
			best = m_keyTimes[i];
			b = i;
		}
	}

	Move(s_characterDeltas[b].x, s_characterDeltas[b].y, dt);

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

void SpriteActor::Draw(sf::RenderWindow& window)
{
	window.draw(m_sprite);
}

void SpriteActor::SetFacing(CharacterDirection direction)
{
	m_sprite.setTextureRect(s_characterFrames[direction][m_frame]);
	m_facing = direction;
}

void SpriteActor::CheckKey(sf::Keyboard::Key key, int i, float dt)
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

void SpriteActor::Move(int x, int y, float dt)
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

sf::IntRect SpriteActor::GetRect()
{
	sf::IntRect rect = m_sprite.getTextureRect();
	rect.left = m_sprite.getPosition().x;
	rect.top = m_sprite.getPosition().y;
	return rect;
}