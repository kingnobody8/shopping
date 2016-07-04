#include <SFML/Graphics.hpp>
//#include <SFML/Ausdio.hpp>

#include "event.h"
#include "debug_text.h"
#include "box_actor.h"
#include "player.h"
#include "button.h"
#include "text_actor.h"
#include <vector>
//#include <functional>
#include "customer.h"
#include "Tmx.h.in"
#include "tilemap.h"
#include "item_manager.h"
#include "item_actor.h"
//#include <vld.h>

sf::Font* g_defaultFont;
sf::Text g_debugText;
sf::RectangleShape g_debugTextBackground;

ItemManager itemManager;

Customer* g_customer;
bool isActionReleased = false;
ItemActor* pItemActor = nullptr;

TileMap g_dummyMap;
TileMap* g_currentLevelMap = nullptr;

const TileMap& GetCurrentMap()
{
	return *g_currentLevelMap;
}
std::map<std::string, TileMap*> g_uiMaps;

bool LoadLevel(const std::string& path)
{
	TileMap* map = new TileMap;
	if (!map->Init(path, &itemManager))
	{
		delete map;
		return false;
	}

	if (g_currentLevelMap != &g_dummyMap)
	{
		g_currentLevelMap->Exit();
		delete g_currentLevelMap;
	}

	g_currentLevelMap = map;
	return true;
}

void UnloadLevel()
{
	if (g_currentLevelMap != &g_dummyMap)
	{
		g_currentLevelMap->Exit();
		delete g_currentLevelMap;
	}
	g_currentLevelMap = &g_dummyMap;
}

bool LoadUi(const std::string& path)
{
	TileMap* map = new TileMap;
	if (!map->Init(path, nullptr))
	{
		delete map;
		return false;
	}
	g_uiMaps[path] = map;
	return true;
}

void UnloadUi(const std::string& path)
{
	auto itr = g_uiMaps.find(path);
	if (itr != g_uiMaps.end())
	{
		itr->second->Exit();
		delete itr->second;
		g_uiMaps.erase(itr);
	}
}

Player* SetLevel(const std::string& path)
{
	UnloadUi("assets/main_menu.tmx");
	LoadLevel(path);
	printf("Entering %s...\n", path.c_str());
	return g_currentLevelMap->GetPlayer();
}

// We keep a separate list of buttons
// because buttons don't necessarily need to be drawn
// or updated. They just need to know what and when to fire
std::vector<Button*> g_buttons;

void RegisterButton(Button* button)
{
	g_buttons.push_back(button);
}

void DestroyButton(Button* button)
{
	g_buttons.erase(std::find(g_buttons.begin(), g_buttons.end(), button));
	delete button;
}

