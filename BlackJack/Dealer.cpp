#include "Dealer.h"

void Dealer::playTurn(Deck& deck)
{
	while (getScore() < 17)
	{
		hit(deck);
	}
}
