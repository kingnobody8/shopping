#include "game.h"
#include "text_actor.h"
#include "tokenizer.h"
#include "debug_text.h"

static void SetLevel(const std::string& path, sf::View* view)
{
	UnloadUi("assets/main_menu.tmx");
	UnloadUi("assets/end_game.tmx");
	LoadLevel(path, view);
	printf("Entering %s...\n", path.c_str());
}

static void PrintCustomer(Customer& c)
{
	printf("Customer Wallet: %d\n", c.GetMunny());
	c.PrintGroceryList();
	c.PrintInventory();
}

static void AddItemAttempt(Customer& c, Item& i)
{
	printf("Adding item: %d %s\n", i.GetCost(), i.GetItemName().c_str());
	printf("Customer wallet: %d\n", c.GetMunny());
	bool canAdd = c.CanAddItem(i);
	printf("CanAddItem: %d\n", canAdd);

	if (canAdd)
	{
		c.AddItem(i);
		PrintCustomer(c);
	}
	printf("\n\n");
}

static const int MAX_FLAVORS = 1;
static const char* s_flavorTexts[][MAX_FLAVORS] =
{
	{
		"You are the best!"
	},
	{
		"A god amongst shoppers."
	},
	{
		"Professional shopper."
	},
	{
		"Grade A shopper!"
	},
	{
		"Average shopper."
	},
	{
		"Casual shopper."
	},
	{
		"Your wife left you."
	},
	{
		"What a disaster!"
	}
};

static const char* s_ranks[] =
{
	"SSS",
	"SS",
	"S",
	"A",
	"B",
	"C",
	"D",
	"E",
};

static const sf::Color s_rankColors[] =
{
	{208, 211, 212},
	{208, 211, 212},
	{208, 211, 212},
	{241, 196, 15},
	{52, 152, 219},
	{155, 89, 182},
	{192, 57, 43},
	{231, 76, 60},
};

Game::Game(sf::View& uiView, sf::View& gameView)
	: m_isPlaying(false)
	, m_isOver(false)
	, m_timeRemaining(0)
	, m_totalTime(0)
	, m_targetTime(0)
	, m_startingMoney(0)
	, m_targetMoney(0)
	, m_uiView(&uiView)
	, m_gameView(&gameView)
{
	m_baseItemCosts[Item::EType::ET_MILK ] = 150;
	m_baseItemCosts[Item::EType::ET_EGGS ] = 200;
	m_baseItemCosts[Item::EType::ET_FRUIT] = 100;
	m_baseItemCosts[Item::EType::ET_MEAT ] = 400;
	m_baseItemCosts[Item::EType::ET_SODA ] = 50;
	m_baseItemCosts[Item::EType::ET_CANDY] = 75;

	m_adjItemCostModifiers[Item::EAdjective::EA_BLUE ] = 1.0f;
	m_adjItemCostModifiers[Item::EAdjective::EA_RED	 ] = 1.25f;
	m_adjItemCostModifiers[Item::EAdjective::EA_GREEN] = 1.5f;
	m_adjItemCostModifiers[Item::EAdjective::EA_WHITE] = 1.75f;

	m_stringToItemType["milk"] = Item::EType::ET_MILK;
	m_stringToItemType["eggs"] = Item::EType::ET_EGGS;
	m_stringToItemType["fruit"] = Item::EType::ET_FRUIT;
	m_stringToItemType["meat"] = Item::EType::ET_MEAT;
	m_stringToItemType["soda"] = Item::EType::ET_SODA;
	m_stringToItemType["candy"] = Item::EType::ET_CANDY;

	m_stringToItemAdjective["blue"] = Item::EAdjective::EA_BLUE;
	m_stringToItemAdjective["red"] = Item::EAdjective::EA_RED;
	m_stringToItemAdjective["green"] = Item::EAdjective::EA_GREEN;
	m_stringToItemAdjective["white"] = Item::EAdjective::EA_WHITE;

	for (int itemType = Item::EType::ET_INVALID + 1; itemType != Item::EType::ET_COUNT; ++itemType)
	{
		for (int itemAdj = Item::EAdjective::EA_INVALID + 1; itemAdj != Item::EAdjective::EA_COUNT; ++itemAdj)
		{
			int cost = (int) (m_baseItemCosts[itemType] * m_adjItemCostModifiers[itemAdj]);
			m_itemDatabase[itemType][itemAdj] = new Item((Item::EAdjective) itemAdj, (Item::EType) itemType, cost);
		}
	}
}

