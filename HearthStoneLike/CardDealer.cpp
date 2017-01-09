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

		_cards->push_back(new Card(randAttack, randDefense));
	}
}

void CardDealer::DealACard(Player* player, Player* player2)
{
	player->AddCard(_cards->back());
	player2->AddCard(_cards->back());
	_cards->pop_back();
}