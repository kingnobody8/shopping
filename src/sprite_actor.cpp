#include "sprite_actor.h"

SpriteActor::SpriteActor()
{
	m_szType = "SpriteActor";
}

SpriteActor::~SpriteActor()
{}

void SpriteActor::Update(float dt)
{
}

void SpriteActor::Draw(sf::RenderWindow& window)
{
	window.draw(m_sprite);

	sf::IntRect rect = GetRect();

	sf::VertexArray verts;
	verts.resize(5);
	verts[0].position.x = rect.left;
	verts[0].position.y = rect.top;
	verts[1].position.x = rect.left + rect.width;
	verts[1].position.y = rect.top;
	verts[2].position.x = rect.left + rect.width;
	verts[2].position.y = rect.top + rect.height;
	verts[3].position.x = rect.left;
	verts[3].position.y = rect.top + rect.height;
	verts[4].position.x = rect.left;
	verts[4].position.y = rect.top;

	for (size_t i = 0; i < verts.getVertexCount(); ++i)
	{
		verts[i].color = sf::Color::Black;
	}

	window.draw(&verts[0], verts.getVertexCount(), sf::PrimitiveType::LinesStrip);
}

sf::IntRect SpriteActor::GetRect()
{
	sf::IntRect rect = m_sprite.getTextureRect();
	rect.left = (int) m_sprite.getPosition().x;
	rect.top = (int) m_sprite.getPosition().y;
	return rect;
}