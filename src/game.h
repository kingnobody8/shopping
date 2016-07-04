#pragma once
#include <SFML/Window.hpp>
#include "customer.h"
#include "tilemap.h"

bool LoadLevel(const std::string& path);
void UnloadLevel();
bool LoadUi(const std::string& path);
void UnloadUi(const std::string& path);

class Game
{
public:
	Game();

	void NewGame();
	void EndGame();

	void OnKeyReleased(sf::Keyboard::Key key);

	bool IsPlaying() const { return m_isPlaying; }
	bool IsOver() const { return m_isOver; }
	bool IsStarted() const { return m_isPlaying || m_isOver; }

private:
	bool m_isPlaying;
	bool m_isOver;
	float m_timeRemaining;
	Customer m_player;

	Item blue_milk;
	Item green_eggs;
	Item white_meat;
	Item red_candy;
	Item blue_eggs;

	GroceryList gc;
};