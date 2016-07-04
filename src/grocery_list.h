#pragma once
#include "item.h"
#include <vector>

class GroceryList
{
public:
	enum ECheckState { ECS_INVALID, ECS_EMPTY, ECS_HIDDEN, ECS_RIGHT_TYPE_WRONG_ADJ, ECS_FILLED, ECS_COUNT, ECS_NOT_IN_LIST };
	static std::string GetCheckStateString(const ECheckState& eCheckState);

private:
	struct CheckSlot
	{
		Item m_Item;
		ECheckState m_eCheckState = ECS_INVALID;
		bool IsValid() { return m_eCheckState > ECS_INVALID && m_eCheckState < ECS_COUNT; }
	};


public:
	GroceryList();

	void AddItem(const Item& Item, const ECheckState& eCheckState = ECheckState::ECS_EMPTY);
	bool IsItemTypeOnList(const Item::EType& eType);
	ECheckState GetItemCheckType(const Item::EType& eType);
	void CheckoutItem(const Item& item);

	void PrintList() const;

	int GetTotalItems() const;
	int GetNumberOfCheckedItems() const;

private:
	CheckSlot* FindCheckSlotByType(const Item::EType& eType);

private:
	std::vector<CheckSlot> m_vCheckSlot;

};