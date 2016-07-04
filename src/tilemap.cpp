#include "tilemap.h"
#include "gfx_util.h"
#include "player.h"
#include "shopper.h"
#include <assert.h>


TileMap::TileMap()
	: m_pMap(nullptr)
	, m_pPlayer(nullptr)
{
}


bool TileMap::Init(const std::string& szMapPath, ItemManager* pTtemManager)
{
	m_pItemManager = m_pItemManager;

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
	m_vLayerData.resize(vLayer.size());
	for (size_t i = 0; i < vLayer.size(); ++i)
	{
		Tmx::Layer* pLayer = vLayer[i];
		switch (pLayer->GetLayerType())
		{
		case Tmx::LayerType::TMX_LAYERTYPE_IMAGE_LAYER:	SetupImageLayer(static_cast<Tmx::ImageLayer*>(pLayer));	break;
		case Tmx::LayerType::TMX_LAYERTYPE_OBJECTGROUP: SetupObjectLayer(static_cast<Tmx::ObjectGroup*>(pLayer)); break;
		case Tmx::LayerType::TMX_LAYERTYPE_TILE: SetupTileLayer(static_cast<Tmx::TileLayer*>(pLayer), i); break;
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
							Actor* pActor = m_vLayerData[ilayer].m_vNodes[x][y].pGridEntity;
							ret.push_back(pActor);
						}
					}
				}
			}
		}
	}

	//for (size_t i = 0; i < m_vObjectActors.size(); ++i)
	//{
	//	//TODO (daniel) for now assume all objects are items
	//	ItemActor* pItemActor = static_cast<ItemActor*>(m_vObjectActors[i]);

	//	sf::IntRect collisionRect = pItemActor->GetRect();
	//	if (collisionRect.intersects(rect))
	//	{
	//		ret.push_back(pItemActor);
	//	}
	//}

	return ret;
}

int TileMap::GetTileWidth() const
{
	return m_pMap->GetTileWidth();
}

int TileMap::GetTileHeight() const
{
	return m_pMap->GetTileHeight();
}

Actor* TileMap::GetTileActorAt(int x, int y, int layer) const
{
	return m_vLayerData[layer].m_vNodes[x][y].pGridEntity;
}


void TileMap::SetupImageLayer(const Tmx::ImageLayer* pLayer) 
{
}

void TileMap::SetupObjectLayer(const Tmx::ObjectGroup* pLayer)
{
	const std::vector<Tmx::Object*>& vObject = pLayer->GetObjects();
	for (size_t i = 0; i < vObject.size(); ++i)
	{
		Tmx::Object* pObject = vObject[i];
		CreateObjectActor(pObject);
	}
}

void TileMap::SetupTileLayer(const Tmx::TileLayer* pLayer, const int& layerId)
{
	int width = pLayer->GetWidth();
	int height = pLayer->GetHeight();

	m_vLayerData[layerId].SetGridSize(width, height);
	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			m_vLayerData[layerId].m_vNodes[x][y].grid_position = sf::Vector2i(x, y);
			m_vLayerData[layerId].m_vNodes[x][y].position = sf::Vector2i(x * m_pMap->GetTileWidth() + m_pMap->GetTileWidth() / 2, y * m_pMap->GetTileHeight() + m_pMap->GetTileHeight() / 2);
			if (pLayer->GetTileTilesetIndex(x, y) != -1) //TODO (daniel) for now all tiles are default tiles, they only render
			{
				assert(m_vLayerData[layerId].m_vNodes[x][y].pGridEntity == nullptr); //two grid entities can't be on the same node!

				int tileId = pLayer->GetTileId(x, y);
				int tilesetId = pLayer->GetTileTilesetIndex(x, y);
				const Tmx::Tileset* pTileset = m_pMap->GetTileset(tilesetId);
				const Tmx::Tile* pTile = pTileset->GetTile(tileId);
				if (pTile != nullptr)
				{
					const Tmx::PropertySet& propSet = pTile->GetProperties();

					if (propSet.HasProperty("spawn_type"))
					{
						std::string szSpawnType = propSet.GetStringProperty("spawn_type");
						GridEntity* pGridEntity = CreateSpawnTile(x, y, layerId, pLayer, szSpawnType);
						m_vLayerData[layerId].m_vNodes[x][y].pGridEntity = pGridEntity;
					}
				}
				else
				{
					TileActor* pTileActor = CreateDefaultTile(x, y, pLayer);
					m_vLayerData[layerId].m_vNodes[x][y].pGridEntity = pTileActor;
				}
			}
		}
	}
}

