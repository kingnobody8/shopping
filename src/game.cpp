#include "game.h"

static void SetLevel(const std::string& path, sf::View* view)
{
	UnloadUi("assets/main_menu.tmx");
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

Game::Game()
	: m_isPlaying(false)
	, blue_milk(Item::EAdjective::EA_BLUE, Item::EType::ET_MILK, 500)
	, green_eggs(Item::EAdjective::EA_GREEN, Item::EType::ET_EGGS, 750)
	, white_meat(Item::EAdjective::EA_WHITE, Item::EType::ET_MEAT, 1000)
	, red_candy(Item::EAdjective::EA_RED, Item::EType::ET_CANDY, 250)
	, blue_eggs(Item::EAdjective::EA_BLUE, Item::EType::ET_EGGS, 300)
	, m_uiView(nullptr)
	, m_gameView(nullptr)
{}

Game::Game(sf::View& uiView, sf::View& gameView)
	: m_isPlaying(false)
	, blue_milk(Item::EAdjective::EA_BLUE, Item::EType::ET_MILK, 500)
	, green_eggs(Item::EAdjective::EA_GREEN, Item::EType::ET_EGGS, 750)
	, white_meat(Item::EAdjective::EA_WHITE, Item::EType::ET_MEAT, 1000)
	, red_candy(Item::EAdjective::EA_RED, Item::EType::ET_CANDY, 250)
	, blue_eggs(Item::EAdjective::EA_BLUE, Item::EType::ET_EGGS, 300)
	, m_uiView(&uiView)
	, m_gameView(&gameView)
{}

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
}

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
	// if we're not playing and the game is not over
	// there is nothing to do
	if (!m_isPlaying && !m_isOver)
	{
		return;
	}
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
			SetLevel("assets/test_shop.tmx", m_uiView);
		}
		if (key == sf::Keyboard::Escape)
		{
			UnloadLevel();
			LoadUi("assets/main_menu.tmx", m_uiView);
			EndGame();
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