#pragma once
#include <vector>
#include <memory>

#include "Card.h"

class Hand
{
public:
	void addCard(std::unique_ptr<Card> card);
	void clear();
	int getScore() const;
	size_t getCardCount() const;

	const std::vector<std::unique_ptr<Card>>& getCards() const;

private:
	std::vector<std::unique_ptr<Card>> mCards;
};

