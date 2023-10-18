#include <chrono>
#include <iostream>

#include "example.h"


void basicTest();
void externalControlTest();

auto main() -> int
{
    basicTest();
    //externalControlTest();

    return 0;
}

void basicTest()
{
    Example game;
    game.show();

    std::cout << "Basic test finished!\n";
}

void externalControlTest()
{
    Engine game(200, 200, "External Control");
    game.setAsExternalGameloopControl();

    int maxSeconds{15};
    int64_t last{0};
    auto start = std::chrono::high_resolution_clock::now();
    do {
        game.show();
        auto end = std::chrono::high_resolution_clock::now();
        auto timer = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
        if (timer > maxSeconds) {
            break;  // Exit Game Loop.
        }
        if (timer > last) {
            game.inform(std::to_string(maxSeconds - timer) + " seconds remaining ...");
            last = timer;
        }
    }
    while (!game.gameStateEqual(State::Game::quit));

    std::cout << "External control test finished!\n";
}
