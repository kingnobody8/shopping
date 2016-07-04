#pragma once
#include "tile_actor.h"
#include "Tmx.h.in"
#include "customer.h"

class ItemActor : public TileActor
{
public:
	ItemActor();
	
	void Init(const std::string& szItemType, const int& cost);

	virtual void Update(float dt);
	virtual void Draw(sf::RenderWindow& window);

	void PurchaseItem(Customer* pCustomer);

	const Item& GetItem() { return *m_pItem; }

private:
	Item::EAdjective m_eAdj;
	Item::EType	m_eType;

	Item* m_pItem;
};