#pragma once
#include "Account.h"
#include <map>
#include <fstream>

class AccountManager {
private:
    std::map<std::string, Account> accounts;

public:
    bool load(const std::string& file);
    bool save(const std::string& file);

    bool registerUser(const std::string& u, const std::string& p);
    Account* login(const std::string& u, const std::string& p);
};
