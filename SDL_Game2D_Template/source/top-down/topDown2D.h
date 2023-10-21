#ifndef TOPDOWN2D_H
#define TOPDOWN2D_H

#include <array>
#include <set>

#include "../../source/engine/engine2.h"


class Game : public Engine2 {
public:
    Game();

private:
    // Layers
    enum Layer {BACKGROUND = -4, SCENE, OBSTACLES, PLAYER, TEXT, TOP};

    // Sprites
    std::string m_gameOverId{"GameOver"};

    std::array<std::string, 3> m_bkgId {"Background", "Background1", "Background2"};
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
    int m_bulletCounter;
    int m_bulletSpeed;

    void newObstacle();

    // HUD
    float m_playerEnergy{100};
    int m_collisions{0};
    int m_hits{0};
    int m_score{0};

    // Audio
    bool mute{false};

    // Graphics
    void render() final;
    void renderBar(stbox::Rectangle rectangle, RGBA color);

    // Game logic
    void update() final;
};

#endif