Game::~Game()
{
	UnRegisterEvent("START_GAME");
}

void Game::Init()
{
	RegisterEvent("START_GAME", [this](void *x) {
		EndGame();
		NewGame();
		SetLevel("assets/test_shop.tmx", m_uiView);
	});

	std::vector<std::string> stores =
	{
		"assets/test_shop.tmx",
	};

	for (size_t i = 0; i < stores.size(); ++i)
	{
		Tmx::Map map;
		map.ParseFile(stores[i]);
		if (map.HasError())
		{
			continue;
		}

		StoreData& storeData = m_stores[stores[i]];

		const std::vector<Tmx::Layer*>& layers = map.GetLayers();
		for (size_t j = 0; j < layers.size(); ++j)
		{
			Tmx::Layer* layer = layers[j];
			if (layer->GetName() == "items")
			{
				Tmx::TileLayer* tileLayer = (Tmx::TileLayer*) layer;
				
				int width = tileLayer->GetWidth();
				int height = tileLayer->GetHeight();

				for (int x = 0; x < width; ++x)
				{
					for (int y = 0; y < height; ++y)
					{
						if (tileLayer->GetTileTilesetIndex(x, y) != -1)
						{
							int tileId = tileLayer->GetTileId(x, y);
							int tilesetId = tileLayer->GetTileTilesetIndex(x, y);
							const Tmx::Tileset* tileset = map.GetTileset(tilesetId);
							const Tmx::Tile* tile = tileset->GetTile(tileId);
							if (tile != nullptr)
							{
								const Tmx::PropertySet& propSet = tile->GetProperties();

								if (propSet.HasProperty("item_type"))
								{
									std::string itemType = propSet.GetStringProperty("item_type");
									Item* item = FindItemByName(itemType);

									storeData.m_items.push_back(ItemData(tileId, item));
								}
							}
						}
					}
				}

				break;
			}
		}
	}
}

void Game::NewGame()
{
	printf("Starting a new game!\n");

	m_isPlaying = true;
	m_isOver = false;

	m_totalTime = 300;
	m_timeRemaining = m_totalTime;
	m_targetTime = 150;

	// Clear item availability
	for (int itemType = Item::EType::ET_INVALID + 1; itemType != Item::EType::ET_COUNT; ++itemType)
	{
		for (int itemAdj = Item::EAdjective::EA_INVALID + 1; itemAdj != Item::EAdjective::EA_COUNT; ++itemAdj)
		{
			m_itemAvailability[itemType][itemAdj].clear();
		}
	}

	// Stock stores
	for (auto itr = m_stores.begin(); itr != m_stores.end(); ++itr)
	{
		for (auto jtr = itr->second.m_items.begin(); jtr != itr->second.m_items.end(); ++jtr)
		{
			jtr->m_stock = rand() % 5;
			if (jtr->m_stock > 0)
			{
				m_itemAvailability[jtr->m_item->GetType()][jtr->m_item->GetAdj()].push_back(itr->first);
			}
		}
	}

	Item* items[Item::EType::ET_COUNT];
	memset(items, 0, sizeof(Item*) * Item::EType::ET_COUNT);
	StoreData& store = m_stores.begin()->second;
	for (auto itr = store.m_items.begin(); itr != store.m_items.end(); ++itr)
	{
		if (itr->m_stock > 0 && rand() % 2 == 1)
		{
			items[itr->m_item->GetType()] = itr->m_item;
		}
	}

	GroceryList gc = GroceryList();
	for (int itemType = Item::EType::ET_INVALID + 1; itemType != Item::EType::ET_COUNT; ++itemType)
	{
		if (items[itemType])
		{
			gc.AddItem(*items[itemType]);
		}
	}

	int costOfList = gc.GetCostOfList();
	m_startingMoney = (int) (costOfList * 1.1f);
	m_targetMoney = m_startingMoney - costOfList;

	printf("Starting with $%d\n", m_startingMoney);
	printf("Target money: $%d\n", m_targetMoney);

	m_player = Customer(gc, m_startingMoney);

	m_player.PrintGroceryList();
	m_player.PrintInventory();
}

