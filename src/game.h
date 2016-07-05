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
	Game(sf::View& uiView, sf::View& gameView);

	~Game();

	void Init();
	void NewGame();
	void EndGame();

	void OnKeyReleased(sf::Keyboard::Key key);

	bool IsPlaying() const { return m_isPlaying; }
	bool IsOver() const { return m_isOver; }
	bool IsStarted() const { return m_isPlaying || m_isOver; }

	void DebugPrintItemActor(ItemActor* itemActor);

private:
	Item* FindItemByName(const std::string& name);

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

	struct ItemData
	{
		ItemData(int tileId, Item* item)
			: m_tileId(tileId)
			, m_item(item)
			, m_discount(1.0f)
			, m_stock(0)
		{}

		int m_tileId;
		Item* m_item;
		float m_discount;
		int m_stock;
	};

	struct StoreData
	{
		ItemData* FindItemDataById(int id)
		{
			for (size_t i = 0; i < m_items.size(); ++i)
			{
				if (m_items[i].m_tileId == id)
				{
					return &m_items[i];
				}
			}
			return nullptr;
		}
		std::vector<ItemData> m_items;
	};

	std::map<std::string, StoreData> m_stores;
	std::vector<std::string> m_itemAvailability[Item::EType::ET_COUNT][Item::EAdjective::EA_COUNT];

	bool m_isPlaying;
	bool m_isOver;
	int m_timeRemaining;
	int m_totalTime;
	int m_targetTime;
	int m_startingMoney;
	int m_targetMoney;
	Customer m_player;

	sf::View* m_uiView;
	sf::View* m_gameView;

	int m_baseItemCosts[Item::EType::ET_COUNT];
	float m_adjItemCostModifiers[Item::EAdjective::EA_COUNT];
	std::map<std::string, Item::EType> m_stringToItemType;
	std::map<std::string, Item::EAdjective> m_stringToItemAdjective;
	Item* m_itemDatabase[Item::EType::ET_COUNT][Item::EAdjective::EA_COUNT];
};