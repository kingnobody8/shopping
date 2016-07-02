#include "sprite_actor.h"
#include "gfx_util.h"

static sf::IntRect s_characterFrames[] =
{
	{0, 0, 32, 32},		// North
	{64, 32, 32, 32},	// South
	{32, 0, 32, 32},	// East
	{0, 64, 32, 32},	// West
};

SpriteActor::SpriteActor()
{
	LoadTexture("assets/textures/prof.png", m_texture);
	
	m_sprite.setTexture(m_texture);
	SetFacing(CharacterDirection::North);
}

SpriteActor::~SpriteActor()
{}

void SpriteActor::Update(float dt)
{
	float speed = 400.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_sprite.move(-speed * dt, 0);

		SetFacing(CharacterDirection::West);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_sprite.move(speed * dt, 0);

		SetFacing(CharacterDirection::East);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		m_sprite.move(0, -speed * dt);

		SetFacing(CharacterDirection::North);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		m_sprite.move(0, speed * dt);

		SetFacing(CharacterDirection::South);
	}
}

void SpriteActor::Draw(sf::RenderWindow& window)
{
	window.draw(m_sprite);
}

void SpriteActor::SetFacing(CharacterDirection direction)
{
	m_sprite.setTextureRect(s_characterFrames[direction]);
}