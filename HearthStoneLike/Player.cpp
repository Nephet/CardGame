#include "stdafx.h"
#include "Player.h"
#include "Card.h"
#include <algorithm>

Player::Player()
{
	_playerID = "NONE";
	_currentMana = 0;
	_manaMax = 10;
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
		return _hand[cardNumberinHand - 1];
	}
	return nullptr;
}

void Player::RemoveCard(int cardNumberinHand)
{
	_hand.erase(_hand.begin() + (cardNumberinHand - 1));
}

void Player::PrintCurrentMana()
{
	std::cout << "Mana remaining : " << _currentMana << std::endl;
}

int Player::GetCurrentMana()
{
	return _currentMana;
}

int Player::GetMaxMana()
{
	return _manaMax;
}

void Player::RefillMana(int mana)
{
	_currentMana += mana;
	if (_currentMana > _manaMax)
	{
		_currentMana = _manaMax;
	}
}

void Player::RemoveMana(int mana)
{
	_currentMana -= mana;
}
