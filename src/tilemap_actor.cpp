#include "tilemap_actor.h"
#include "gfx_util.h"


TileMapActor::TileMapActor()
{
	
}

TileMapActor::TileMapActor(Tmx::Map* pMap)
	: m_pMap(pMap)
{

}


TileMapActor::~TileMapActor()
{}

void TileMapActor::Update(float dt)
{
}

void TileMapActor::Draw(sf::RenderWindow& window)
{
	window.draw(m_sprite);
}

void TileMapActor::LoadAssets()
{
	std::vector<Tmx::Tileset*> vTileset = m_pMap->GetTilesets();
	for (int i = 0; i < vTileset.size(); ++i)
	{
		Tmx::Tileset* pTileset = vTileset[i];
		const Tmx::Image* pImage = pTileset->GetImage();
		const std::string szImagePath = pImage->GetSource();
		//LoadTexture(szImagePath, pTexture);
	}

	LoadTexture("assets/textures/pokemart_tileset.png", m_texture);
	m_sprite.setTexture(m_texture);


}

void TileMapActor::UnloadAssets()
{

}