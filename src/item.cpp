#include "item.h"
#include <algorithm>

Item::Item()
	: m_eAdj(EA_INVALID)
	, m_eType(ET_INVALID)
	, m_nCost(-1)
{

}

Item::Item(const EAdjective& eAdj, const EType& eType, const int& nCost)
	: m_eAdj(eAdj)
	, m_eType(eType)
	, m_nCost(nCost)
{

}

bool Item::IsValid() const
{
	return m_eAdj > EA_INVALID && m_eAdj < EA_COUNT && m_eType > ET_INVALID && m_eType < ET_COUNT && m_nCost > 0;
}

/*STATIC*/ const std::string Item::GetAdjString(const EAdjective& eAdj)
{
	switch (eAdj)
	{
	case EAdjective::EA_INVALID:	return "EA_INVALID";
	case EAdjective::EA_BLUE:		return "BLUE";
	case EAdjective::EA_RED:		return "RED";
	case EAdjective::EA_GREEN:		return "GREEN";
	case EAdjective::EA_WHITE:		return "WHITE";
	case EAdjective::EA_COUNT:		return "EA_COUNT";
	}
	return "";
}

/*STATIC*/ const std::string Item::GetTypeString(const EType& eAdj)
{
	switch (eAdj)
	{
	case EType::ET_INVALID:		return "ET_INVALID";
	case EType::ET_MILK:		return "MILK";
	case EType::ET_EGGS:		return "EGGS";
	case EType::ET_FRUIT:		return "FRUIT";
	case EType::ET_MEAT:		return "MEAT";
	case EType::ET_SODA:		return "SODA";
	case EType::ET_CANDY:		return "CANDY";
	case EType::ET_COUNT:		return "ET_COUNT";
	}
	return "";
}

/*STATIC*/ const Item::EAdjective Item::GetAdjFromString(std::string szAdj)
{
	std::transform(szAdj.begin(), szAdj.end(), szAdj.begin(), ::toupper);

	if (szAdj == "BLUE") return EAdjective::EA_BLUE;
	if (szAdj == "RED") return EAdjective::EA_RED;
	if (szAdj == "GREEN") return EAdjective::EA_GREEN;
	if (szAdj == "WHITE") return EAdjective::EA_WHITE;

	return EAdjective::EA_INVALID;
}

/*STATIC*/ const Item::EType Item::GetTypeFromString(std::string szType)
{
	std::transform(szType.begin(), szType.end(), szType.begin(), ::toupper);

	if (szType == "MILK") return EType::ET_MILK;
	if (szType == "EGGS") return EType::ET_EGGS;
	if (szType == "FRUIT") return EType::ET_FRUIT;
	if (szType == "MEAT") return EType::ET_MEAT;
	if (szType == "SODA") return EType::ET_SODA;
	if (szType == "CANDY") return EType::ET_CANDY;

	return EType::ET_INVALID;
}