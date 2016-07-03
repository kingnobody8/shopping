#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "debug_text.h"
#include "box_actor.h"
#include "player.h"
#include "text_actor.h"
#include <vector>
#include "customer.h"
#include "Tmx.h.in"
#include "tilemap.h"
//#include <vld.h>

sf::Font* g_defaultFont;
sf::Text g_debugText;
sf::RectangleShape g_debugTextBackground;
std::vector<Actor*> g_actors;

void RegisterActor(Actor* actor)
{
	g_actors.push_back(actor);
}

void DestroyActor(Actor* actor)
{
	g_actors.erase(std::find(g_actors.begin(), g_actors.end(), actor));
	delete actor;
}

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
	Character::InitializeCharacterFrameMap();

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

	sf::Music music;
	if (music.openFromFile("assets/sounds/110-pokemon-center.wav"))
	{
		music.setLoop(true);
		//music.play();
	}

	sf::RenderWindow window(sf::VideoMode(800, 600), "Shopping Game", sf::Style::Default);
	window.setActive();

	TileMap tMap;
	tMap.Init("assets/test_shop.tmx", g_actors);

	g_defaultFont = new sf::Font;

	g_defaultFont->loadFromFile("assets/fonts/m5x7.ttf");
	g_debugText.setFont(*g_defaultFont);
	g_debugTextBackground.setFillColor(sf::Color::Black);

	Player* man = tMap.GetPlayer();// CreateActor<Player>();

	/*for (int i = 0; i < g_actors.size(); ++i)
	{
		if (g_actors[i]->GetType() == )
		{
			man = g_actors[i];
			break;
		}
	}*/

	// Create the camera, origin at center
	const float w = 352.0f;	// '11' cells
	const float h = 256.0f; // '8' cells
	sf::View view(sf::FloatRect(-w / 2.0f, -h / 2.0f, w, h));
	sf::IntRect camMoveRect;
	camMoveRect.left = view.getCenter().x - view.getSize().x / 3.0f;
	camMoveRect.top = view.getCenter().y - view.getSize().y / 3.0f;
	camMoveRect.width = view.getSize().x / 3.0f;
	camMoveRect.height = view.getSize().y / 3.0f;
	//(view.getCenter() - view.getSize() / 3.0f, view.getCenter + view.)


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

			// Skin swap test
			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::S)
			{
				static short skin = 0;
				skin = (skin + 1) % 4;
				man->SetSkin(skin);
			}
		}

		// Update actors
		for (size_t i = 0; i < g_actors.size(); ++i)
		{
			g_actors[i]->Update(dt);
		}

		auto collides =	tMap.PerformCollisionTest(man->GetRect());
		if(!collides.empty())
		{
			for (size_t i = 0; i < collides.size(); ++i)
			{
				TileActor* pTileActor = static_cast<TileActor*>(collides[i]);
				sf::IntRect tileRect = pTileActor->GetRect();
				sf::IntRect manRect = man->GetRect();
				sf::IntRect intersect;
				tileRect.intersects(manRect, intersect);

				int multi = 1;
				if (intersect.width > intersect.height)
				{
					intersect.width = 0;
					if (manRect.top > tileRect.top)
					{
						multi = -1;
					}
				}
				else
				{
					intersect.height = 0;
					if (manRect.left > tileRect.left)
					{
						multi = -1;
					}
				}

				sf::Vector2f diff(intersect.width, intersect.height);
				diff.x *= multi;
				diff.y *= multi;

				man->SetPosition(man->GetPosition() - diff);
			}
		}


		sf::IntRect manRect = man->GetRect();
		bool bSet = false;
		if (manRect.left < camMoveRect.left)
		{
			camMoveRect.left = manRect.left;
			bSet = true;
		}
		if (manRect.top < camMoveRect.top)
		{
			camMoveRect.top = manRect.top;
			bSet = true;
		}
		if (manRect.left + manRect.width > camMoveRect.left + camMoveRect.width)
		{
			camMoveRect.left = manRect.left + manRect.width - camMoveRect.width;
			bSet = true;
		}
		if (manRect.top + manRect.height > camMoveRect.top + camMoveRect.height)
		{
			camMoveRect.top = manRect.top + manRect.height - camMoveRect.height;
			bSet = true;
		}

		if (bSet)
		{
			// Round to nearest int to avoid artifacting with half pixels in tilemap
			float x = (int) (camMoveRect.left + camMoveRect.width / 2.0f);
			float y = (int) (camMoveRect.top + camMoveRect.height / 2.0f);
			
			view.setCenter(x, y);
		}


		// Clear
		window.clear(sf::Color(50, 75, 50));

		// Set active camera
		window.setView(view);

		// Draw actors
		for (size_t i = 0; i < g_actors.size(); ++i)
		{
			g_actors[i]->Draw(window);
		}

		sf::Vertex verts[4];
		verts[1].position.x = 16;
		verts[0].color = verts[1].color = sf::Color(255, 0, 0);
		verts[3].position.y = 16;
		verts[2].color = verts[3].color = sf::Color(0, 255, 0);
		window.draw(verts, 4, sf::PrimitiveType::Lines);

		sf::VertexArray camRectVerts;
		camRectVerts.resize(5);
		camRectVerts[0].position.x = camMoveRect.left;
		camRectVerts[0].position.y = camMoveRect.top;
		camRectVerts[1].position.x = camMoveRect.left + camMoveRect.width;
		camRectVerts[1].position.y = camMoveRect.top;
		camRectVerts[2].position.x = camMoveRect.left + camMoveRect.width;
		camRectVerts[2].position.y = camMoveRect.top + camMoveRect.height;
		camRectVerts[3].position.x = camMoveRect.left;
		camRectVerts[3].position.y = camMoveRect.top + camMoveRect.height;
		camRectVerts[4].position.x = camMoveRect.left;
		camRectVerts[4].position.y = camMoveRect.top;

		for (size_t i = 0; i < camRectVerts.getVertexCount(); ++i)
		{
			camRectVerts[i].color = sf::Color::Blue;
		}

		window.draw(&camRectVerts[0], camRectVerts.getVertexCount(), sf::PrimitiveType::LinesStrip);

		g_debugText.setColor(sf::Color::Black);
		DebugPrintf("Munny: %d", customer.GetMunny());

		// Debug text
		window.setView(window.getDefaultView());
		window.draw(g_debugTextBackground);
		window.draw(g_debugText);

		// Display window
		window.display();
	}

	delete g_defaultFont;

	return 0;
}