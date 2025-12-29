#include "Card.h"

Card::Card(Rank rank, Suit suit)
	: mRank(rank), mSuit(suit)
{ }

Rank Card::getRank() const
{
	return mRank;
}

Suit Card::getSuit() const
{
	return mSuit;
}

int Card::getValue() const
{
	switch (mRank)
	{
	case Rank::Two:
		return 2;
	case Rank::Three:
		return 3;
	case Rank::Four:
		return 4;
	case Rank::Five:
		return 5;
	case Rank::Six:
		return 6;
	case Rank::Seven:
		return 7;
	case Rank::Eight:
		return 8;
	case Rank::Nine:
		return 9;
	case Rank::Ten:
	case Rank::Jack:
	case Rank::Queen:
	case Rank::King:
		return 10;
	case Rank::Ace:
		return 11;
	default:
		return 0;
	}
}
