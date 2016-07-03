#include "shopper.h"
#include "shopper_controller.h"

Shopper::Shopper()
{
	m_controller = new ShopperController(this);
	SetSkin(1 + rand() % 3);
}

Shopper::~Shopper()
{}