Actor* TileMap::CreateObjectActor(Tmx::Object* pObject)
{
	std::string type = pObject->GetType();
	Actor* actor = nullptr;
	//if (type == "Spawn")
	//{
	//	const Tmx::PropertySet& props = pObject->GetProperties();
	//	std::string spawnEntity = props.GetStringProperty("Entity");
	//	SpriteActor* spawned = nullptr;
	//
	//	if (spawnEntity == "Player")
	//	{
	//		assert(m_pPlayer == nullptr);
	//		m_pPlayer = CreateActor<Player>();
	//		spawned = m_pPlayer;
	//	}
	//	else if (spawnEntity == "Shopper")
	//	{
	//		spawned = CreateActor<Shopper>();
	//	}
	//
	//	if (spawned)
	//	{
	//		sf::IntRect rect = spawned->GetRect();
	//		spawned->SetPosition(sf::Vector2f(pObject->GetX() + pObject->GetWidth() / 2.0f - rect.width / 2.0f, pObject->GetY() + pObject->GetHeight() / 2.0f - rect.height / 2.0f));
	//		actor = spawned;
	//	}
	//}
	//else if (type == "ItemCollider")
	//{
	//	ItemActor* itemActor = new ItemActor();
	//	itemActor->Init(pObject);
	//	//ItemActor* itemActor = new ItemActor();
		//itemActor->Init(pObject);
		//m_vObjectActors.push_back(itemActor); //TODO (daniel) remove this when we have handling for multiple object types
		//return itemActor;
	//}

	return actor;
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
	const Tmx::Tileset* pTileset = m_pMap->GetTileset(tilesetId);
	const Tmx::Tile* pTile = pTileset->GetTile(tileId);

	sf::Texture* pTexture = m_vTilesetTexture[tilesetId];
	TileActor* pTileActor = CreateActor<TileActor>();
	pTileActor->SetTexture(pTexture);
	pTileActor->SetTextureRect(rect);
	pTileActor->SetPosition(sf::Vector2f(x * m_pMap->GetTileWidth(), y * m_pMap->GetTileHeight()));

	return pTileActor;
}

GridEntity* TileMap::CreateSpawnTile(int x, int y, int layerId, const Tmx::TileLayer* pTileLayer, const std::string& szSpawnType)
{
	GridEntity* spawn = nullptr;
	if (szSpawnType == "player")
	{
		assert(m_pPlayer == nullptr);
		m_pPlayer = CreateActor<Player>();
		spawn = m_pPlayer;
	}
	else if (szSpawnType == "shopper")
	{

	}

	if (spawn)
	{
		int width = m_pMap->GetTileWidth();
		int height = m_pMap->GetTileHeight();
		sf::IntRect rect = spawn->GetRect();
		spawn->SetPosition(sf::Vector2f(x * width + width / 2.0f - rect.width / 2.0f, y * height + height / 2.0f - rect.height / 2.0f));

		GridNode* pGridNode = &m_vLayerData[layerId].m_vNodes[x][y];
		assert(pGridNode->pGridEntity == nullptr); //can't have two grid entites on the same node
		pGridNode->pGridEntity = spawn;
		spawn->SetGridNode(pGridNode, layerId);
		spawn->AlignPositionToNode();
	}

	return spawn;
}