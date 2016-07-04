#include "grid_entity.h"
#include "tilemap.h"

GridEntity::GridEntity()
	: m_pGridNode(nullptr)
	, m_nLayerId(-1)
{
}

GridEntity::GridEntity(GridNode* pGridNode, const int& nLayerId)
	: m_pGridNode(pGridNode)
	, m_nLayerId(nLayerId)
{
}


GridEntity::~GridEntity()
{
}

void GridEntity::Draw(sf::RenderWindow& window)
{
	SpriteActor::Draw(window);

	sf::IntRect rect = GetRect();
	sf::Vector2f pos = GetPosition();
	pos.x += rect.width / 2;
	pos.y += rect.height / 2;

	sf::Vertex verts[4];
	verts[0].position = verts[1].position = pos;
	verts[1].position.x += 4;
	verts[0].color = verts[1].color = sf::Color(255, 0, 0);

	verts[2].position = verts[3].position = pos;
	verts[3].position.y += 4;
	verts[2].color = verts[3].color = sf::Color(0, 255, 0);
	window.draw(verts, 4, sf::PrimitiveType::Lines);
}

bool GridEntity::CanMove(const EDirection& eDir)
{
	assert(eDir > EDirection::ED_INVALID && eDir < EDirection::ED_COUNT);
	switch (eDir)
	{
	case ED_UP:
	{
		if (m_pGridNode->pUp != nullptr && m_pGridNode->pUp->pGridEntity == nullptr)
		{
			return true;
		}
		break;
	}
	case ED_DOWN:
	{
		if (m_pGridNode->pDown != nullptr && m_pGridNode->pDown->pGridEntity == nullptr)
		{
			return true;
		}
		break;
	}
	case ED_RIGHT:
	{
		if (m_pGridNode->pRight != nullptr && m_pGridNode->pRight->pGridEntity == nullptr)
		{
			return true;
		}
		break;
	}
	case ED_LEFT:
	{
		if (m_pGridNode->pLeft != nullptr && m_pGridNode->pLeft->pGridEntity == nullptr)
		{
			return true;
		}
		break;
	}
	}
	return false;
}

/*virtual*/ void GridEntity::Move(const EDirection& eDir)
{
	assert(eDir > EDirection::ED_INVALID && eDir < EDirection::ED_COUNT);
	assert(CanMove(eDir));

	GridNode* pNextNode = nullptr;

	switch (eDir)
	{
	case ED_UP:		pNextNode = m_pGridNode->pUp; break;
	case ED_DOWN:	pNextNode = m_pGridNode->pDown; break;
	case ED_RIGHT:	pNextNode = m_pGridNode->pRight; break;
	case ED_LEFT:	pNextNode = m_pGridNode->pLeft; break;
	}

	m_pGridNode->pGridEntity = nullptr;
	pNextNode->pGridEntity = this;
	m_pGridNode = pNextNode;
}

void GridEntity::AlignPositionToNode()
{
	sf::IntRect rect = GetRect();

	SetPosition(sf::Vector2f(m_pGridNode->position.x - rect.width / 2, m_pGridNode->position.y - rect.height / 2));
}