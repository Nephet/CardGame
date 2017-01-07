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
	void PrintHand();

	Card* ChooseCard(int cardNumberinHand);

private:
	std::vector<Card*> _deck;
	std::vector<Card*> _hand;
};

