#pragma once
#include "GameBase.h"
#include <stack>

class GoGame : public GameBase {
private:
    std::stack<Move> history;
    int passCount = 0;

    bool hasLiberty(int x, int y,
                    PlayerColor color,
                    std::vector<std::vector<bool>>& visited);

    void removeDeadStones(PlayerColor opponent);

public:
    void start(int size) override;
    bool makeMove(const Move& move) override;
    bool checkGameOver() override;
    PlayerColor getWinner() const override;
    void display() const override;
    bool undo() override;
};
