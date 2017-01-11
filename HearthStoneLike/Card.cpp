#include "stdafx.h"
#include "Card.h"

Card::Card(int att, int def, int manaNeed)
{
	_attack = att;
	_defense = def;
	_manaNeeded = manaNeed;
}

Card::~Card()
{
}

void Card::ToString()
{
	std::cout << "att :" << _attack << " def :" << _defense << " manaNeeded :" << _manaNeeded << std::endl;
}

int Card::GetManaNeeded()
{
	return _manaNeeded;
}

bool Card::TakeDamage(int amount)
{
	_defense -= amount;
	if (_defense <= 0)
	{
		return true;
	}
	return false;
}

int Card::GetAttack()
{
	return _attack;
}