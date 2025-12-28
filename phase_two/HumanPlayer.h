#pragma once
#include "Player.h"
#include <iostream>

class HumanPlayer : public Player {
public:
    explicit HumanPlayer(const std::string& n) : Player(n) {}

    Move getMove() override {
        Move m;
        std::cin >> m.x;
        if (m.x == -1) {
            m.isPass = true;
            return m;
        }
        std::cin >> m.y;
        m.isPass = false;
        return m;
    }
};
