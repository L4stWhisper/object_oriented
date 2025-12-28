#pragma once
#include "GameBase.h"
#include <stack>

class GomokuGame : public GameBase {
private:
    std::stack<Move> history;

    bool checkFive(int x, int y);

public:
    void start(int size) override;
    bool makeMove(const Move& move) override;
    bool checkGameOver() override;
    PlayerColor getWinner() const override;
    void display() const override;
    bool undo() override;
};
