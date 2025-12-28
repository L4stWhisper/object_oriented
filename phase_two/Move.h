#pragma once
#include <string>

enum class PlayerColor { BLACK, WHITE, NONE };

struct Move {
    int x;
    int y;
    bool isPass = false;   // 围棋虚着
};
