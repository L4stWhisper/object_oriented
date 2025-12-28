#include <cassert>
#include <iostream>

#include "GomokuGame.h"
#include "GoGame.h"
#include "ReversiGame.h"

void test_gomoku_win() {
    GomokuGame game;
    game.start(8);

    game.makeMove({0,0,false}); // B
    game.makeMove({1,0,false}); // W
    game.makeMove({0,1,false}); // B
    game.makeMove({1,1,false}); // W
    game.makeMove({0,2,false}); // B
    game.makeMove({1,2,false}); // W
    game.makeMove({0,3,false}); // B
    game.makeMove({1,3,false}); // W
    game.makeMove({0,4,false}); // B wins

    assert(game.isGameOver());
    assert(game.getWinner() == PlayerColor::BLACK);

    std::cout << "[PASS] Gomoku win detection\n";
}

void test_go_pass_end() {
    GoGame game;
    game.start(8);

    game.makeMove({-1,-1,true}); // B pass
    game.makeMove({-1,-1,true}); // W pass

    assert(game.isGameOver());

    std::cout << "[PASS] Go double-pass end\n";
}

void test_reversi_start() {
    ReversiGame game;
    game.start(8);

    assert(game.getCurrentPlayer() == PlayerColor::BLACK);

    std::cout << "[PASS] Reversi initial state\n";
}

int main() {
    test_gomoku_win();
    test_go_pass_end();
    test_reversi_start();

    std::cout << "\nAll tests passed.\n";
    return 0;
}
