#include <iostream>

#include "Game.h"

Game* Game::game_instance_ = nullptr;

Game* Game::GetInstance() {
    if (!game_instance_) {
        game_instance_ = new Game{};
    }
    return game_instance_;
}

Game::Game() {
    red_ = new Team{};
    std::cout << red_->GetInfo() << std::endl;
    blue_ = new Team{};
    std::cout << blue_->GetInfo() << std::endl;
}

Game::~Game() {
    delete red_;
    delete blue_;
}

void Game::Run() {
    // TODO: сделать механику игры
    bool red_team_order = true;
    while (!red_->IsEmpty() && !blue_->IsEmpty()) {
        IUnit* red_unit = red_->GetUnit();
        IUnit* blue_unit = red_->GetUnit();
        if (red_team_order) {
            // ...
            red_team_order = false;
        } else {
            // ...
            red_team_order = true;
        }
    }
}

