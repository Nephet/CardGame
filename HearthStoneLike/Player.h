#pragma once

#include <vector>
#include <iostream>

class Card;

class Player
{
public:
	Player();
	~Player();

	void AddCard(Card* card);
	void PickCard();

	void PrintDeck();

private:
	std::vector<Card*> _deck;
	std::vector<Card*> _hand;
};

