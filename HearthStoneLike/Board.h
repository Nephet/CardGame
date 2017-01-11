#pragma once

#include "Card.h"
#include <vector>

class Board
{
public:
	Board();
	~Board();

	void PrintBoard();
	int GetNbCardsOnBoard();
	bool IsFree();
	void PutCard(Card* theCard);

private:
	std::vector<Card*> _currentCards;
	int maxCardsOnBoard;
};

