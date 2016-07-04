#pragma once

#include "actor.h"

extern sf::Font* g_defaultFont;

class TextActor : public Actor
{
public:
	TextActor(const sf::Font& font = *g_defaultFont);
	~TextActor();

	virtual void Draw(sf::RenderWindow& window);
	virtual sf::IntRect GetRect();

	sf::Text m_text;
};