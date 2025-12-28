#pragma once
#include <vector>
#include <iostream>
#include "Move.h"

class Board {
protected:
    int size;
    std::vector<std::vector<PlayerColor>> grid;

public:
    Board(int s) : size(s), grid(s, std::vector<PlayerColor>(s, PlayerColor::NONE)) {}

    int getSize() const { return size; }

    PlayerColor at(int x, int y) const {
        return grid[x][y];
    }

    bool isInside(int x, int y) const {
        return x >= 0 && x < size && y >= 0 && y < size;
    }

    bool place(int x, int y, PlayerColor c) {
        if (!isInside(x, y) || grid[x][y] != PlayerColor::NONE)
            return false;
        grid[x][y] = c;
        return true;
    }

    void remove(int x, int y) {
        grid[x][y] = PlayerColor::NONE;
    }

    void display() const {
        std::cout << "  ";
        for (int i = 0; i < size; i++) std::cout << i << " ";
        std::cout << "\n";
        for (int i = 0; i < size; i++) {
            std::cout << i << " ";
            for (int j = 0; j < size; j++) {
                char c = '.';
                if (grid[i][j] == PlayerColor::BLACK) c = 'X';
                if (grid[i][j] == PlayerColor::WHITE) c = 'O';
                std::cout << c << " ";
            }
            std::cout << "\n";
        }
    }
};
