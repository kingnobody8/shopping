#pragma once
#include <string>

class Item
{
public:
	enum EAdjective { EA_INVALID = -1, EA_BLUE, EA_RED, EA_GREEN, EA_WHITE, EA_COUNT };
	enum EType { ET_INVALID = -1, ET_MILK, ET_EGGS, ET_MEAT, ET_SODA, ET_CANDY, ET_COUNT };
	static const std::string GetAdjString(const EAdjective& eAdj);
	static const std::string GetTypeString(const EType& eAdj);

public:
	Item();
	Item(const EAdjective& eAdj, const EType& eType, const int& nCost);

	bool IsValid() const;

	inline EAdjective GetAdj() const { return m_eAdj; }
	inline EType GetType() const { return m_eType; }
	inline int GetCost() const { return m_nCost; }

	const std::string GetItemName() const { return GetAdjString(m_eAdj) + std::string("_") + GetTypeString(m_eType); }

private:
	EAdjective m_eAdj;
	EType m_eType;
	int m_nCost;

};