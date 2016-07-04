#pragma once
#include "sprite_actor.h"

//forward declare
struct GridNode;

class GridEntity : public SpriteActor
{
public:
	enum EDirection 
	{
		ED_INVALID = -1,
		ED_NORTH,
		ED_SOUTH,
		ED_EAST,
		ED_WEST,
		ED_COUNT,
		ED_UP = ED_NORTH,
		ED_DOWN,
		ED_RIGHT,
		ED_LEFT
	};

public:
	GridEntity();
	GridEntity(GridNode* pGridNode, const int& nLayerId);
	virtual ~GridEntity();

	virtual void Draw(sf::RenderWindow& window);

	inline GridNode* GetGridNode() const { return m_pGridNode; }
	inline const int& GetLayerId() const { return m_nLayerId; }

	bool CanMove(const EDirection& eDir);
	virtual void Move(const EDirection& eDir);

	void SetGridNode(GridNode* pGridNode, const int& nLayerId) { m_pGridNode = pGridNode; m_nLayerId = nLayerId; }

	void AlignPositionToNode();

protected:
	GridNode* m_pGridNode;
	int m_nLayerId;

};