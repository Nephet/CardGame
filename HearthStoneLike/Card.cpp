#include "stdafx.h"
#include "Card.h"


Card::Card(int att, int def)
{
	_attack = att;
	_defense = def;
}


Card::~Card()
{
}

void Card::ToString()
{
	std::cout << "att :" << _attack << " def :" << _defense << std::endl;
}
