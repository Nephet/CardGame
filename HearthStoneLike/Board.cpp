#include "stdafx.h"
#include "Board.h"

Board::Board()
{
	maxCardsOnBoard = 5;
}

Board::~Board()
{
}

void Board::PrintBoard()
{
	int index = 1;
	for each (Card* card in _currentCards)
	{
		std::cout << index << " - ";
		card->ToString();
		index++;
	}
}

int Board::GetNbCardsOnBoard()
{
	return (int)_currentCards.size();
}

bool Board::IsFree()
{
	if (GetNbCardsOnBoard() < maxCardsOnBoard)
	{
		return true;
	}
	return false;
}

void Board::PutCard(Card* theCard)
{
	_currentCards.push_back(theCard);
}

Card* Board::GetCard(int index)
{
	return _currentCards.at(index - 1);
}

void Board::RemoveCard(int index)
{
	_currentCards.erase(_currentCards.begin() + (index - 1));
}