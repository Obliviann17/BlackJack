#include "Deck.h"

#include <random>
#include <algorithm>
#include <chrono>

Deck::Deck()
{
	for (int s = 0; s < 4; ++s)
	{
		for (int r = 0; r < 13; ++r)
		{
			mCards.push_back(std::make_unique<Card>(static_cast<Rank>(r), static_cast<Suit>(s)));
		}
	}
}

void Deck::shuffle()
{
	unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
	std::mt19937 generator(seed);

	std::shuffle(mCards.begin(), mCards.end(), generator);
}

std::unique_ptr<Card> Deck::Draw()
{
	if (!mCards.empty())
	{
		std::unique_ptr<Card> drawnCard = std::move(mCards.back());
		mCards.pop_back();
		return drawnCard;
	}
	return nullptr;
}
