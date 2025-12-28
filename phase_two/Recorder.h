#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "Move.h"

class Recorder {
private:
    std::vector<Move> records;

public:
    void record(const Move& m) {
        records.push_back(m);
    }

    void clear() {
        records.clear();
    }

    const std::vector<Move>& getRecords() const {
        return records;
    }

    bool save(const std::string& filename) const {
        std::ofstream ofs(filename);
        if (!ofs) return false;
        for (auto& m : records) {
            ofs << m.x << " " << m.y << " " << m.isPass << "\n";
        }
        return true;
    }

    bool load(const std::string& filename) {
        std::ifstream ifs(filename);
        if (!ifs) return false;
        records.clear();
        Move m;
        while (ifs >> m.x >> m.y >> m.isPass) {
            records.push_back(m);
        }
        return true;
    }
};
