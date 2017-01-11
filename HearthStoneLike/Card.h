#pragma once
#include <iostream>

class Card
{
public:
	Card(int att, int def, int manaNeed);
	~Card();

	void ToString();

	int GetManaNeeded();

private:
	int _attack;
	int _defense;
	int _manaNeeded;
};

