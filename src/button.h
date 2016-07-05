#pragma once

#include <assert.h>
#include <functional>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "event.h"
#include "actor.h"

class Button;

Button* CreateButton(Actor *actor = nullptr, sf::View *view = nullptr, const std::string& event = "");

// This makes items clickable
// It is not responsible for any animations
// A button fires an event when clicked
class Button
{
public:
	Button();
	Button(Actor *actor, sf::View *view, const std::string& event);
	~Button();

	// Right now the only thing that determines whether we can fire
	// is if this button has an event to fire
	bool CanFire() const;

	// Resets the event if one was set
	void Reset();

	void SetEvent(const std::string& event);
	void SetActor(Actor *Actor);
	void SetView(sf::View *view);
	// Fires an event/callback if a mouse button is detected
	bool CheckMousePress(const sf::Event::MouseButtonEvent& mbe, sf::RenderWindow& window) const;

private:
	// sprite to use as the hitbox
	// for mouse clicks/taps
	Actor *m_actor;
	// keep track of view for buttons
	// so we know how to translate the mouse click
	sf::View *m_view;
	std::string m_event;
};