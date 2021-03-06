#include "customer.h"
#include <assert.h>

Customer::Customer()
	: m_nMunny(-1)
{
}

Customer::Customer(const GroceryList& GroceryList, const int& nMunny)
	: m_GroceryList(GroceryList)
	, m_nMunny(nMunny)
{
}


bool Customer::CanAddItem(const Item& item)
{
	return m_GroceryList.GetItemCheckType(item.GetType()) == GroceryList::ECheckState::ECS_EMPTY && item.GetCost() <= m_nMunny;
}

void Customer::AddItem(const Item& item, float discount)
{
	assert(item.IsValid() && CanAddItem(item));

	m_vInventory.push_back(item);
	m_GroceryList.CheckoutItem(item);

	m_nMunny -= (int) (item.GetCost() * discount);
}


void Customer::PrintGroceryList() const
{
	printf("Grocery List:\n");
	m_GroceryList.PrintList();
	printf("\n");
}

void Customer::PrintInventory() const
{
	printf("Inventory:\n");
	printf("$%d\n", m_nMunny);
	for (size_t i = 0; i < m_vInventory.size(); ++i)
	{
		printf("%d: %s\n", i, m_vInventory[i].GetItemName().c_str());
	}
	printf("\n");
}