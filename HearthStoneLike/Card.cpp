#include "stdafx.h"
#include "Card.h"

Card::Card(int att, int def, int manaNeed)
{
	_attack = att;
	_defense = def;
	_manaNeeded = manaNeed;
	_hadAttacked = false;
}

Card::~Card()
{
}

void Card::ToString()
{
	std::string canAttack = _hadAttacked ? "no" : "yes";
	std::cout << "att :" << _attack << " def :" << _defense << " manaNeeded :" << _manaNeeded << " can attack : " << canAttack << std::endl;
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

void Card::SetHadAttacked(bool newState)
{
	_hadAttacked = newState;
}

bool Card::GetHadAttacked()
{
	return _hadAttacked;
}
