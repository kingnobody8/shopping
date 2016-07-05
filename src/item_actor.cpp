#include "item_actor.h"
#include "gfx_util.h"
#include <assert.h>
#include <sstream>
#include <iterator>  
#include <iostream> 

ItemActor::ItemActor()
	: m_eAdj(Item::EAdjective::EA_INVALID)
	, m_eType(Item::EType::ET_INVALID)
	, m_id(-1)
{
	m_szType = "ItemActor";
}

void ItemActor::Update(float dt)
{
}

void ItemActor::Draw(sf::RenderWindow& window)
{
	TileActor::Draw(window);
	sf::IntRect rect = GetRect();

	sf::VertexArray verts;
	verts.resize(5);
	verts[0].position.x = rect.left;
	verts[0].position.y = rect.top;
	verts[1].position.x = rect.left + rect.width;
	verts[1].position.y = rect.top;
	verts[2].position.x = rect.left + rect.width;
	verts[2].position.y = rect.top + rect.height;
	verts[3].position.x = rect.left;
	verts[3].position.y = rect.top + rect.height;
	verts[4].position.x = rect.left;
	verts[4].position.y = rect.top;

	for (size_t i = 0; i < verts.getVertexCount(); ++i)
	{
		sf::Color clr = sf::Color::Magenta;
		switch (m_eAdj)
		{
		case Item::EAdjective::EA_BLUE: clr = sf::Color::Blue; break;
		case Item::EAdjective::EA_GREEN: clr = sf::Color::Green; break;
		case Item::EAdjective::EA_RED: clr = sf::Color::Red; break;
		case Item::EAdjective::EA_WHITE: clr = sf::Color::White; break;
		}
		verts[i].color = clr;
	}

	window.draw(&verts[0], verts.getVertexCount(), sf::PrimitiveType::LinesStrip);
}

void ItemActor::Init(const std::string& szItemType, int id)
{
	size_t index = szItemType.find_first_of("_");
	std::string szAdj = std::string(szItemType.begin(), szItemType.begin() + index);
	std::string szType = std::string(szItemType.begin() + index + 1, szItemType.end());

	m_eAdj = Item::GetAdjFromString(szAdj);
	m_eType = Item::GetTypeFromString(szType);
	assert(m_eAdj != Item::EAdjective::EA_INVALID && m_eType != Item::EType::ET_INVALID);

	m_id = id;
}

void ItemActor::PurchaseItem(Customer* pCustomer)
{
	/*if (pCustomer->CanAddItem(*m_pItem))
	{
		pCustomer->AddItem(*m_pItem);
		pCustomer->PrintGroceryList();
		pCustomer->PrintInventory();
	}*/

	//pCustomer->CanAddItem()
}