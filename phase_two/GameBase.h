#pragma once
#include <memory>
#include <vector>
#include "Board.h"

class GameBase {
protected:
    std::unique_ptr<Board> board;
    PlayerColor currentPlayer;
    bool gameOver = false;

public:
    virtual ~GameBase() = default;

    virtual void start(int size) = 0;
    virtual bool makeMove(const Move& move) = 0;
    virtual bool checkGameOver() = 0;
    virtual PlayerColor getWinner() const = 0;
    virtual void display() const = 0;
    virtual bool undo() = 0;

    PlayerColor getCurrentPlayer() const {
        return currentPlayer;
    }

    bool isGameOver() const {
        return gameOver;
    }
};
