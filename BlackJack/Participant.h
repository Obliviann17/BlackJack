#pragma once
#include "Hand.h"
#include "Deck.h"

class Participant
{
public:
	virtual ~Participant() = default;

	void hit(Deck& deck)
	{
		auto card = deck.Draw();
		if (card)
		{
			mHand.addCard(std::move(card));
		}
	};
	void clearHand() { return mHand.clear(); };
	int getScore() const { return mHand.getScore(); };
	bool isBusted() const { return getScore() > 21; };

	const Hand& getHand() const { return mHand; };

	virtual void playTurn(Deck& deck) = 0;

protected:
	Hand mHand;
};

