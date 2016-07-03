#pragma once
#include "actor.h"
#include "Tmx.h.in"

class ItemActor : public Actor
{
public:
	ItemActor();
	virtual ~ItemActor();

	virtual void Update(float dt);
	virtual void Draw(sf::RenderWindow& window);

	sf::IntRect GetRect() const;

private:
	Tmx::Object* m_pObject;
};