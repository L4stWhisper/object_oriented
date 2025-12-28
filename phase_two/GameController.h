#pragma once
#include <memory>
#include "GameBase.h"
#include "Player.h"
#include "Recorder.h"

class GameController {
private:
    std::unique_ptr<GameBase> game;
    std::unique_ptr<Player> p1;
    std::unique_ptr<Player> p2;
    Recorder recorder;

public:
    void setGame(std::unique_ptr<GameBase> g);
    void setPlayers(std::unique_ptr<Player> a,
                    std::unique_ptr<Player> b);
    void run();
};
