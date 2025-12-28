#pragma once
#include "GameBase.h"

class ReversiGame : public GameBase {
public:
    void start(int size) override;
    bool makeMove(const Move& move) override;
    bool checkGameOver() override;
    PlayerColor getWinner() const override;
    void display() const override;
    bool undo() override;
};
