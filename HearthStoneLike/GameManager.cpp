#include "stdafx.h"
#include "GameManager.h"
#include "Board.h"

GameManager::GameManager()
{
	_nbOfCards = 10;
	_nbTurn = 1;
	_nbPlayerPlayed = 0;
	_playerTurn = true;
	_players = std::vector<size_t>();
	_endGame = false;
	_gameStarted = false;
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

void GameManager::EndOfTurn()
{
	if (_playerTurn)
	{
		GetPlayer1Board()->SetHadAttack(false);
	}
	else
	{
		GetPlayer2Board()->SetHadAttack(false);
	}
	_playerTurn = !_playerTurn;
	_nbPlayerPlayed++;
	if (_nbPlayerPlayed >= 2)
	{
		_nbTurn++;
	}
}

void GameManager::PrintHands(bool thePlayer)
{
	std::cout << "-------------------------------------------" << std::endl;
	std::cout << "CARDS" << std::endl;
	std::cout << "-------------------------------------------" << std::endl;
	std::cout << "Current Hand :" << std::endl;
	if (thePlayer)
	{
		GetPlayer1()->PrintHand();
		std::cout << "Enemy Hand :" << std::endl;
		GetPlayer2()->PrintHand();
	}
	else
	{
		GetPlayer2()->PrintHand();
		std::cout << "Enemy Hand :" << std::endl;
		GetPlayer1()->PrintHand();
	}
	std::cout << std::endl;
	std::cout << "-------------------------------------------" << std::endl;
	std::cout << "-------------------------------------------" << std::endl;
}

void GameManager::PrintPlayerHand(bool thePlayer)
{
	std::cout << "-------------------------------------------" << std::endl;
	std::cout << "CARDS" << std::endl;
	std::cout << "-------------------------------------------" << std::endl;
	std::cout << "Current Hand :" << std::endl;
	if (thePlayer)
		GetPlayer1()->PrintHand();
	else
		GetPlayer2()->PrintHand();
	std::cout << std::endl;
	std::cout << "-------------------------------------------" << std::endl;
	std::cout << "-------------------------------------------" << std::endl;
}

void GameManager::PrintBoards(bool thePlayer)
{
	std::cout << "-------------------------------------------" << std::endl;
	std::cout << "BOARDS" << std::endl;
	std::cout << "-------------------------------------------" << std::endl;
	if (thePlayer)
	{
		std::cout << "Current Board :" << std::endl;
		_player1Board->PrintBoard();
		std::cout << "Enemy Board :" << std::endl;
		_player2Board->PrintBoard();
	}
	else
	{
		std::cout << "Current Board :" << std::endl;
		_player2Board->PrintBoard();
		std::cout << "Enemy Board :" << std::endl;
		_player1Board->PrintBoard();
	}
	std::cout << std::endl;
	std::cout << "-------------------------------------------" << std::endl;
	std::cout << "-------------------------------------------" << std::endl;
}

void GameManager::PrintPlayerMana(bool thePlayer)
{
	if (thePlayer)
	{
		std::cout << "Mana remaining : " << GetPlayer1()->GetCurrentMana() << std::endl;
	}
	else
	{
		std::cout << "Mana remaining : " << GetPlayer2()->GetCurrentMana() << std::endl;
	}
}

void GameManager::PrintPlayerLife(bool thePlayer)
{
	if (thePlayer)
	{
		std::cout << "Life remaining : " << GetPlayer1()->GetLife() << std::endl;
	}
	else
	{
		std::cout << "Life remaining : " << GetPlayer2()->GetLife() << std::endl;
	}
}

void GameManager::PrintPlayersLife(bool thePlayer)
{
	if (thePlayer)
	{
		std::cout << "Current Life remaining : " << GetPlayer1()->GetLife() << std::endl;
		std::cout << "Enemy Life remaining : " << GetPlayer2()->GetLife() << std::endl;
	}
	else
	{
		std::cout << "CurrentLife remaining : " << GetPlayer2()->GetLife() << std::endl;
		std::cout << "Enemy Life remaining : " << GetPlayer1()->GetLife() << std::endl;
	}
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

Player* GameManager::GetPlayer1()
{
	return _player1;
}

Player* GameManager::GetPlayer2()
{
	return _player2;
}

Board* GameManager::GetPlayer1Board()
{
	return _player1Board;
}

Board* GameManager::GetPlayer2Board()
{
	return _player2Board;
}

void GameManager::DealCards()
{
	for (int i = 0; i < _nbOfCards; i++)
	{
		_cardDealer->DealACard(_player1, _player2);
	}
}

void GameManager::PickCards(Player* player, int nbCard)
{
	for (int i = 0; i < nbCard; i++)
	{
		player->PickCard();
	}
}

bool GameManager::IsBoardFree(bool thePlayer)
{
	if (thePlayer)
	{
		if(!_player1Board->IsFree())
		{
			std::cout << "Too many cards on the board !" << std::endl;
			return false;
		}
		return true;
	}
	else
	{
		if (!_player2Board->IsFree())
		{
			std::cout << "Too many cards on the board !" << std::endl;
			return false;
		}
		return true;
	}
}

void GameManager::PutCardOnBoard(bool thePlayer, Card* theCard)
{
	if (thePlayer)
	{
		_player1Board->PutCard(theCard);
	}
	else
	{
		_player2Board->PutCard(theCard);
	}
}

void GameManager::RemoveCardFromPlayer(bool thePlayer, int cardIndex)
{
	if (thePlayer)
	{
		_player1->RemoveCard(cardIndex);
	}
	else
	{
		_player2->RemoveCard(cardIndex);
	}
}

bool GameManager::GetPlayerTurn()
{
	return _playerTurn;
}

void GameManager::GivePlayerMana()
{
	if (_playerTurn)
	{
		GetPlayer1()->RefillMana(_nbTurn * 3);
	}
	else
	{
		GetPlayer2()->RefillMana(_nbTurn * 3);
	}
}

void GameManager::AttackCard(bool thePlayer, Card* theAttackingCard, int attackedCardIndex)
{
	if (thePlayer)
	{
		Card* card = _player2Board->GetCard(attackedCardIndex);
		if (card != nullptr)
		{
			bool isDead = card->TakeDamage(theAttackingCard->GetAttack());
			if (isDead)
			{
				_player2Board->RemoveCard(attackedCardIndex);
			}
		}
	}
	else
	{
		Card* card = _player1Board->GetCard(attackedCardIndex);
		if (card != nullptr)
		{
			bool isDead = card->TakeDamage(theAttackingCard->GetAttack());
			if (isDead)
			{
				_player1Board->RemoveCard(attackedCardIndex);
			}
		}
	}
}

bool GameManager::CheckEndOfGame()
{
	if (_player1->GetLife() <= 0 || _player2->GetLife() <= 0)
	{
		_endGame = true;
		return true;
	}
	return false;
}

bool GameManager::CheckGameFinished()
{
	return _endGame;
}

void GameManager::AddPlayer(int pseudo)
{
	_players.push_back(pseudo);
	if (_players.size() == 2)
	{
		_gameStarted = true;
	}
}

bool GameManager::GetCurrentPlayer(int pseudo)
{
	for (int i = 0; i < _players.size(); i++)
	{
		if (pseudo == _players[i])
		{
			return (bool)i;
		}
	}
}

int GameManager::GetCurrentPlayers()
{
	return _players.size();
}

void GameManager::PrintWinner(int hash)
{
	_endGame = true;
	std::cout << std::endl;
	if (_player1->GetLife() <= 0)
	{
		if (hash == _players[1])
		{
			std::cout << "*******************************************" << std::endl;
			std::cout << "YOU LOOSE" << std::endl;
			std::cout << "*******************************************\n" << std::endl;
		}
		else
		{
			std::cout << "*******************************************" << std::endl;
			std::cout << "YOU WIN" << std::endl;
			std::cout << "*******************************************\n" << std::endl;
		}
	}
	else if (_player2->GetLife() <= 0)
	{
		if (hash == _players[1])
		{
			std::cout << "*******************************************" << std::endl;
			std::cout << "YOU WIN" << std::endl;
			std::cout << "*******************************************\n" << std::endl;
		}
		else
		{
			std::cout << "*******************************************" << std::endl;
			std::cout << "YOU LOOSE" << std::endl;
			std::cout << "*******************************************\n" << std::endl;
		}
	}
	else
	{
		std::cout << "exception : Bug at the end of the game with life..." << std::endl;
	}
	std::cout << std::endl;
}

bool GameManager::GetGameStarted()
{
	return _gameStarted;
}

void GameManager::SendDamageToEnemy(bool thePlayerSender, int amount)
{
	if (thePlayerSender)
	{
		_player2->RemoveLife(amount);
	}
	else
	{
		_player1->RemoveLife(amount);
	}
}