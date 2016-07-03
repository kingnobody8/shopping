#include "button.h"

Button* CreateButton()
{
	Button* button = new Button();
	RegisterButton(button);
	return button;
}

Button::Button()
    : m_hitbox(nullptr),
    m_event("")
{
    this->Reset();
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

void Button::SetEvent(const std::string& event, EventCallback callback)
{
    this->m_event = event;
	RegisterEvent(event, callback);
}

void Button::SetHitbox(sf::IntRect *hitbox)
{
	this->m_hitbox = hitbox;
}

void Button::CheckMousePress(const sf::Event::MouseButtonEvent& mbe) const
{
	// button should have a hitbox when checking for collisisions
	assert(this->m_hitbox != nullptr);
    if(!this->CanFire() || !this->m_hitbox->contains(mbe.x, mbe.y))
    {
        return;
    }

    if(this->m_event != "")
    {
        FireEvent(this->m_event, nullptr);
        printf("Fired event: %s\n", this->m_event.c_str());
    }
}