#pragma once
#include "tile_actor.h"
#include "Tmx.h.in"
#include "customer.h"

class ItemActor : public TileActor
{
public:
	ItemActor();
	
	void Init(const std::string& szItemType, int id);

	virtual void Update(float dt);
	virtual void Draw(sf::RenderWindow& window);

	void PurchaseItem(Customer* pCustomer);

	int GetId() { return m_id; }

private:
	Item::EAdjective m_eAdj;
	Item::EType	m_eType;
	int m_id;
};