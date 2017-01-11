#include "stdafx.h"
#include "Player.h"
#include "Card.h"

Player::Player()
{
	_playerID = "NONE";
}

Player::~Player()
{
}

void Player::InitPlayer(std::string id)
{
	_playerID = id;
}

std::string Player::GetPlayerID()
{
	return _playerID;
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
	for each (Card* card in _deck)
	{
		card->ToString();
	}
}

void Player::PrintHand()
{
	int index = 1;
	for each (Card* card in _hand)
	{
		std::cout << index << " - ";
		card->ToString();
		index++;
	}
}

Card* Player::ChooseCard(int cardNumberinHand)
{
	if (_hand.size() >= cardNumberinHand)
	{
		Card* temp = _hand[cardNumberinHand - 1];

		_hand.erase(_hand.begin() + (cardNumberinHand - 1));

		return temp;
	}
	return nullptr;
}
