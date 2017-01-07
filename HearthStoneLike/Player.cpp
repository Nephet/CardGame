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
