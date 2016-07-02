#pragma once
#include "actor.h"
#include "Tmx.h.in"

class TileMapActor : public Actor
{
public:
	TileMapActor();
	TileMapActor(Tmx::Map* pMap);
	virtual ~TileMapActor();

	virtual void Update(float dt);
	virtual void Draw(sf::RenderWindow& window);

	void LoadAssets();
	void UnloadAssets();

private:
	sf::Sprite m_sprite;
	sf::Texture m_texture;
	Tmx::Map* m_pMap;
};