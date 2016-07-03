#pragma once

#include <SFML/Graphics.hpp>

extern sf::Text g_debugText;
extern sf::RectangleShape g_debugTextBackground;

void DebugPrintf(const char* fmt, ...);