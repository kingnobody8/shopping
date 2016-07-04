#pragma once
#include "grid_entity.h"

//TODO (daniel) make this inherit from sprite actor
class TileActor : public GridEntity
{
public:
	TileActor();
	virtual ~TileActor();

	virtual void Update(float dt);

	void SetTexture(sf::Texture* pTexture);
	void SetTextureRect(const sf::IntRect& rect);

protected:

};