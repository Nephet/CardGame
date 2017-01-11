#pragma once
#include "CardDealer.h"
#include "Player.h"
#include "Board.h"

class GameManager
{
public:
	GameManager();
	~GameManager();

	void Init(unsigned int seed);

	void InitGame(unsigned int seed);
	
	Player* GetPlayer1();
	Player* GetPlayer2();

	int SelectPlayer(std::string id);

	void EndOfTurn();

	void PrintHands(bool thePlayer);
	void PrintPlayerHand(bool thePlayer);
	void PrintBoards(bool thePlayer);
	void PrintPlayerMana(bool thePlayer);

	bool IsBoardFree(bool thePlayer);
	void PutCardOnBoard(bool thePlayer, Card* theCard);
	void RemoveCardFromPlayer(bool thePlayer, int cardIndex);

	bool GetPlayerTurn();

	void GivePlayerMana();

private:

	void DealCards();
	void PickCards(Player* player, int nbCard);

	CardDealer* _cardDealer;

	Player* _player1;
	Player* _player2;

	Board* _player1Board;
	Board* _player2Board;

	int _nbOfCards;

	int _nbPlayerPlayed;
	int _nbTurn;

	bool _playerTurn;
};

