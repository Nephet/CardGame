#include "stdafx.h"
#include "GameManager.h"


GameManager::GameManager(unsigned int seed)
{
	_nbOfCards = 20;
	
	InitGame(seed);
	DealCards();

	PickCards(_player1, 5);
	PickCards(_player2, 5);

	std::cout << "hand j1" << std::endl;
	_player1->PrintHand();
	std::cout << "hand j2" << std::endl;
	_player2->PrintHand();
}


GameManager::~GameManager()
{
}

void GameManager::InitGame(unsigned int seed)
{
	_cardDealer = new CardDealer();
	_cardDealer->CreateCards(_nbOfCards, seed);

	_player1 = new Player();
	_player2 = new Player();

	_player1Board = new Board();
	_player2Board = new Board();

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
	for (int i = 0; i < _nbOfCards; i++)
	{
		_cardDealer->DealACard(_player1,_player2);
	}
}

void GameManager::PickCards(Player* player, int nbCard)
{
	for (int i = 0; i < nbCard; i++)
	{
		player->PickCard();
	}
}

