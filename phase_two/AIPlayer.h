#pragma once
#include "Player.h"
#include "AIStrategy.h"
#include <memory>

class AIPlayer : public Player {
private:
    std::unique_ptr<AIStrategy> strategy;
    const Board* board;
    PlayerColor color;

public:
    AIPlayer(const std::string& n,
             std::unique_ptr<AIStrategy> s,
             PlayerColor c,
             const Board* b)
        : Player(n), strategy(std::move(s)), color(c), board(b) {}

    Move getMove() override {
        return strategy->decideMove(*board, color);
    }
};
