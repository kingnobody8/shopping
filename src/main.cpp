#include <SFML/Graphics.hpp>
//#include <SFML/Audio.hpp>

#include "event.h"
#include "debug_text.h"
#include "box_actor.h"
#include "player.h"
#include "button.h"
#include "text_actor.h"
#include <vector>
#include "customer.h"
#include "Tmx.h.in"
#include "tilemap.h"
#include "item_manager.h"
#include "item_actor.h"
#include "game.h"
//#include <vld.h>

sf::Font* g_defaultFont;
sf::Text g_debugText;
sf::RectangleShape g_debugTextBackground;

ItemManager itemManager;

TileMap g_dummyMap;
TileMap* g_currentLevelMap = nullptr;

const TileMap& GetCurrentMap()
{
	return *g_currentLevelMap;
}
std::map<std::string, TileMap*> g_uiMaps;

bool LoadLevel(const std::string& path, sf::View* view)
{
	TileMap* map = new TileMap;
	if (!map->Init(path, &itemManager, view))
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

bool LoadUi(const std::string& path, sf::View* view)
{
	TileMap* map = new TileMap;
	if (!map->Init(path, nullptr, view))
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

TileMap* FindUi(const std::string& path)
{
	auto itr = g_uiMaps.find(path);
	if (itr != g_uiMaps.end())
	{
		return itr->second;;
	}
	return nullptr;
}

int main(int argc, char** argv)
{
	g_defaultFont = new sf::Font;

	g_defaultFont->loadFromFile("assets/fonts/m5x7.ttf");
	g_debugText.setFont(*g_defaultFont);
	g_debugTextBackground.setFillColor(sf::Color::Black);

	g_currentLevelMap = &g_dummyMap;

	Character::InitializeCharacterFrameMap();

	//sf::Music music;
	//if (music.openFromFile("assets/sounds/110-pokemon-center.wav"))
	//{
	//	music.setLoop(true);
	//	//music.play();
	//}

	sf::RenderWindow window(sf::VideoMode(800, 600), "Shopping Game", sf::Style::Default);
	window.setActive();

	sf::View uiView = window.getDefaultView();
	// Create the camera, origin at center
	const float w = 176; // '11' cells
	const float h = 128; // '8' cells
	sf::View view(sf::FloatRect(-w / 2.0f, -h / 2.0f, w, h));

	LoadUi("assets/main_menu.tmx", &uiView);
	Game game(uiView, view);
	game.Init();

	Player* man = g_currentLevelMap->GetPlayer();

	sf::IntRect camMoveRect;
	camMoveRect.left = view.getCenter().x - view.getSize().x / 3.0f;
	camMoveRect.top = view.getCenter().y - view.getSize().y / 3.0f;
	camMoveRect.width = view.getSize().x / 3.0f;
	camMoveRect.height = view.getSize().y / 3.0f;

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
				if (!game.IsStarted())
				{
					window.close();
				}
			}

			if (event.type == sf::Event::KeyReleased)
			{
				game.OnKeyReleased(event.key.code);
			}

			if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Button::Left)
			{
				// for now only allow one button to be triggered per frame
				bool buttonTriggered = false;
				const sf::Event::MouseButtonEvent &mbe = event.mouseButton;
				// check all the buttons on all the tilemaps to see if any buttons were pressed
				for (auto itr = g_uiMaps.begin(); itr != g_uiMaps.end(); ++itr)
				{
					auto buttons = itr->second->GetButtons();
					for (auto bitr = buttons.begin(); bitr != buttons.end(); ++bitr)
					{
						if ((*bitr)->CheckMousePress(mbe, window))
						{
							buttonTriggered = true;
							goto button_check_break;
						}
					}
				}
				button_check_break:
				if (!buttonTriggered)
				{
					auto buttons = g_currentLevelMap->GetButtons();
					for (auto itr = buttons.begin(); itr != buttons.end(); ++itr)
					{
						if ((*itr)->CheckMousePress(mbe, window))
						{
							buttonTriggered = true;
							break;
						}
					}
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
		game.Update(dt);

		Player* player = g_currentLevelMap->GetPlayer();
		if (player)
		{
			sf::IntRect manRect = player->GetRect();
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
		window.setView(window.getDefaultView());
		game.Draw(window);

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

		if (player != nullptr)
		{
			bool hit = false;
			std::vector<GridEntity*> vGridEnts = g_currentLevelMap->GetGridEntitiesAtTilePos(player->GetGridNode()->grid_position.x, player->GetGridNode()->grid_position.y);
			for (size_t i = 0; i < vGridEnts.size(); ++i)
			{
				if (vGridEnts[i] == nullptr || vGridEnts[i] == player)
					continue;

				if (vGridEnts[i]->GetType() == "ItemActor")
				{
					hit = true;
					ItemActor* pItemActor = static_cast<ItemActor*>(vGridEnts[i]);
					game.DebugPrintItemActor(pItemActor);
				}
			}
			if (!hit)
			{
				DebugPrintf("");
			}
		}

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