void PrintCustomer(Customer* c)
{
	printf("Customer Wallet: %d\n", c->GetMunny());
	c->PrintGroceryList();
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

void PurchaseItemTest(void* clientData)
{
	EVENT_AS(ItemActor, itemActor);

	itemActor.PurchaseItem(g_customer);
	printf("Got it\n");
}

int main(int argc, char** argv)
{
	g_defaultFont = new sf::Font;

	g_defaultFont->loadFromFile("assets/fonts/m5x7.ttf");
	g_debugText.setFont(*g_defaultFont);
	g_debugTextBackground.setFillColor(sf::Color::Black);

	g_currentLevelMap = &g_dummyMap;

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

	g_customer = new Customer(gc, 1000);

	RegisterEvent("PurchaseItem", PurchaseItemTest);

	//sf::Music music;
	//if (music.openFromFile("assets/sounds/110-pokemon-center.wav"))
	//{
	//	music.setLoop(true);
	//	//music.play();
	//}

	sf::RenderWindow window(sf::VideoMode(800, 600), "Shopping Game", sf::Style::Default);
	window.setActive();

	LoadUi("assets/main_menu.tmx");
	std::string nextLevel = "assets/test_shop.tmx";

	Player* man = g_currentLevelMap->GetPlayer();

	if (man)
	{
		Button *manButton = CreateButton();
		manButton->SetSpriteActor(man);
		manButton->SetEvent("GOO GOO", ([](void *x) {printf("GOO GOO EVENT BODY\n"); }));
	}

	// Create the camera, origin at center
	const float w = 176; // '11' cells
	const float h = 128; // '8' cells
	sf::View view(sf::FloatRect(-w / 2.0f, -h / 2.0f, w, h));
	sf::IntRect camMoveRect;
	camMoveRect.left = view.getCenter().x - view.getSize().x / 3.0f;
	camMoveRect.top = view.getCenter().y - view.getSize().y / 3.0f;
	camMoveRect.width = view.getSize().x / 3.0f;
	camMoveRect.height = view.getSize().y / 3.0f;

	sf::View uiView = window.getDefaultView();

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
				if (g_currentLevelMap != &g_dummyMap)
				{
					UnloadLevel();
					LoadUi("assets/main_menu.tmx");
					man = nullptr;
				}
				else
				{
					window.close();
				}
			}

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F1)
			{
				nextLevel = "assets/test_shop.tmx";
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F2)
			{
				nextLevel = "assets/test_shop2.tmx";
			}

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
			{
				man = SetLevel(nextLevel);
			}

			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space)
			{
				isActionReleased = true;
				if (pItemActor)
				{
					FireEvent("PurchaseItem", pItemActor);
				}
			}
			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Num1)
			{
				AddItemAttempt(g_customer, &blue_milk);
			}
			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Num2)
			{
				AddItemAttempt(g_customer, &green_eggs);
			}
			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Num3)
			{
				AddItemAttempt(g_customer, &white_meat);
			}
			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Num4)
			{
				AddItemAttempt(g_customer, &red_candy);
			}
			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Num5)
			{
				AddItemAttempt(g_customer, &blue_eggs);
			}
			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::R)
			{
				gc = GroceryList();
				gc.AddItem(blue_milk);
				gc.AddItem(green_eggs);
				gc.AddItem(white_meat);
				gc.AddItem(red_candy);

				delete g_customer;
				g_customer = new Customer(gc, 1000);

				PrintCustomer(g_customer);
			}

			// Skin swap test
			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::S)
			{
				static short skin = 0;
				skin = (skin + 1) % 4;
				if (man)
				{
					man->SetSkin(skin);
				}
			}
			else if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Button::Left)
			{
				const sf::Event::MouseButtonEvent &mbe = event.mouseButton;
				sf::Vector2f& vec = window.mapPixelToCoords(sf::Vector2i(mbe.x, mbe.y), view);
				for (size_t i = 0; i < g_buttons.size(); ++i)
				{
					g_buttons[i]->CheckMousePress(vec);
				}
			}

			if (event.type == sf::Event::MouseWheelScrolled)
			{
				float zoom = 1.0f - event.mouseWheelScroll.delta * 0.11f;
				view.zoom(zoom);
			}
		}

		// Update actors
		g_currentLevelMap->Update(dt);
		for (auto itr = g_uiMaps.begin(); itr != g_uiMaps.end(); ++itr)
		{
			itr->second->Update(dt);
		}

		if (man)
		{
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
				float x = (int)(camMoveRect.left + camMoveRect.width / 2.0f);
				float y = (int)(camMoveRect.top + camMoveRect.height / 2.0f);

				view.setCenter(x, y);
			}
		}


		// Clear
		window.clear(sf::Color(50, 75, 50));

		// Set active camera and  draw actors
		window.setView(view);
		g_currentLevelMap->Draw(window);

		for (auto itr = g_uiMaps.begin(); itr != g_uiMaps.end(); ++itr)
		{
			uiView.setCenter(itr->second->GetWidth() / 2, itr->second->GetHeight() / 2);
			window.setView(uiView);
			itr->second->Draw(window);
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

		DebugPrintf("Munny: %d", g_customer->GetMunny());
		//if(showItem)
		//	DebugPrintf("%s: %d", itemName.c_str(), cost);

		// Debug text
		window.setView(window.getDefaultView());
		window.draw(g_debugTextBackground);
		window.draw(g_debugText);

		// Display window
		window.display();
	}

	delete g_defaultFont;
	delete g_customer;

	return 0;
}