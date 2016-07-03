#pragma once
#include "tile_actor.h"
#include "Tmx.h.in"
#include <map>

class TileMap
{
public:
	TileMap();

	bool Init(const std::string& szMapPath, std::vector<Actor*>& vActorsInitalList);
	void Exit();

	std::vector<Actor*> PerformCollisionTest(const sf::IntRect& rect);

private:
	//void FillOutDefaultTile(TileActor* pTileActor);

private:
	Tmx::Map* m_pMap;
	std::vector<sf::Texture*> m_vTilesetTexture;
	std::vector<Actor*> m_vActors;
	std::vector<std::vector<Actor*>> m_vLayerGrid;
};