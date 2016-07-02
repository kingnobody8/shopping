#include "tilemap_actor.h"
#include "gfx_util.h"
#include "tile_actor.h"


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
	//for (size_t i = 0; i < m_vImage.size(); ++i)
	//{
	//	window.draw(m_vImage[i].sprite);
	//}
}

void TileMapActor::LoadAssets(std::vector<Actor*>& vActors)
{
	std::vector<Tmx::Tileset*> vTileset = m_pMap->GetTilesets();
	m_vTilesetTexture.resize(vTileset.size());
	for (size_t i = 0; i < vTileset.size(); ++i)
	{
		Tmx::Tileset* pTileset = vTileset[i];
		const Tmx::Image* pImage = pTileset->GetImage();
		const std::string szImagePath = "assets/" + pImage->GetSource();

		sf::Texture* pTexture = new sf::Texture();
		bool b = LoadTexture(szImagePath, *pTexture);
		m_vTilesetTexture[i] = pTexture;
	}

	const std::vector<Tmx::TileLayer*>& vTileLayer = m_pMap->GetTileLayers();
	for (size_t i = 0; i < vTileLayer.size(); ++i)
	{
		Tmx::TileLayer* pTileLayer = vTileLayer[i];
		int width = pTileLayer->GetWidth();
		int height = pTileLayer->GetHeight();
		for (int x = 0; x < width; ++x)
		{
			for (int y = 0; y < height; ++y)
			{
				if (pTileLayer->GetTileTilesetIndex(x, y) != -1)
				{
					int tileId = pTileLayer->GetTileId(x, y);
					int tileGid = pTileLayer->GetTileGid(x, y);
					int tilesetId = pTileLayer->GetTileTilesetIndex(x, y);

					auto pTileset = m_pMap->GetTileset(tilesetId);
					const Tmx::Tile* pTile = pTileset->GetTile(tileGid);

					int columns = pTileset->GetImage()->GetWidth() / pTileset->GetTileWidth();
					int rows = pTileset->GetImage()->GetHeight() / pTileset->GetTileHeight();

					sf::IntRect rect;
					rect.width = m_pMap->GetTileWidth();
					rect.height = m_pMap->GetTileHeight();
					rect.left = tileId % columns * pTileset->GetTileWidth();
					rect.top = tileId / columns * pTileset->GetTileWidth();


					sf::Texture* pTexture = m_vTilesetTexture[tilesetId];
					TileActor* pTileActor = new TileActor();
					pTileActor->SetTexture(pTexture);
					pTileActor->SetTextureRect(rect);
					pTileActor->SetPosition(x * m_pMap->GetTileWidth(), y * m_pMap->GetTileHeight());

					vActors.push_back(pTileActor);
				}
			}
		}
	}


}

void TileMapActor::UnloadAssets()
{
	for (size_t i = 0; i < m_vTilesetTexture.size(); ++i)
	{
		delete m_vTilesetTexture[i];
	}
	m_vTilesetTexture.clear();
}