#include "tilemap.h"
#include "gfx_util.h"


TileMap::TileMap()
	: m_pMap(nullptr)
{
	
}


bool TileMap::Init(const std::string& szMapPath, std::vector<Actor*>& vActors)
{
	m_pMap = new Tmx::Map();
	m_pMap->ParseFile(szMapPath);
	if (m_pMap->HasError())
	{
		printf("error code: %d\n", m_pMap->GetErrorCode());
		printf("error text: %s\n", m_pMap->GetErrorText().c_str());
		return false;
	}

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


	return true;
}

void TileMap::Exit()
{
	for (size_t i = 0; i < m_vTilesetTexture.size(); ++i)
	{
		delete m_vTilesetTexture[i];
	}
	m_vTilesetTexture.clear();

	//TODO (daniel) remove all actors created from the tilemap

	delete m_pMap;
	m_pMap = nullptr;
}


//void TileActor* TileMap::FillOutDefaultTile(TileActor* pTileActor)
//{
//
//}
