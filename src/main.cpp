#include <SFML\Graphics.hpp>
#include "box_actor.h"
#include <vector>

int main(int argc, char** argv)
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Shopping Game", sf::Style::Default);
	window.setActive();

	std::vector<Actor*> m_actors;

	m_actors.push_back(new BoxActor);

	sf::Clock clock;
	while (window.isOpen())
	{
		float dt = clock.restart().asSeconds();

		// Poll events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			{
				window.close();
			}
		}

		// Update actors
		for (size_t i = 0; i < m_actors.size(); ++i)
		{
			m_actors[i]->Update(dt);
		}

		// Clear
		window.clear(sf::Color(50, 75, 50));

		// Draw actors
		for (size_t i = 0; i < m_actors.size(); ++i)
		{
			m_actors[i]->Draw(window);
		}

		// Display window
		window.display();
	}

	return 0;
}