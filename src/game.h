#pragma once
#include <SFML/Window.hpp>
#include "customer.h"
#include "event.h"
#include "tilemap.h"

bool LoadLevel(const std::string& path, sf::View* view);
void UnloadLevel();
bool LoadUi(const std::string& path, sf::View* view);
void UnloadUi(const std::string& path);
TileMap* FindUi(const std::string& path);

class Game
{
public:
	Game();
	Game(sf::View& uiView, sf::View& gameView);

	~Game();

	void Init();
	void NewGame();
	void EndGame();

	void OnKeyReleased(sf::Keyboard::Key key);

	bool IsPlaying() const { return m_isPlaying; }
	bool IsOver() const { return m_isOver; }
	bool IsStarted() const { return m_isPlaying || m_isOver; }

private:
	enum ScoreRank
	{
		RankSSS,
		RankSS,
		RankS,
		RankA,
		RankB,
		RankC,
		RankD,
		RankE,
	};

	bool m_isPlaying;
	bool m_isOver;
	int m_timeRemaining;
	int m_totalTime;
	int m_targetTime;
	int m_startingMoney;
	int m_targetMoney;
	Customer m_player;

	Item blue_milk;
	Item green_eggs;
	Item white_meat;
	Item red_candy;
	Item blue_eggs;
	sf::View* m_uiView;
	sf::View* m_gameView;
};