#include "box_actor.h"

BoxActor::BoxActor()
{
	sf::Vector2f size(50, 50);
	m_shape.setSize(size - sf::Vector2f(3, 3));
	m_shape.setOutlineThickness(3);
	m_shape.setOutlineColor(sf::Color::Black);
	m_shape.setFillColor(sf::Color(100, 100, 200));
	m_shape.setOrigin(size / 2.f);
}

BoxActor::~BoxActor()
{}

void BoxActor::Update(float dt)
{
	float speed = 400.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_shape.move(-speed * dt, 0);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_shape.move(speed * dt, 0);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		m_shape.move(0, -speed * dt);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		m_shape.move(0, speed * dt);
	}
}

void BoxActor::Draw(sf::RenderWindow& window)
{
	window.draw(m_shape);
}