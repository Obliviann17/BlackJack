#pragma once
#include "Participant.h"

class Player : public Participant
{
public:
	void playTurn(Deck& deck) override;
};

