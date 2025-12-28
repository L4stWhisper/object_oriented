#include "GoGame.h"

void GoGame::start(int size) {
    board = std::make_unique<Board>(size);
    currentPlayer = PlayerColor::BLACK;
    gameOver = false;
    passCount = 0;
    while (!history.empty()) history.pop();
}

bool GoGame::hasLiberty(int x, int y,
                        PlayerColor color,
                        std::vector<std::vector<bool>>& visited) {
    if (!board->isInside(x, y)) return false;
    if (visited[x][y]) return false;
    visited[x][y] = true;

    if (board->at(x, y) == PlayerColor::NONE) return true;
    if (board->at(x, y) != color) return false;

    static int dx[4] = {1, -1, 0, 0};
    static int dy[4] = {0, 0, 1, -1};

    for (int d = 0; d < 4; d++) {
        if (hasLiberty(x + dx[d], y + dy[d], color, visited))
            return true;
    }
    return false;
}

void GoGame::removeDeadStones(PlayerColor opponent) {
    int n = board->getSize();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board->at(i, j) == opponent) {
                std::vector<std::vector<bool>> visited(
                    n, std::vector<bool>(n, false));
                if (!hasLiberty(i, j, opponent, visited)) {
                    board->remove(i, j);
                }
            }
        }
    }
}

bool GoGame::makeMove(const Move& move) {
    if (gameOver) return false;

    if (move.isPass) {
        passCount++;
        history.push(move);
    } else {
        if (!board->place(move.x, move.y, currentPlayer))
            return false;

        removeDeadStones(
            currentPlayer == PlayerColor::BLACK
                ? PlayerColor::WHITE
                : PlayerColor::BLACK);

        passCount = 0;
        history.push(move);
    }

    if (passCount >= 2) {
        gameOver = true;
    }

    currentPlayer =
        (currentPlayer == PlayerColor::BLACK)
            ? PlayerColor::WHITE
            : PlayerColor::BLACK;
    return true;
}

bool GoGame::checkGameOver() {
    return gameOver;
}

PlayerColor GoGame::getWinner() const {
    return PlayerColor::NONE; // 简化：不计算目数
}

void GoGame::display() const {
    board->display();
}

bool GoGame::undo() {
    if (history.empty()) return false;
    // 简化：不完全回滚提子
    history.pop();
    return true;
}
