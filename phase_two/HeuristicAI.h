#pragma once
#include "AIStrategy.h"

class HeuristicAI : public AIStrategy {
public:
    Move decideMove(const Board& board, PlayerColor color) override;
};
