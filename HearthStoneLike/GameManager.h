#pragma once
#include "CardDealer.h"
#include "Player.h"

class GameManager
{
public:
	GameManager();
	~GameManager();

	void InitGame();
	
	Player* GetPlayer1();
	Player* GetPlayer2();


private:

	void DealCards();
	void PickCards(Player* player, int nbCard);

	CardDealer* _cardDealer;
	Player* _player1;
	Player* _player2;

	int nbOfCards = 40;

};

