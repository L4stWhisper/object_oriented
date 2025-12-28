#pragma once
#include "Move.h"
#include "Board.h"

class AIStrategy {
public:
    virtual ~AIStrategy() = default;
    virtual Move decideMove(const Board& board, PlayerColor color) = 0;
};
