
#pragma once
#include <string>
#include "Move.h"

class Player {
protected:
    std::string name;

public:
    explicit Player(const std::string& n) : name(n) {}
    virtual ~Player() = default;

    virtual Move getMove() = 0;
    std::string getName() const { return name; }
};

