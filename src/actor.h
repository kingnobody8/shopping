#pragma once

#include <SFML/Graphics.hpp>

class Actor;

void RegisterActor(Actor* actor);
void DestroyActor(Actor* actor);

template<typename TActor>
inline TActor* CreateActor()
{
	TActor* actor = new TActor();
	RegisterActor(actor);
	return actor;
}

class Actor
{
public:
	Actor() {}
	virtual ~Actor() {}

	virtual void Update(float dt) {}
	virtual void Draw(sf::RenderWindow& window) {}
};