#include "stdafx.h"
#include "GameManager.h"


GameManager::GameManager()
{
	InitGame();
	DealCards();

	PickCards(_player1, 5);
	PickCards(_player2, 5);
}


GameManager::~GameManager()
{
}

void GameManager::InitGame()
{
	_cardDealer = new CardDealer();
	_player1 = new Player();
	_player2 = new Player();

	_cardDealer->CreateCards(nbOfCards);
}

Player * GameManager::GetPlayer1()
{
	return _player1;
}

Player * GameManager::GetPlayer2()
{
	return _player2;
}

void GameManager::DealCards()
{
	for (int i = 0; i < nbOfCards; i++)
	{
		_cardDealer->DealACard(_player1);
		_cardDealer->DealACard(_player2);
	}
}

void GameManager::PickCards(Player* player, int nbCard)
{
	for (int i = 0; i < nbCard; i++)
	{
		player->PickCard();
	}
}

