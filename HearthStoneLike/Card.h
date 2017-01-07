#pragma once
#include <iostream>

class Card
{
public:
	Card(int att, int def);
	~Card();

	void ToString();

private:
	int _attack;
	int _defense;
};

