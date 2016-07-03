#pragma once
#include "tile_actor.h"
#include "Tmx.h.in"
#include <map>
#include "player.h"
#include "item_manager.h"


class TileMap
{
public:
	TileMap();

	bool Init(const std::string& szMapPath, std::vector<Actor*>& vActorsInitalList, ItemManager* pItemManager);
	void Exit();

	inline Player* GetPlayer() const { return m_pPlayer; }

	std::vector<Actor*> PerformCollisionTest(const sf::IntRect& rect);

private:
	void SetupImageLayer(const Tmx::ImageLayer* pLayer);
	void SetupObjectLayer(const Tmx::ObjectGroup* pLayer, std::vector<Actor*>& vActors);
	void SetupTileLayer(const Tmx::TileLayer* pLayer, const int& layerId, std::vector<Actor*>& vActors);
	Actor* CreateObjectActor(Tmx::Object* pObject);
	const sf::IntRect CreateTileTextureRect(int tileId, int tilesetId);
	TileActor* CreateDefaultTile(int x, int y, const Tmx::TileLayer* pTileLayer);

private:
	Tmx::Map* m_pMap;
	std::vector<sf::Texture*> m_vTilesetTexture;
	std::vector<Actor*> m_vActors;
	std::vector<std::vector<Actor*>> m_vTileLayerGrid;
	std::vector<Actor*> m_vObjectActors;

	ItemManager* m_pItemManager;
	Player* m_pPlayer;
};