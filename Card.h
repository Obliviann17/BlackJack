#pragma once

enum class Suit
{
	Hearts,		// Чирва
	Diamonds,	// Бубна
	Clubs,		// Хреста
	Spades		// Піка
};

enum class Rank
{
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine,
	Ten,
	Jack,
	Queen,
	King,
	Ace

};

class Card
{
public:
	Card(Rank rank, Suit suit);

	Rank getRank() const;
	Suit getSuit() const;
	int getValue() const;

private:
	Rank mRank;
	Suit mSuit;
};