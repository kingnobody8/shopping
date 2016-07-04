#pragma once

#include <assert.h>

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
	// Some actors may have rectangles, but those that don't should just hit this assert
	virtual sf::IntRect GetRect() { assert(false); return sf::IntRect(); }

	std::string m_name;

protected:
	std::string m_szType;
};