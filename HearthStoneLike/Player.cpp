#include "stdafx.h"
#include "Player.h"
#include "Card.h"

Player::Player()
{
}


Player::~Player()
{
}

void Player::AddCard(Card * card)
{
	_deck.push_back(card);
}

void Player::PickCard()
{
	if (!_deck.empty())
	{
		_hand.push_back(_deck.back());
		_deck.pop_back();
	}
}

void Player::PrintDeck()
{
	std::cout << "New Deck : " << std::endl;
	for each (Card* card in _deck)
	{
		card->ToString();
	}
}

void Player::PrintHand()
{
	std::cout << "Player Hand : " << std::endl;
	int index = 1;
	for each (Card* card in _hand)
	{
		std::cout << index;
		card->ToString();
		index++;
	}
}

Card* Player::ChooseCard(int cardNumberinHand)
{
	if (_hand.size() >= cardNumberinHand)
	{
		return _hand[cardNumberinHand - 1];
	}
	return nullptr;
}