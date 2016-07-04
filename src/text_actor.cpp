#include "text_actor.h"

TextActor::TextActor(const sf::Font& font)
{
	m_szType = "TextActor";
	m_text.setFont(font);
}

TextActor::~TextActor()
{
}

void TextActor::Draw(sf::RenderWindow& window)
{
	window.draw(m_text);
}

sf::IntRect TextActor::GetRect()
{
	return sf::IntRect(m_text.getGlobalBounds());
}
