#pragma once
#include "tile_actor.h"
#include "Tmx.h.in"

class TileMap
{
public:
	TileMap();

	bool Init(const std::string& szMapPath, std::vector<Actor*>& vActorsInitalList);
	void Exit();

private:
	//void FillOutDefaultTile(TileActor* pTileActor);

private:
	Tmx::Map* m_pMap;
	std::vector<sf::Texture*> m_vTilesetTexture;
	std::vector<Actor*> m_vActors;
};