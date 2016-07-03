#pragma once
#include "actor.h"
#include "Tmx.h.in"
#include "customer.h"

class ItemActor : public Actor
{
public:
	ItemActor();
	
	void Init(Tmx::Object* pObject);

	virtual void Update(float dt);
	virtual void Draw(sf::RenderWindow& window);

	sf::IntRect GetRect() const;

	void PurchaseItem(Customer* pCustomer);

private:
	Tmx::Object* m_pObject;
	Item::EAdjective m_eAdj;
	Item::EType	m_eType;

	Item* m_pItem;
};