#include "sprite_actor.h"

SpriteActor::SpriteActor()
{
	m_szType = "SpriteActor";
}

SpriteActor::~SpriteActor()
{}

void SpriteActor::Update(float dt)
{
}

void SpriteActor::Draw(sf::RenderWindow& window)
{
	window.draw(m_sprite);
}

sf::IntRect SpriteActor::GetRect()
{
	sf::IntRect rect = m_sprite.getTextureRect();
	rect.left = (int) m_sprite.getPosition().x;
	rect.top = (int) m_sprite.getPosition().y;
	return rect;
}