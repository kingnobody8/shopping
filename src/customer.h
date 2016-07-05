#pragma once
#include "item.h"
#include "grocery_list.h"
#include <vector>

class Customer
{
public:
	Customer();
	Customer(const GroceryList& GroceryList, const int& nMunny);

	bool CanAddItem(const Item& item);
	void AddItem(const Item& item, float discount);

	inline const int& GetMunny() const { return m_nMunny; }
	inline const GroceryList& GetGroceryList() const { return m_GroceryList; }
	inline const std::vector<Item>& GetInventory() const { return m_vInventory; }

	void PrintGroceryList() const;
	void PrintInventory() const;

private:
	int m_nMunny;
	GroceryList m_GroceryList;
	std::vector<Item> m_vInventory;

};