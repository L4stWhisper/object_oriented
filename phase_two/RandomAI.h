#pragma once
#include "AIStrategy.h"
#include <random>

class RandomAI : public AIStrategy {
public:
    Move decideMove(const Board& board, PlayerColor color) override;
};
