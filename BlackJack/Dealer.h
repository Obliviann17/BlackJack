#pragma once
#include "Participant.h"

class Dealer : public Participant
{
public:
	void playTurn(Deck& deck) override;
};

