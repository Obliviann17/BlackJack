#include <iostream>

#include "Deck.h"
#include "Player.h"
#include "Dealer.h"

int main()
{
    Deck deck;
    deck.shuffle();
	
    Player player;
    Dealer dealer;

    player.hit(deck);
    dealer.hit(deck);
    player.hit(deck);
    dealer.hit(deck);

    std::cout << "Player score: " << player.getScore() << std::endl;

    dealer.playTurn(deck);

    std::cout << "Dealer score: " << dealer.getScore() << std::endl;

    return 0;
}

