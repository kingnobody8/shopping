#include "item_actor.h"
#include "gfx_util.h"


ItemActor::ItemActor()
{
	
}


ItemActor::~ItemActor()
{}

void ItemActor::Update(float dt)
{
}

void ItemActor::Draw(sf::RenderWindow& window)
{
}

sf::IntRect ItemActor::GetRect() const
{
	sf::IntRect rect;
	rect.left = m_pObject->GetX();
	rect.top = m_pObject->GetY();
	rect.width = m_pObject->GetWidth();
	rect.height = m_pObject->GetHeight();
	return rect;
}