#pragma once

#include <SFML/Graphics.hpp>

extern sf::Text g_debugText;

void DebugPrintf(const char* fmt, ...);