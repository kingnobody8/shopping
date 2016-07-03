#include "tile_actor.h"
#include "gfx_util.h"


TileActor::TileActor()
{
	m_szType = "TileActor";
}


TileActor::~TileActor()
{}

void TileActor::Update(float dt)
{
}

void TileActor::Draw(sf::RenderWindow& window)
{
	window.draw(m_sprite);
}

void TileActor::SetTexture(sf::Texture* pTexture)
{
	m_sprite.setTexture(*pTexture);
}

void TileActor::SetTextureRect(const sf::IntRect& rect)
{
	m_sprite.setTextureRect(rect);
}

void TileActor::SetPosition(int x, int y)
{
	m_sprite.setPosition(x, y);
}

sf::IntRect TileActor::GetRect() const
{
	sf::IntRect rect = m_sprite.getTextureRect();
	rect.left = (int) m_sprite.getPosition().x;
	rect.top = (int) m_sprite.getPosition().y;
	return rect;
}