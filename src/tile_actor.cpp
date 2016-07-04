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

void TileActor::SetTexture(sf::Texture* pTexture)
{
	m_sprite.setTexture(*pTexture);
}

void TileActor::SetTextureRect(const sf::IntRect& rect)
{
	m_sprite.setTextureRect(rect);
}
