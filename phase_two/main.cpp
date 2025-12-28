#include <iostream>
#include <memory>
#include <cstdlib>
#include <ctime>

#include "GomokuGame.h"
#include "GoGame.h"
#include "ReversiGame.h"

#include "GameController.h"
#include "HumanPlayer.h"
#include "AIPlayer.h"
#include "RandomAI.h"
#include "HeuristicAI.h"

#include "AccountManager.h"
#include "Account.h"
#include "AIStrategy.h"
#include "Player.h"
#include "GameBase.h"
#include "Move.h"
#include "Board.h"


using std::cin;
using std::cout;
using std::endl;

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    cout << "=============================\n";
    cout << "  Board Game Platform (OOP)\n";
    cout << "=============================\n";

    cout << "Select Game Type:\n";
    cout << "1. Gomoku (Five-in-a-Row)\n";
    cout << "2. Go\n";
    cout << "3. Reversi (Othello)\n";
    cout << "Choice: ";

    int gameChoice;
    cin >> gameChoice;

    std::unique_ptr<GameBase> game;
    switch (gameChoice) {
        case 1:
            game = std::make_unique<GomokuGame>();
            break;
        case 2:
            game = std::make_unique<GoGame>();
            break;
        case 3:
            game = std::make_unique<ReversiGame>();
            break;
        default:
            cout << "Invalid game type.\n";
            return 0;
    }

    cout << "\nSelect Play Mode:\n";
    cout << "1. Player vs Player\n";
    cout << "2. Player vs AI\n";
    cout << "3. AI vs AI\n";
    cout << "Choice: ";

    int mode;
    cin >> mode;

    std::unique_ptr<Player> p1;
    std::unique_ptr<Player> p2;

    if (mode == 1) {
        p1 = std::make_unique<HumanPlayer>("Player1");
        p2 = std::make_unique<HumanPlayer>("Player2");
    }
    else if (mode == 2) {
        cout << "Choose AI Level:\n";
        cout << "1. Random AI\n";
        cout << "2. Heuristic AI\n";
        int aiLevel;
        cin >> aiLevel;

        p1 = std::make_unique<HumanPlayer>("Player");

        if (aiLevel == 1) {
            p2 = std::make_unique<AIPlayer>(
                "AI",
                std::make_unique<RandomAI>(),
                PlayerColor::WHITE,
                nullptr
            );
        } else {
            p2 = std::make_unique<AIPlayer>(
                "AI",
                std::make_unique<HeuristicAI>(),
                PlayerColor::WHITE,
                nullptr
            );
        }
    }
    else if (mode == 3) {
        p1 = std::make_unique<AIPlayer>(
            "AI-Black",
            std::make_unique<RandomAI>(),
            PlayerColor::BLACK,
            nullptr
        );
        p2 = std::make_unique<AIPlayer>(
            "AI-White",
            std::make_unique<HeuristicAI>(),
            PlayerColor::WHITE,
            nullptr
        );
    }
    else {
        cout << "Invalid mode.\n";
        return 0;
    }

    GameController controller;
    controller.setGame(std::move(game));
    controller.setPlayers(std::move(p1), std::move(p2));

    cout << "\nGame Start!\n";
    cout << "--------------------------------\n";
    cout << "Input format: x y\n";
    cout << "For Go: input -1 to pass\n";
    cout << "--------------------------------\n";

    controller.run();

    cout << "\nGame Finished.\n";
    return 0;
}
