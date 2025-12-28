#include "AccountManager.h"

bool AccountManager::load(const std::string& file) {
    std::ifstream ifs(file);
    if (!ifs) return false;
    Account a;
    while (ifs >> a.username >> a.password >> a.games >> a.wins) {
        accounts[a.username] = a;
    }
    return true;
}

bool AccountManager::save(const std::string& file) {
    std::ofstream ofs(file);
    if (!ofs) return false;
    /*for (auto& [_, a] : accounts) {
        ofs << a.username << " " << a.password
            << " " << a.games << " " << a.wins << "\n";
    }*/
   for (auto& kv : accounts) {
    const Account& a = kv.second;
    ofs << a.username << " " << a.password
        << " " << a.games << " " << a.wins << "\n";
}

    return true;
}

bool AccountManager::registerUser(const std::string& u, const std::string& p) {
    if (accounts.count(u)) return false;
    accounts[u] = {u, p, 0, 0};
    return true;
}

Account* AccountManager::login(const std::string& u, const std::string& p) {
    if (!accounts.count(u)) return nullptr;
    if (accounts[u].password != p) return nullptr;
    return &accounts[u];
}
