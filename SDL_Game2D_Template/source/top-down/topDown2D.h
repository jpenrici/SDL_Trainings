#ifndef TOPDOWN2D_H
#define TOPDOWN2D_H

#include <set>

#include "../../source/engine/engine2.h"


class Game : public Engine2 {
public:
    Game();

private:
    std::string m_gameOver{"GameOver"};

    std::string m_bkgId[3] {"Background", "Background1", "Background2"};
    int m_bkgSpeed;

    std::string m_playerId{"Player"};
    int m_playerSpeed;

    std::string m_obstacleId{"Meteor"};
    std::set<std::string> m_obstaclesIds;
    int m_obstacleCounter;
    int m_obstacleSpeed;

    std::string m_bulletId{"Bullet"};
    std::set<std::string> m_bulletIds;
    bool m_bulletLock;
    bool m_shoot;
    int m_bulletCounter;
    int m_bulletSpeed;

    // HUD
    int m_playerEnergy{100};
    int m_collisions{0};
    int m_hits{0};
    int m_score{0};

    // Game
    void newObstacle();
    void renderEnergyBar(Position position, float width, float height);

    void render() final;
    void update() final;
};

#endif
