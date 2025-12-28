#include "GomokuGame.h"

void GomokuGame::start(int size) {
    board = std::make_unique<Board>(size);
    currentPlayer = PlayerColor::BLACK;
    gameOver = false;
    while (!history.empty()) history.pop();
}

bool GomokuGame::makeMove(const Move& move) {
    if (gameOver) return false;
    if (!board->place(move.x, move.y, currentPlayer))
        return false;

    history.push(move);
    if (checkFive(move.x, move.y)) {
        gameOver = true;
        return true;
    }

    currentPlayer = (currentPlayer == PlayerColor::BLACK)
                        ? PlayerColor::WHITE
                        : PlayerColor::BLACK;
    return true;
}

bool GomokuGame::checkFive(int x, int y) {
    static int dx[4] = {1, 0, 1, 1};
    static int dy[4] = {0, 1, 1, -1};

    for (int d = 0; d < 4; d++) {
        int cnt = 1;
        for (int k = 1; k < 5; k++) {
            int nx = x + dx[d] * k;
            int ny = y + dy[d] * k;
            if (!board->isInside(nx, ny) || board->at(nx, ny) != currentPlayer)
                break;
            cnt++;
        }
        for (int k = 1; k < 5; k++) {
            int nx = x - dx[d] * k;
            int ny = y - dy[d] * k;
            if (!board->isInside(nx, ny) || board->at(nx, ny) != currentPlayer)
                break;
            cnt++;
        }
        if (cnt >= 5) return true;
    }
    return false;
}

bool GomokuGame::checkGameOver() {
    return gameOver;
}

PlayerColor GomokuGame::getWinner() const {
    return gameOver ? currentPlayer : PlayerColor::NONE;
}

void GomokuGame::display() const {
    board->display();
}

bool GomokuGame::undo() {
    if (history.empty()) return false;
    Move last = history.top();
    history.pop();
    board->remove(last.x, last.y);
    currentPlayer = (currentPlayer == PlayerColor::BLACK)
                        ? PlayerColor::WHITE
                        : PlayerColor::BLACK;
    gameOver = false;
    return true;
}
