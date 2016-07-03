#pragma once
#include "actor.h"

class TileActor : public Actor
{
public:
	TileActor();
	virtual ~TileActor();

	virtual void Update(float dt);
	virtual void Draw(sf::RenderWindow& window);

	void SetTexture(sf::Texture* pTexture);
	void SetTextureRect(const sf::IntRect& rect);
	void SetPosition(float x, float y);

	sf::IntRect GetRect() const;

private:
	sf::Sprite m_sprite;

};