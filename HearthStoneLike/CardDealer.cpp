#include "stdafx.h"
#include "CardDealer.h"
#include "Card.h"
#include "Player.h"

CardDealer::CardDealer()
{
	_attackMax = 100;
	_defenseMax = 100;
	_cards = new std::vector<Card*>();
}


CardDealer::~CardDealer()
{
}

void CardDealer::CreateCards(int nbToCreate, unsigned int seed)
{
	std::srand(seed); //use current time as seed for random generator
	for (auto i = 0; i < nbToCreate; ++i)
	{
		int randAttack = std::rand() % (_attackMax + 1);
		int randDefense = std::rand() % (_defenseMax + 1);
		int randMana = std::rand() % (5);

		Card* c = new Card(randAttack, randDefense, randMana);

		int randAttackBis = std::rand() % (_attackMax + 1);
		int randDefenseBis = std::rand() % (_defenseMax + 1);
		int randManaBis = std::rand() % (5);
		Card* c2 = new Card(randAttackBis, randDefenseBis, randManaBis);

		_cards->push_back(c);
		_cards->push_back(c2);
	}
}

void CardDealer::DealACard(Player* player, Player* player2)
{
	player->AddCard(_cards->back());
	_cards->pop_back();
	player2->AddCard(_cards->back());
	_cards->pop_back();
}
