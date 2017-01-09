#pragma once
#include "CardDealer.h"
#include "Player.h"
#include "Board.h"

class GameManager
{
public:
	GameManager(unsigned int seed);
	~GameManager();

	void InitGame(unsigned int seed);
	
	Player* GetPlayer1();
	Player* GetPlayer2();

private:

	void DealCards();
	void PickCards(Player* player, int nbCard);

	CardDealer* _cardDealer;
	Player* _player1;
	Player* _player2;

	Board* _player1Board;
	Board* _player2Board;

	int _nbOfCards;

};

