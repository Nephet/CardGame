#pragma once

#include <vector>

#include <cstdlib>
#include <iostream>
#include <ctime>

class Card;
class Player;

class CardDealer
{
public:
	CardDealer();
	~CardDealer();

	void CreateCards(int nbToCreate, unsigned int seed);

	void DealACard(Player* player);

private:
	std::vector<Card*>* _cards;

	int _attackMax;
	int _defenseMax;
};

