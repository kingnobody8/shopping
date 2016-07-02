#pragma once

#include "actor.h"

class BoxActor : public Actor
{
public:
	BoxActor();
	virtual ~BoxActor();

	virtual void Update(float dt);
	virtual void Draw(sf::RenderWindow& window);

private:
	sf::RectangleShape m_shape;
};