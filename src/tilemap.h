#pragma once
#include "tile_actor.h"
#include "Tmx.h.in"
#include <map>
#include "player.h"
#include "item_manager.h"
#include <assert.h>

const int MAX_GRID_SIZE = 128;

//forward declare
class GridEntity;

struct GridNode
{
	GridNode* pUp;
	GridNode* pDown;
	GridNode* pLeft;
	GridNode* pRight;

	GridEntity* pGridEntity;

	sf::Vector2i grid_position;
	sf::Vector2i position;

	GridNode() : pUp(nullptr), pDown(nullptr), pLeft(nullptr), pRight(nullptr), pGridEntity(nullptr) {}
};

struct LayerData
{
	GridNode m_vNodes[MAX_GRID_SIZE][MAX_GRID_SIZE];
	std::vector<Actor*> m_vObjects;
	sf::Vector2i m_gridSize;

	LayerData()
		: m_vNodes{ {},{} }
		, m_gridSize(MAX_GRID_SIZE, MAX_GRID_SIZE)
	{
		for (int x = 0; x < MAX_GRID_SIZE; ++x)
		{
			for (int y = 0; y < MAX_GRID_SIZE; ++y)
			{
				GridNode& tmp = m_vNodes[x][y];
				if (x > 0)
				{
					tmp.pLeft = &m_vNodes[x - 1][y];
				}
				if (x < MAX_GRID_SIZE - 1)
				{
					tmp.pRight = &m_vNodes[x + 1][y];
				}
				if (y > 0)
				{
					tmp.pUp = &m_vNodes[x][y - 1];
				}
				if (y < MAX_GRID_SIZE - 1)
				{
					tmp.pDown = &m_vNodes[x][y + 1];
				}
			}
		}
	}

	void SetGridSize(const int& width, const int& height) //TODO (daniel) come back here and fix setting the null edges
	{
		assert(width > 0 && width <= MAX_GRID_SIZE && height > 0 && height <= MAX_GRID_SIZE);
		m_gridSize = sf::Vector2i(width, height);

		for (int x = 0; x < MAX_GRID_SIZE; ++x)
		{
			for (int y = 0; y < MAX_GRID_SIZE; ++y)
			{
				GridNode& tmp = m_vNodes[x][y];
				tmp = GridNode();

				if (x > 0)
				{
					tmp.pLeft = &m_vNodes[x - 1][y];
				}
				if (x < width - 1)
				{
					tmp.pRight = &m_vNodes[x + 1][y];
				}
				if (y > 0)
				{
					tmp.pUp = &m_vNodes[x][y - 1];
				}
				if (y < height - 1)
				{
					tmp.pDown = &m_vNodes[x][y + 1];
				}
			}
		}
	}
};

const class TileMap& GetCurrentMap();
bool LoadMap(const std::string& mapPath, const std::string& view);

class TileMap
{
public:
	TileMap();
	~TileMap();

	bool Init(const std::string& szMapPath, ItemManager* pItemManager, sf::View* view);
	void Exit();

	inline Player* GetPlayer() const { return m_pPlayer; }

	int GetTileWidth() const;
	int GetTileHeight() const;

	int GetWidth() const;
	int GetHeight() const;

	GridEntity* GetGridEntityAtTilePos(const int& layerId, const int& x, const int& y) const;
	std::vector<GridEntity*> GetGridEntitiesAtTilePos(const int& x, const int& y) const;
	Actor* FindActorByName(const std::string& name) const;
	template<typename TActor>
	TActor* FindActorByName(const std::string& name) const
	{
		return static_cast<TActor*>(FindActorByName(name));
	}

	void Update(float dt);
	void Draw(sf::RenderWindow& window);

private:
	void SetupImageLayer(const Tmx::ImageLayer* pLayer);
	void SetupObjectLayer(const Tmx::ObjectGroup* pLayer, int layerId, sf::View* view);
	void SetupTileLayer(const Tmx::TileLayer* pLayer, const int& layerId);
	Actor* CreateObjectActor(Tmx::Object* pObject, sf::View* view);
	const sf::IntRect CreateTileTextureRect(int tileId, int tilesetId);
	TileActor* CreateDefaultTile(int x, int y, const Tmx::TileLayer* pTileLayer);
	GridEntity* CreateSpawnTile(int x, int y, int layerId, const Tmx::TileLayer* pTileLayer, const std::string& szSpawnType);
	ItemActor* CreateItemActor(int x, int y, int layerId, const Tmx::TileLayer* pTileLayer, const std::string& szItemType);

	void AddGridEntityToGrid(GridEntity* pGridEnt, const int& layerId, const int& x, const int& y);

private:
	Tmx::Map* m_pMap;
	std::vector<sf::Texture*> m_vTilesetTexture;
	std::vector<Actor*> m_vActors;			//list of all actors instantiated during map load
	std::vector<LayerData> m_vLayerData;	//list of layers with grid data

	ItemManager* m_pItemManager;
	Player* m_pPlayer;
};