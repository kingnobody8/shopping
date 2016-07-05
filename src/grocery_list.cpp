#include "grocery_list.h"
#include <assert.h>

GroceryList::GroceryList()
{

}


void GroceryList::AddItem(const Item& Item, const ECheckState& eCheckState)
{
	assert(!IsItemTypeOnList(Item.GetType())); //we should only have one

	CheckSlot slot;
	slot.m_eCheckState = eCheckState;
	slot.m_Item = Item;
	m_vCheckSlot.push_back(slot);
}

bool GroceryList::IsItemTypeOnList(const Item::EType& eType)
{
	return GetItemCheckType(eType) != GroceryList::ECheckState::ECS_NOT_IN_LIST;
}

GroceryList::ECheckState GroceryList::GetItemCheckType(const Item::EType& eType)
{
	CheckSlot* pCheckSlot = FindCheckSlotByType(eType);
	if(pCheckSlot == nullptr)
		return GroceryList::ECheckState::ECS_NOT_IN_LIST;
	return pCheckSlot->m_eCheckState;
}

void GroceryList::CheckoutItem(const Item& item)
{
	CheckSlot* pCheckSlot = FindCheckSlotByType(item.GetType());
	assert(pCheckSlot);
	assert(pCheckSlot->IsValid());

	if (item.GetAdj() == pCheckSlot->m_Item.GetAdj())
	{
		pCheckSlot->m_eCheckState = ECheckState::ECS_FILLED;
	}
	else
	{
		pCheckSlot->m_eCheckState = ECheckState::ECS_RIGHT_TYPE_WRONG_ADJ;
	}
}


void GroceryList::PrintList() const
{
	for (size_t i = 0; i < m_vCheckSlot.size(); ++i)
	{
		printf("%d\t%s\t%s ($%d)\n", i, m_vCheckSlot[i].m_Item.GetItemName().c_str(), GetCheckStateString(m_vCheckSlot[i].m_eCheckState).c_str(), m_vCheckSlot[i].m_Item.GetCost());
	}
}

int GroceryList::GetTotalItems() const
{
	return (int) m_vCheckSlot.size();
}

int GroceryList::GetNumberOfCheckedItems() const
{
	int n = 0;
	for (size_t i = 0; i < m_vCheckSlot.size(); ++i)
	{
		if (m_vCheckSlot[i].m_eCheckState == GroceryList::ECheckState::ECS_FILLED || m_vCheckSlot[i].m_eCheckState == GroceryList::ECheckState::ECS_RIGHT_TYPE_WRONG_ADJ)
		{
			++n;
		}
	}
	return n;
}

int GroceryList::GetCostOfList() const
{
	int n = 0;
	for (size_t i = 0; i < m_vCheckSlot.size(); ++i)
	{
		n += m_vCheckSlot[i].m_Item.GetCost();
	}
	return n;
}


GroceryList::CheckSlot* GroceryList::FindCheckSlotByType(const Item::EType& eType)
{
	for (size_t i = 0; i < m_vCheckSlot.size(); ++i)
	{
		if (m_vCheckSlot[i].m_Item.GetType() == eType)
		{
			return &m_vCheckSlot[i];
		}
	}
	return nullptr;
}


/*STATIC*/ std::string GroceryList::GetCheckStateString(const ECheckState& eCheckState)
{
	switch (eCheckState)
	{
	default:
	case ECheckState::ECS_INVALID: return "ECS_INVALID";
	case ECheckState::ECS_EMPTY: return "EMPTY";
	case ECheckState::ECS_HIDDEN: return "HIDDEN";
	case ECheckState::ECS_RIGHT_TYPE_WRONG_ADJ: return "RIGHT_TYPE_WRONG_ADJ";
	case ECheckState::ECS_FILLED: return "FILLED";
	case ECheckState::ECS_COUNT: return "ECS_COUNT";
	case ECheckState::ECS_NOT_IN_LIST: return "ECS_NOT_IN_LIST";
	}
}
