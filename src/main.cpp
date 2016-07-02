#include <SFML\Graphics.hpp>
#include "box_actor.h"
#include "sprite_actor.h"
#include <vector>
#include "customer.h"
#include "Tmx.h.in"
#include "tilemap_actor.h"

void PrintCustomer(Customer* c)
{
	printf("Customer Wallet: %d\n", c->GetMunny());
	printf("List:\n");
	c->PrintGroceryList();
	printf("Inventory:\n");
	c->PrintInventory();
}

void AddItemAttempt(Customer* c, Item* i)
{
	printf("Adding item: %d %s\n", i->GetCost(), i->GetItemName().c_str());
	printf("Customer wallet: %d\n", c->GetMunny());
	bool canAdd = c->CanAddItem(*i);
	printf("CanAddItem: %d\n", canAdd);

	if (canAdd)
	{
		c->AddItem(*i);
		PrintCustomer(c);
	}
	printf("\n\n");
}

int main(int argc, char** argv)
{
	Tmx::Map *map = new Tmx::Map();
	std::string fileName = (argc > 1) ? argv[1] : "assets/test_shop.tmx";
	map->ParseFile(fileName);

	if (map->HasError())
	{
		printf("error code: %d\n", map->GetErrorCode());
		printf("error text: %s\n", map->GetErrorText().c_str());

		system("pause");

		return map->GetErrorCode();
	}

	Item blue_milk(Item::EAdjective::EA_BLUE, Item::EType::ET_MILK, 500);
	Item green_eggs(Item::EAdjective::EA_GREEN, Item::EType::ET_EGGS, 750);
	Item white_meat(Item::EAdjective::EA_WHITE, Item::EType::ET_MEAT, 1000);
	Item red_candy(Item::EAdjective::EA_RED, Item::EType::ET_CANDY, 250);
	Item blue_eggs(Item::EAdjective::EA_BLUE, Item::EType::ET_EGGS, 300);

	GroceryList gc;
	gc.AddItem(blue_milk);
	gc.AddItem(green_eggs);
	gc.AddItem(white_meat);
	gc.AddItem(red_candy);

	Customer customer(gc, 1000);


	sf::RenderWindow window(sf::VideoMode(800, 600), "Shopping Game", sf::Style::Default);
	window.setActive();

	std::vector<Actor*> m_actors;

	TileMapActor* tileMap = new TileMapActor(map);
	tileMap->LoadAssets(m_actors);

	m_actors.push_back(tileMap);

	SpriteActor* man = new SpriteActor();
	m_actors.push_back(man);

	// Create the camera, origin at center
	const float w = 352.0f;	// '11' cells
	const float h = 256.0f; // '8' cells
	sf::View view(sf::FloatRect(-w / 2.0f, -h / 2.0f, w, h));

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

			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Num1)
			{
				AddItemAttempt(&customer, &blue_milk);
			}
			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Num2)
			{
				AddItemAttempt(&customer, &green_eggs);
			}
			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Num3)
			{
				AddItemAttempt(&customer, &white_meat);
			}
			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Num4)
			{
				AddItemAttempt(&customer, &red_candy);
			}
			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Num5)
			{
				AddItemAttempt(&customer, &blue_eggs);
			}
			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::R)
			{
				gc = GroceryList();
				gc.AddItem(blue_milk);
				gc.AddItem(green_eggs);
				gc.AddItem(white_meat);
				gc.AddItem(red_candy);

				customer =  Customer(gc, 1000);

				PrintCustomer(&customer);
			}
		}

		// Update actors
		for (size_t i = 0; i < m_actors.size(); ++i)
		{
			m_actors[i]->Update(dt);
		}

		view.setCenter(man->GetPosition());

		// Clear
		window.clear(sf::Color(50, 75, 50));

		// Set active camera
		window.setView(view);

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