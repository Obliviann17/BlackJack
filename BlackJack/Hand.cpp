#include "Hand.h"

void Hand::addCard(std::unique_ptr<Card> card)
{
	mCards.push_back(std::move(card));
}

void Hand::clear()
{
	mCards.clear();
}

int Hand::getScore() const
{
	int score = 0;
	int aceCount = 0;

	for (const auto& card : mCards)
	{
		int value = card->getValue();
		score += value;

		if (card->getRank() == Rank::Ace)
		{
			aceCount++;
		}
	}

	while (score > 21 && aceCount > 0)
	{
		score -= 10;
		aceCount--;
	}

	return score;
}

size_t Hand::getCardCount() const
{
	return mCards.size();
}

const std::vector<std::unique_ptr<Card>>& Hand::getCards() const
{
	
}