void Game::EndGame()
{
	// if we're not playing and the game is not over
	// there is nothing to do
	if (!m_isPlaying && !m_isOver)
	{
		return;
	}
	printf("Game is over!\n");

	m_isPlaying = false;
	m_isOver = true;

	PrintCustomer(m_player);

	UnloadLevel();
	LoadUi("assets/end_game.tmx", m_uiView);

	TileMap* ui = FindUi("assets/end_game.tmx");
	if (ui)
	{
		TextActor* timeText = ui->FindActorByName<TextActor>("TimeText");
		if (timeText)
		{
			int minutes = m_timeRemaining / 60;
			int seconds = m_timeRemaining - minutes * 60;
			char buffer[8];
			sprintf(buffer, "%02d:%02d", minutes, seconds);
			timeText->m_text.setString(buffer);
		}

		TextActor* moneyText = ui->FindActorByName<TextActor>("MoneyText");
		if (moneyText)
		{
			char buffer[16];
			sprintf(buffer, "$%d", m_player.GetMunny());
			moneyText->m_text.setString(buffer);
		}

		const GroceryList& gc = m_player.GetGroceryList();
		int total = gc.GetTotalItems();
		int checked = gc.GetNumberOfCheckedItems();

		TextActor* itemText = ui->FindActorByName<TextActor>("ItemsText");
		if (itemText)
		{
			
			char buffer[16];
			sprintf(buffer, "%d/%d", checked, total);
			itemText->m_text.setString(buffer);
		}

		float timeFactor = std::min(1.0f, m_timeRemaining / (float) m_targetTime);
		float moneyFactor = std::min(1.0f, m_player.GetMunny() / (float) m_targetMoney);
		float itemFactor = checked / (float) total;
		float factor = (timeFactor + moneyFactor + itemFactor) / 3.0f;
		int score = (int) (1000000 * factor);

		ScoreRank rank = RankE;
		if (score == 100000)
		{
			rank = RankSSS;
		}
		else if (score >= 999000)
		{
			rank = RankSSS;
		}
		else if (score >= 989000)
		{
			rank = RankSS;
		}
		else if (score >= 959000)
		{
			rank = RankS;
		}
		else if (score >= 899000)
		{
			rank = RankA;
		}
		else if (score >= 799000)
		{
			rank = RankB;
		}
		else if (score >= 699000)
		{
			rank = RankC;
		}
		else if (score >= 599000)
		{
			rank = RankD;
		}

		TextActor* rankText = ui->FindActorByName<TextActor>("RankText");
		if (rankText)
		{
			rankText->m_text.setString(s_ranks[rank]);
			rankText->m_text.setColor(s_rankColors[rank]);
		}

		TextActor* scoreText = ui->FindActorByName<TextActor>("ScoreText");
		if (scoreText)
		{
			char buffer[16];
			sprintf(buffer, "%d", score);
			scoreText->m_text.setString(buffer);
		}

		TextActor* flavorText = ui->FindActorByName<TextActor>("FlavorText");
		if (flavorText)
		{
			const char* txt = s_flavorTexts[rank][rand() % MAX_FLAVORS];
			flavorText->m_text.setString(txt);
		}
	}
}

