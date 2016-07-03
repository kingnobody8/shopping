#include "item_actor.h"
#include "gfx_util.h"
#include <assert.h>
#include <sstream>
#include <iterator>  
#include <iostream> 

ItemActor::ItemActor()
{
	m_szType = "ItemActor";
}


ItemActor::~ItemActor()
{}

void ItemActor::Update(float dt)
{
}

void ItemActor::Draw(sf::RenderWindow& window)
{
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
		verts[i].color = sf::Color::Green;
	}

	window.draw(&verts[0], verts.getVertexCount(), sf::PrimitiveType::LinesStrip);
}

void ItemActor::Init(Tmx::Object* pObject)
{
	m_pObject = pObject;
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

void ItemActor::PurchaseItem(Customer* pCustomer)
{
	const Tmx::PropertySet& propSet = m_pObject->GetProperties();
	assert(propSet.HasProperty("item_type"));

	std::string s = propSet.GetStringProperty("item_type");
	std::stringstream ss(s);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);
	std::copy(vstrings.begin(), vstrings.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
	assert(vstrings.size() == 2);

	std::string szAdj = vstrings[0];
	std::string szType = vstrings[1];

	Item::EAdjective eAdj = Item::GetAdjFromString(szAdj);
	Item::EType eType = Item::GetTypeFromString(szType);
	assert(eAdj != Item::EAdjective::EA_INVALID && eType != Item::EType::ET_INVALID);

	//pCustomer->CanAddItem()
}