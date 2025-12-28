#include "ReversiGame.h"

void ReversiGame::start(int size) {
    board = std::make_unique<Board>(size);
    currentPlayer = PlayerColor::BLACK;
    gameOver = false;

    int mid = size / 2;
    board->place(mid - 1, mid - 1, PlayerColor::WHITE);
    board->place(mid, mid, PlayerColor::WHITE);
    board->place(mid - 1, mid, PlayerColor::BLACK);
    board->place(mid, mid - 1, PlayerColor::BLACK);
}

bool ReversiGame::makeMove(const Move& move) {
    if (!board->place(move.x, move.y, currentPlayer))
        return false;

    currentPlayer = (currentPlayer == PlayerColor::BLACK)
                        ? PlayerColor::WHITE
                        : PlayerColor::BLACK;
    return true;
}

bool ReversiGame::checkGameOver() {
    return false; // 简化
}

PlayerColor ReversiGame::getWinner() const {
    return PlayerColor::NONE;
}

void ReversiGame::display() const {
    board->display();
}

bool ReversiGame::undo() {
    return false;
}
