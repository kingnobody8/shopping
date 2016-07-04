#include "tilemap.h"
#include "gfx_util.h"
#include "player.h"
#include "shopper.h"
#include "text_actor.h"
#include <assert.h>


TileMap::TileMap()
	: m_pMap(nullptr)
	, m_pPlayer(nullptr)
{
}

TileMap::~TileMap()
{
}

bool TileMap::Init(const std::string& szMapPath, ItemManager* pTtemManager)
{
	Exit();

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
		case Tmx::LayerType::TMX_LAYERTYPE_OBJECTGROUP: SetupObjectLayer(static_cast<Tmx::ObjectGroup*>(pLayer), i); break;
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

	for (size_t i = 0; i < m_vActors.size(); ++i)
	{
		delete m_vActors[i];
	}
	m_vActors.clear();
	m_vLayerData.clear();

	delete m_pMap;
	m_pMap = nullptr;

	m_pPlayer = nullptr;
}

int TileMap::GetTileWidth() const
{
	return m_pMap->GetTileWidth();
}

int TileMap::GetTileHeight() const
{
	return m_pMap->GetTileHeight();
}

int TileMap::GetWidth() const
{
	return m_pMap->GetWidth() * m_pMap->GetTileWidth();
}

int TileMap::GetHeight() const
{
	return m_pMap->GetHeight() * m_pMap->GetTileHeight();
}

GridEntity* TileMap::GetGridEntityAtTilePos(const int& layerId, const int& x, const int& y) const
{
	return m_vLayerData[layerId].m_vNodes[x][y].pGridEntity;
}

std::vector<GridEntity*> TileMap::GetGridEntitiesAtTilePos(const int& x, const int& y) const
{
	std::vector<GridEntity*> ents;
	ents.resize(m_vLayerData.size());
	for (size_t i = 0; i < m_vLayerData.size(); ++i)
	{
		ents[i] = m_vLayerData[i].m_vNodes[x][y].pGridEntity;
	}
	return ents;
}

void TileMap::Update(float dt)
{
	for (size_t i = 0; i < m_vActors.size(); ++i)
	{
		m_vActors[i]->Update(dt);
	}
}

void TileMap::Draw(sf::RenderWindow& window)
{
	for (size_t i = 0; i < m_vActors.size(); ++i)
	{
		Actor* pActor = m_vActors[i];
		pActor->Draw(window);
	}
}

void TileMap::SetupImageLayer(const Tmx::ImageLayer* pLayer)
{
}

void TileMap::SetupObjectLayer(const Tmx::ObjectGroup* pLayer, int layerId)
{
	const std::vector<Tmx::Object*>& vObject = pLayer->GetObjects();
	m_vLayerData[layerId].m_vObjects.resize(vObject.size());
	for (size_t i = 0; i < vObject.size(); ++i)
	{
		Tmx::Object* pObject = vObject[i];
		Actor* actor = CreateObjectActor(pObject);
		if (actor != nullptr)
		{
			m_vLayerData[layerId].m_vObjects[i] = actor;
			m_vActors.push_back(actor);
		}
		else
		{
			printf("error: cannot create object: %s\n", pObject->GetName().c_str());
		}

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
						if (pGridEntity != nullptr)
						{
							m_vLayerData[layerId].m_vNodes[x][y].pGridEntity = pGridEntity;
							m_vActors.push_back(pGridEntity);
						}
					}
					else if (propSet.HasProperty("item_type"))
					{
						std::string szItemType = propSet.GetStringProperty("item_type");
						ItemActor* pItemActor = CreateItemActor(x, y, layerId, pLayer, szItemType);
						if (pItemActor != nullptr)
						{
							m_vLayerData[layerId].m_vNodes[x][y].pGridEntity = pItemActor;
							m_vActors.push_back(pItemActor);
						}
					}
				}
				else
				{
					TileActor* pTileActor = CreateDefaultTile(x, y, pLayer);
					if (pTileActor != nullptr)
					{
						m_vLayerData[layerId].m_vNodes[x][y].pGridEntity = pTileActor;
						m_vActors.push_back(pTileActor);
					}
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
	//if (type == "Spawn")
	//{
	//	const Tmx::PropertySet& props = pObject->GetProperties();
	//	std::string spawnEntity = props.GetStringProperty("Entity");
	//	SpriteActor* spawned = nullptr;

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
	//	m_vObjectActors.push_back(itemActor); //TODO (daniel) remove this when we have handling for multiple object types
	//	actor = itemActor;
	//}
	if (type == "Text")
	{
		const Tmx::PropertySet& props = pObject->GetProperties();
		std::string text = props.GetStringProperty("Text");

		TextActor* textActor = CreateActor<TextActor>();
		textActor->m_text.setString(text);
		textActor->m_text.setPosition(pObject->GetX(), pObject->GetY());

		int size = props.GetIntProperty("Size", textActor->m_text.getCharacterSize());
		textActor->m_text.setCharacterSize(size);

		std::string align = props.GetStringProperty("Align");
		if (align == "Center")
		{
			sf::FloatRect bounds = textActor->m_text.getLocalBounds();
			textActor->m_text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);

			textActor->m_text.move(pObject->GetWidth() / 2.0f, pObject->GetHeight() / 2.0f);
		}

		actor = textActor;
	}

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
		spawn = CreateActor<Shopper>();
	}

	if (spawn)
	{
		AddGridEntityToGrid(spawn, layerId, x, y);
	}

	return spawn;
}

ItemActor* TileMap::CreateItemActor(int x, int y, int layerId, const Tmx::TileLayer* pTileLayer, const std::string& szItemType)
{
	ItemActor* pItemActor = CreateActor<ItemActor>();

	AddGridEntityToGrid(pItemActor, layerId, x, y);

	pItemActor->Init(szItemType, 300); //TODO (daniel) figure out where cost should be init

	//TODO (remove this eventually
	{
		int tileId = pTileLayer->GetTileId(x, y);
		int tilesetId = pTileLayer->GetTileTilesetIndex(x, y);

		sf::IntRect rect = CreateTileTextureRect(tileId, tilesetId);
		const Tmx::Tileset* pTileset = m_pMap->GetTileset(tilesetId);
		const Tmx::Tile* pTile = pTileset->GetTile(tileId);

		sf::Texture* pTexture = m_vTilesetTexture[tilesetId];
		pItemActor->SetTexture(pTexture);
		pItemActor->SetTextureRect(rect);
		pItemActor->SetPosition(sf::Vector2f(x * m_pMap->GetTileWidth(), y * m_pMap->GetTileHeight()));
	}

	return pItemActor;
}

void TileMap::AddGridEntityToGrid(GridEntity* pGridEnt, const int& layerId, const int& x, const int& y)
{
	GridNode* pGridNode = &m_vLayerData[layerId].m_vNodes[x][y];
	assert(pGridNode->pGridEntity == nullptr); //can't have two grid entites on the same node
	pGridNode->pGridEntity = pGridEnt;
	pGridEnt->SetGridNode(pGridNode, layerId);
	pGridEnt->AlignPositionToNode();
}