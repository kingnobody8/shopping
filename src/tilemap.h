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

	LayerData() //TODO (daniel) come back here and fix setting the null edges
		: m_vNodes{ {},{} }
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
class TileMap
{
public:
	TileMap();

	bool Init(const std::string& szMapPath, ItemManager* pItemManager);
	void Exit();

	inline Player* GetPlayer() const { return m_pPlayer; }

	std::vector<Actor*> PerformCollisionTest(const sf::IntRect& rect);

	int GetTileWidth() const;
	int GetTileHeight() const;

	Actor* GetTileActorAt(int x, int y, int layer) const;

private:
	void SetupImageLayer(const Tmx::ImageLayer* pLayer);
	void SetupObjectLayer(const Tmx::ObjectGroup* pLayer);
	void SetupTileLayer(const Tmx::TileLayer* pLayer, const int& layerId);
	Actor* CreateObjectActor(Tmx::Object* pObject);
	const sf::IntRect CreateTileTextureRect(int tileId, int tilesetId);
	TileActor* CreateDefaultTile(int x, int y, const Tmx::TileLayer* pTileLayer);
	GridEntity* CreateSpawnTile(int x, int y, int layerId, const Tmx::TileLayer* pTileLayer, const std::string& szSpawnType);

private:
	Tmx::Map* m_pMap;
	std::vector<sf::Texture*> m_vTilesetTexture;
	std::vector<Actor*> m_vActors;
	std::vector<LayerData> m_vLayerData;

	ItemManager* m_pItemManager;
	Player* m_pPlayer;
};