#pragma once
#include <string>

struct Account {
    std::string username;
    std::string password;
    int games = 0;
    int wins = 0;
};
