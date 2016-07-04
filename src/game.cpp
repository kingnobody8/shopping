#include "game.h"
#include "text_actor.h"

static void SetLevel(const std::string& path)
{
	UnloadUi("assets/main_menu.tmx");
	UnloadUi("assets/end_game.tmx");
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

Game::Game()
	: m_isPlaying(false)
	, m_isOver(false)
	, m_timeRemaining(0)
	, m_totalTime(0)
	, m_targetTime(0)
	, m_startingMoney(0)
	, m_targetMoney(0)
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

	m_totalTime = 300;
	m_timeRemaining = m_totalTime;
	m_targetTime = 150;

	m_startingMoney = 1000;
	m_targetMoney = 500;
	
	gc = GroceryList();
	gc.AddItem(blue_milk);
	gc.AddItem(green_eggs);
	gc.AddItem(white_meat);
	gc.AddItem(red_candy);

	m_player = Customer(gc, m_startingMoney);
}

void Game::EndGame()
{
	printf("Game is over!\n");

	m_isPlaying = false;
	m_isOver = true;

	PrintCustomer(m_player);

	UnloadLevel();
	LoadUi("assets/end_game.tmx");

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

		TextActor* itemText = ui->FindActorByName<TextActor>("ItemsText");
		if (itemText)
		{
			int total = gc.GetTotalItems();
			int checked = gc.GetNumberOfCheckedItems();
			char buffer[16];
			sprintf(buffer, "%d/%d", checked, total);
			itemText->m_text.setString(buffer);
		}

		float timeFactor = std::min(1.0f, m_timeRemaining / (float) m_targetTime);
		float moneyFactor = std::min(1.0f, m_player.GetMunny() / (float) m_targetMoney);
		float itemFactor = gc.GetNumberOfCheckedItems() / (float) gc.GetTotalItems();
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
		if (key == sf::Keyboard::E)
		{
			EndGame();
		}
		if (key == sf::Keyboard::Escape)
		{
			UnloadLevel();
			UnloadUi("assets/end_game.tmx");
			LoadUi("assets/main_menu.tmx");
			m_isPlaying = false;
			m_isOver = false;
		}
		if (key == sf::Keyboard::Return)
		{
			if (IsOver())
			{
				UnloadUi("assets/end_game.tmx");
				LoadUi("assets/main_menu.tmx");
				UnloadLevel();
				m_isOver = false;
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