void Game::OnKeyReleased(sf::Keyboard::Key key)
{
	if (IsStarted())
	{
		if (key == sf::Keyboard::R)
		{
			EndGame();
			NewGame();
			SetLevel("assets/test_shop.tmx", m_uiView);
		}
		if (key == sf::Keyboard::E)
		{
			EndGame();
		}
		if (key == sf::Keyboard::Escape)
		{
			UnloadLevel();
			UnloadUi("assets/end_game.tmx");
			LoadUi("assets/main_menu.tmx", m_uiView);
			m_isPlaying = false;
			m_isOver = false;
		}
		if (key == sf::Keyboard::Return)
		{
			if (IsOver())
			{
				UnloadUi("assets/end_game.tmx");
				LoadUi("assets/main_menu.tmx", m_uiView);
				UnloadLevel();
				m_isOver = false;
			}
		}
		if (key == sf::Keyboard::Space)
		{
			const TileMap map = GetCurrentMap();
			Player* pPlayer = map.GetPlayer();
			if (pPlayer != nullptr)
			{
				sf::Vector2i gridPos = pPlayer->GetGridNode()->grid_position;
				std::vector<GridEntity*> vGridEnts = map.GetGridEntitiesAtTilePos(pPlayer->GetGridNode()->grid_position.x, pPlayer->GetGridNode()->grid_position.y);
				for (size_t i = 0; i < vGridEnts.size(); ++i)
				{
					if (vGridEnts[i] == nullptr || vGridEnts[i] == pPlayer)
						continue;

					if (vGridEnts[i]->GetType() == "ItemActor")
					{
						ItemActor* pItemActor = static_cast<ItemActor*>(vGridEnts[i]);
						int id = pItemActor->GetId();
						ItemData* itemData = m_stores["assets/test_shop.tmx"].FindItemDataById(id);
						if (itemData)
						{
							if (m_player.CanAddItem(*itemData->m_item))
							{
								m_player.AddItem(*itemData->m_item);
								m_player.PrintGroceryList();
								m_player.PrintInventory();
							}
							else
							{
								//TODO (daniel) put bad beep here + message
							}
						}
						else
						{
							printf("Missing item id = %d\n", id);
						}
					}
				}
			}
		}
	}
	else
	{
		if (key == sf::Keyboard::Return)
		{
			NewGame();
			SetLevel("assets/test_shop.tmx", m_gameView);
		}
	}
}

void Game::DebugPrintItemActor(ItemActor* itemActor)
{
	int id = itemActor->GetId();
	ItemData* itemData = m_stores["assets/test_shop.tmx"].FindItemDataById(id);
	if (itemData)
	{
		Item& item = *itemData->m_item;
		DebugPrintf("%s : %d", item.GetItemName().c_str(), item.GetCost());
	}
}

Item* Game::FindItemByName(const std::string& name)
{
	Tokenizer tokens(name, "_");

	if (tokens.size() != 2)
	{
		printf("Invalid item: %s", name.c_str());
		return nullptr;
	}

	const std::string& prefix = tokens[0];
	const std::string& suffix = tokens[1];

	Item::EType itemType = Item::EType::ET_INVALID;
	Item::EAdjective itemAdj = Item::EAdjective::EA_INVALID;

	auto itrSuffix = m_stringToItemType.find(suffix);
	if (itrSuffix != m_stringToItemType.end())
	{
		itemType = itrSuffix->second;
	}

	auto itrPrefix = m_stringToItemAdjective.find(prefix);
	if (itrPrefix != m_stringToItemAdjective.end())
	{
		itemAdj = itrPrefix->second;
	}

	if (itemType == Item::EType::ET_INVALID || itemAdj == Item::EAdjective::EA_INVALID)
	{
		printf("Unknown item: %s", name.c_str());
		return nullptr;
	}

	return m_itemDatabase[itemType][itemAdj];
}