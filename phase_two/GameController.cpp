#include "GameController.h"
#include <iostream>

void GameController::setGame(std::unique_ptr<GameBase> g) {
    game = std::move(g);
}

void GameController::setPlayers(std::unique_ptr<Player> a,
                                std::unique_ptr<Player> b) {
    p1 = std::move(a);
    p2 = std::move(b);
}

void GameController::run() {
    game->start(8);
    while (!game->isGameOver()) {
        game->display();
        Player* cur =
            game->getCurrentPlayer() == PlayerColor::BLACK
                ? p1.get()
                : p2.get();
        Move m = cur->getMove();
        if (game->makeMove(m)) {
            recorder.record(m);
        } else {
            std::cout << "Invalid move\n";
        }
    }
}
