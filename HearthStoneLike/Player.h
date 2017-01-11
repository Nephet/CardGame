#pragma once

#include <vector>
#include <iostream>

class Card;

class Player
{
public:
	Player();
	~Player();

	std::string GetPlayerID();

	void InitPlayer(std::string id);

	void AddCard(Card* card);
	void PickCard();

	void PrintDeck();
	void PrintHand();

	Card* ChooseCard(int cardNumberinHand);
	void RemoveCard(int cardNumberinHand);

private:
	std::vector<Card*> _deck;
	std::vector<Card*> _hand;

	std::string _playerID;
};

