#pragma once

#include <vector>
#include <memory>

#include "Card.h"

class Deck
{
public:
	Deck();

	void shuffle();
	std::unique_ptr<Card> Draw();

private:
	std::vector<std::unique_ptr<Card>> mCards;
};

