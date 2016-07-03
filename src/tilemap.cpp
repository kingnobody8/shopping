#include "tilemap.h"
#include "gfx_util.h"


TileMap::TileMap()
	: m_pMap(nullptr)
{

}


bool TileMap::Init(const std::string& szMapPath, std::vector<Actor*>& vActors)
{
	//load map
	m_pMap = new Tmx::Map();
	m_pMap->ParseFile(szMapPath);
	if (m_pMap->HasError())
	{
		printf("error code: %d\n", m_pMap->GetErrorCode());
		printf("error text: %s\n", m_pMap->GetErrorText().c_str());
		return false;
	}

	//create tileset textures
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

	//create actors for all layers
	const std::vector<Tmx::Layer*>& vLayer = m_pMap->GetLayers();
	m_vTileLayerGrid.resize(vLayer.size());
	for (size_t i = 0; i < vLayer.size(); ++i)
	{
		Tmx::Layer* pLayer = vLayer[i];
		switch (pLayer->GetLayerType())
		{
		case Tmx::LayerType::TMX_LAYERTYPE_IMAGE_LAYER:	SetupImageLayer(static_cast<Tmx::ImageLayer*>(pLayer));	break;
		case Tmx::LayerType::TMX_LAYERTYPE_OBJECTGROUP: SetupObjectLayer(static_cast<Tmx::ObjectGroup*>(pLayer), vActors); break;
		case Tmx::LayerType::TMX_LAYERTYPE_TILE: SetupTileLayer(static_cast<Tmx::TileLayer*>(pLayer), i, vActors); break;
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

std::vector<Actor*> TileMap::PerformCollisionTest(const sf::IntRect& rect)
{
	/*const std::vector<Tmx::ObjectGroup*> vObjGroups = m_pMap->GetObjectGroups();
	for (int og = 0; og < vObjGroups.size(); ++og)
	{
		Tmx::ObjectGroup* pObjGroup = vObjGroups[og];
		std::vector<Tmx::Object*> vObjects = pObjGroup->GetObjects();
		for (int i = 0; i < vObjects.size(); ++i)
		{
			Tmx::Object* pObject = vObjects[i];
			std::string tupe = pObject->GetType();
		}
	}*/

	std::vector<Actor*> ret;

	const std::vector<Tmx::TileLayer*>& vTileLayer = m_pMap->GetTileLayers();
	for (size_t ilayer = 0; ilayer < vTileLayer.size(); ++ilayer)
	{
		Tmx::TileLayer* pTileLayer = vTileLayer[ilayer];
		int width = pTileLayer->GetWidth();
		int height = pTileLayer->GetHeight();

		for (int x = 0; x < width; ++x)
		{
			for (int y = 0; y < height; ++y)
			{
				int index = y * width + x;
				if (pTileLayer->GetTileTilesetIndex(x, y) != -1)
				{
					int tileId = pTileLayer->GetTileId(x, y);
					int tileGid = pTileLayer->GetTileGid(x, y);
					int tilesetId = pTileLayer->GetTileTilesetIndex(x, y);

					auto pTileset = m_pMap->GetTileset(tilesetId);
					const Tmx::Tile* pTile = pTileset->GetTile(tileId);
					if (pTile == nullptr)
						continue;

					std::vector<Tmx::Object*> vObjects = pTile->GetObjects();
					for (size_t i = 0; i < vObjects.size(); ++i)
					{
						Tmx::Object* pObject = vObjects[i];
						sf::IntRect collisionRect(x * pTileset->GetTileWidth(), y * pTileset->GetTileHeight(), width, height);
						if (collisionRect.intersects(rect))
						{
							Actor* pActor = m_vTileLayerGrid[ilayer][index];
							ret.push_back(pActor);
						}
					}
				}
			}
		}
	}

	return ret;
}


void TileMap::SetupImageLayer(const Tmx::ImageLayer* pLayer) 
{
}

void TileMap::SetupObjectLayer(const Tmx::ObjectGroup* pLayer, std::vector<Actor*>& vActors)
{
	const std::vector<Tmx::Object*> vObject = pLayer->GetObjects();
	for (size_t i = 0; i < vObject.size(); ++i)
	{
		Tmx::Object* pObject = vObject[i];
		Actor* pActor = CreateObjectActor(pObject);
		vActors.push_back(pActor);
	}
}

void TileMap::SetupTileLayer(const Tmx::TileLayer* pLayer, const int& layerId, std::vector<Actor*>& vActors)
{
	int width = pLayer->GetWidth();
	int height = pLayer->GetHeight();

	m_vTileLayerGrid[layerId].resize(width * height);
	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			int index = y * width + x;
			m_vTileLayerGrid[layerId][index] = nullptr;

			if (pLayer->GetTileTilesetIndex(x, y) != -1) //TODO (daniel) for now all tiles are default tiles, they only render
			{
				TileActor* pTileActor = CreateDefaultTile(x, y, pLayer);
				vActors.push_back(pTileActor);
				m_vTileLayerGrid[layerId][index] = pTileActor;
			}
		}
	}
}

Actor* TileMap::CreateObjectActor(const Tmx::Object* pObject)
{
	std::string type = pObject->GetType();
	if (type == "Spawn")
	{
	//	SpriteActor* pSpriteActor = new Sprite
	}

	return nullptr;
}

const sf::IntRect TileMap::CreateTileTextureRect(int tileId, int tilesetId)
{
	const Tmx::Tileset* pTileset = m_pMap->GetTileset(tilesetId);

	int columns = pTileset->GetImage()->GetWidth() / pTileset->GetTileWidth();
	int rows = pTileset->GetImage()->GetHeight() / pTileset->GetTileHeight();

	sf::IntRect rect;
	rect.width = pTileset->GetTileWidth();
	rect.height = pTileset->GetTileHeight();
	rect.left = tileId % columns * pTileset->GetTileWidth();
	rect.top = tileId / columns * pTileset->GetTileWidth();

	return rect;
}

TileActor* TileMap::CreateDefaultTile(int x, int y, const Tmx::TileLayer* pTileLayer)
{
	int tileId = pTileLayer->GetTileId(x, y);
	int tilesetId = pTileLayer->GetTileTilesetIndex(x, y);

	sf::IntRect rect = CreateTileTextureRect(tileId, tilesetId);

	sf::Texture* pTexture = m_vTilesetTexture[tilesetId];
	TileActor* pTileActor = new TileActor();
	pTileActor->SetTexture(pTexture);
	pTileActor->SetTextureRect(rect);
	pTileActor->SetPosition((float)x * m_pMap->GetTileWidth(), (float)y * m_pMap->GetTileHeight());

	return pTileActor;
}
