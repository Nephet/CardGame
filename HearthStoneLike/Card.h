#pragma once
#include <iostream>

class Card
{
public:
	Card(int att, int def, int manaNeed);
	~Card();

	void ToString();

	int GetManaNeeded();

	bool TakeDamage(int amount);
	int GetAttack();

	void SetHadAttacked(bool newState);
	bool GetHadAttacked();

private:
	int _attack;
	int _defense;
	int _manaNeeded;

	bool _hadAttacked;
};

