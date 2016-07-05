#include "button.h"

Button* CreateButton(Actor *actor, sf::View *view, const std::string& event)
{
	return new Button(actor, view, event);
}

Button::Button()
    : m_actor(nullptr),
    m_view(nullptr),
    m_event("")
{
    this->Reset();
}

Button::Button(Actor *actor, sf::View *view, const std::string& event)
	: m_actor(actor),
	m_view(view),
	m_event(event)
{
}

Button::~Button()
{
	this->Reset();
}

bool Button::CanFire() const
{
    return this->m_event != "";
}

void Button::Reset()
{
	this->m_event = "";
}

void Button::SetEvent(const std::string& event)
{
    this->m_event = event;
}

void Button::SetActor(Actor *actor)
{
	this->m_actor = actor;
}

void Button::SetView(sf::View *view)
{
    this->m_view = view;
}

bool Button::CheckMousePress(const sf::Event::MouseButtonEvent& mbe, sf::RenderWindow& window) const
{
	sf::Vector2f& vec = window.mapPixelToCoords(sf::Vector2i(mbe.x, mbe.y), *this->m_view);
	// button should have an actor when checking for collisisions
	assert(this->m_actor != nullptr);
    if(!this->CanFire() || !this->m_actor->GetRect().contains(vec.x, vec.y))
    {
        return false;
    }

	printf("Fired event: %s\n", this->m_event.c_str());
	FireEvent(this->m_event, nullptr);
	return true;
}