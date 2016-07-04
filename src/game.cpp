#include "game.h"

static void SetLevel(const std::string& path)
{
	UnloadUi("assets/main_menu.tmx");
	LoadLevel(path);
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

Game::Game()
	: m_isPlaying(false)
	, blue_milk(Item::EAdjective::EA_BLUE, Item::EType::ET_MILK, 500)
	, green_eggs(Item::EAdjective::EA_GREEN, Item::EType::ET_EGGS, 750)
	, white_meat(Item::EAdjective::EA_WHITE, Item::EType::ET_MEAT, 1000)
	, red_candy(Item::EAdjective::EA_RED, Item::EType::ET_CANDY, 250)
	, blue_eggs(Item::EAdjective::EA_BLUE, Item::EType::ET_EGGS, 300)
{}

void Game::NewGame()
{
	printf("Starting a new game!\n");

	m_isPlaying = true;
	m_isOver = false;

	m_timeRemaining = 9999;

	gc = GroceryList();
	gc.AddItem(blue_milk);
	gc.AddItem(green_eggs);
	gc.AddItem(white_meat);
	gc.AddItem(red_candy);

	m_player = Customer(gc, 1000);
}

void Game::EndGame()
{
	printf("Game is over!\n");

	m_isPlaying = false;
	m_isOver = false;

	PrintCustomer(m_player);
}

void Game::OnKeyReleased(sf::Keyboard::Key key)
{
	if (IsStarted())
	{
		if (key == sf::Keyboard::Num1)
		{
			AddItemAttempt(m_player, blue_milk);
		}
		if (key == sf::Keyboard::Num2)
		{
			AddItemAttempt(m_player, green_eggs);
		}
		if (key == sf::Keyboard::Num3)
		{
			AddItemAttempt(m_player, white_meat);
		}
		if (key == sf::Keyboard::Num4)
		{
			AddItemAttempt(m_player, red_candy);
		}
		if (key == sf::Keyboard::Num5)
		{
			AddItemAttempt(m_player, blue_eggs);
		}
		if (key == sf::Keyboard::R)
		{
			EndGame();
			NewGame();
			SetLevel("assets/test_shop.tmx");
		}
		if (key == sf::Keyboard::Escape)
		{
			UnloadLevel();
			LoadUi("assets/main_menu.tmx");
			EndGame();
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
						if (m_player.CanAddItem(pItemActor->GetItem()))
						{
							m_player.AddItem(pItemActor->GetItem());
							m_player.PrintGroceryList();
							m_player.PrintInventory();
						}
						else
						{
							//TODO (daniel) put bad beep here + message
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
			SetLevel("assets/test_shop.tmx");
		}
	}
}