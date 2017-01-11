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
	void PrintCurrentMana();

	Card* ChooseCard(int cardNumberinHand);
	void RemoveCard(int cardNumberinHand);

	int GetCurrentMana();
	int GetMaxMana();

	void RefillMana(int mana);
	void RemoveMana(int mana);

private:
	std::vector<Card*> _deck;
	std::vector<Card*> _hand;

	std::string _playerID;

	int _currentMana;
	int _manaMax;
};

