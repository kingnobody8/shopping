#pragma once
#include "tile_actor.h"
#include "Tmx.h.in"
#include <map>
#include "player.h"


class TileMap
{
public:
	TileMap();

	bool Init(const std::string& szMapPath);
	void Exit();

	inline Player* GetPlayer() const { return m_pPlayer; }

	std::vector<Actor*> PerformCollisionTest(const sf::IntRect& rect);

private:
	void SetupImageLayer(const Tmx::ImageLayer* pLayer);
	void SetupObjectLayer(const Tmx::ObjectGroup* pLayer);
	void SetupTileLayer(const Tmx::TileLayer* pLayer, const int& layerId);
	Actor* CreateObjectActor(const Tmx::Object* pObject);
	const sf::IntRect CreateTileTextureRect(int tileId, int tilesetId);
	TileActor* CreateDefaultTile(int x, int y, const Tmx::TileLayer* pTileLayer);

private:
	Tmx::Map* m_pMap;
	std::vector<sf::Texture*> m_vTilesetTexture;
	std::vector<Actor*> m_vActors;
	std::vector<std::vector<Actor*>> m_vTileLayerGrid;

	Player* m_pPlayer;
};