#pragma once

#include <assert.h>
#include <functional>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "event.h"

class Button;
void RegisterButton(Button *button);

Button* CreateButton();

// This makes items clickable
// It is not responsible for any animations
// A button fires an event when clicked
class Button
{
public:
	Button();
	virtual ~Button();

	// Right now the only thing that determines whether we can fire
	// is if this button has an event to fire
	bool CanFire() const;

	// Resets the event if one was set
	void Reset();

	void SetEvent(const std::string& event, EventCallback callback);
	void SetSpriteActor(SpriteActor *spriteActor);
	// Fires an event/callback if a mouse button is detected
	void CheckMousePress(const sf::Event::MouseButtonEvent& mbe) const;

private:
	// sprite to use as the hitbox
	// for mouse clicks/taps
	SpriteActor *m_spriteActor;
	std::string m_event;
};