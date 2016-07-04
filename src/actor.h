#pragma once

#include <SFML/Graphics.hpp>

class Actor;

template<typename TActor>
inline TActor* CreateActor()
{
	TActor* actor = new TActor();
	return actor;
}

class Actor
{
public:
	Actor() : m_szType("Actor") {}
	virtual ~Actor() {}

	virtual void Update(float dt) {}
	virtual void Draw(sf::RenderWindow& window) {}

	const std::string& GetType() { return m_szType; }

protected:
	std::string m_szType;
};