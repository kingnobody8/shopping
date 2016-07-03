#pragma once
#include "actor.h"

//TODO (daniel) make this inherit from sprite actor
class TileActor : public Actor
{
public:
	TileActor();
	virtual ~TileActor();

	virtual void Update(float dt);
	virtual void Draw(sf::RenderWindow& window);

	void SetTexture(sf::Texture* pTexture);
	void SetTextureRect(const sf::IntRect& rect);
	void SetPosition(int x, int y);

	sf::IntRect GetRect() const;

private:
	sf::Sprite m_sprite;

};