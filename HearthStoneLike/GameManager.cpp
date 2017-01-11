#include "stdafx.h"
#include "GameManager.h"


GameManager::GameManager()
{
	_nbOfCards = 20;
	_player1Turn = true;
	
}

GameManager::~GameManager()
{

}

void GameManager::Init(unsigned int seed)
{
	InitGame(seed);
	DealCards();

	PickCards(_player1, 5);
	PickCards(_player2, 5);
}

int GameManager::SelectPlayer(std::string id)
{
	if (_player1->GetPlayerID() == "NONE")
	{
		std::cout << "USE PLAYER 1" << std::endl;
		_player1->InitPlayer(id);
		return 1;
	}
	else
	{
		std::cout << "USE PLAYER 2" << std::endl;
		_player2->InitPlayer(id);
		return 2;
	}
}

bool GameManager::GetPlayerTurn()
{
	return _player1Turn;
}

void GameManager::EndOfTurn()
{
	_player1Turn = !_player1Turn;
}

void GameManager::PrintHands(bool thePlayer)
{
	std::cout << "-------------------------------------------" << std::endl;
	std::cout << "CARDS" << std::endl;
	std::cout << "-------------------------------------------" << std::endl;
	if (thePlayer)
	{
		std::cout << "Current Hand :" << std::endl;
		GetPlayer1()->PrintHand();
		std::cout << "Enemy Hand :" << std::endl;
		GetPlayer2()->PrintHand();
		std::cout << std::endl;
	}
	else
	{
		std::cout << "Current Hand :" << std::endl;
		GetPlayer2()->PrintHand();
		std::cout << "Enemy Hand :" << std::endl;
		GetPlayer1()->PrintHand();
		std::cout << std::endl;
	}
	std::cout << "-------------------------------------------" << std::endl;
	std::cout << "-------------------------------------------" << std::endl;
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

