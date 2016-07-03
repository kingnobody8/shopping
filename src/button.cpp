#include "button.h"

Button* CreateButton()
{
	Button* button = new Button();
	RegisterButton(button);
	return button;
}

Button::Button()
    : m_spriteActor(nullptr),
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

void Button::SetSpriteActor(SpriteActor *spriteActor)
{
	this->m_spriteActor = spriteActor;
}

void Button::CheckMousePress(const sf::Vector2f mouse) const
{
	// button should have a sprite when checking for collisisions
	assert(this->m_spriteActor != nullptr);
    if(!this->CanFire() || !this->m_spriteActor->GetRect().contains(mouse.x, mouse.y))
    {
        return;
    }

    if(this->m_event != "")
    {
        FireEvent(this->m_event, nullptr);
        printf("Fired event: %s\n", this->m_event.c_str());
    }
}