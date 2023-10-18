#include "topDown2D.h"


Game::Game() : Engine2(600, 480, "Game Test")
{
    auto blue  = RGBA{  0,   0, 255};
    auto green = RGBA{  0, 255,   0};
    auto red   = RGBA{255,   0,   0};
    auto white = RGBA{255, 255, 255};

    // Game
    viewBoxCollider();

    // Background
    loadSpriteSheet(m_bkgId[0], "resources/background.png");
    auto size = spriteSheetSize(m_bkgId[0]);
    m_bkgSpeed = 10;

    newSprite(m_bkgId[1], m_bkgId[0]);
    setSpriteBox(m_bkgId[1], {{0, 0}, size.width, size.height});
    setSpriteLayer(m_bkgId[1], -2);

    newSprite(m_bkgId[2], m_bkgId[0]);
    setSpriteBox(m_bkgId[2], {{0, -size.height}, size.width, size.height});
    setSpriteLayer(m_bkgId[2], -2);

    // Game Over
    loadSpriteSheet(m_gameOver, "resources/gameOver.png");
    newSprite(m_gameOver, m_gameOver);
    setSpritePosition(m_gameOver, {60, 50});
    setSpriteLayer(m_gameOver, 10);
    setSpriteActivity(m_gameOver, State::Activity::disabled);

    // Player
    loadSpriteSheet(m_playerId, 1, 6, "resources/spaceship.png");
    newSprite(m_playerId, m_playerId, {{300, 380}, 96, 96});
    newAnimation(m_playerId, m_playerId, {0, 1, 2}, 10);
    setAnimation(m_playerId, m_playerId);
    setSpriteLayer(m_playerId, 1);
    m_playerSpeed = 380;

    // Bullet
    loadSpriteSheet(m_bulletId, 1, 2, "resources/bullet.png");
    m_bulletCounter = 0;
    m_bulletLock = false;
    m_bulletSpeed = 10;

    // Obstacle
    loadSpriteSheet(m_obstacleId, "resources/meteor.png");
    m_obstacleCounter = 0;
    m_obstacleSpeed = 50;
    for (int i = 0; i < 80; ++i) {
        newObstacle();
    }

    // Load and configure resources.
    auto font = "NotoSansBlack";
    loadFont(font, "resources/NotoSans-Black.ttf");

    // Texts
    newText("Title", font, "Top Down", {windowWidth<float>() - 100, 10}, white, 20);
    newText("Dt", font, "", {150, 10}, green, 14);
    newText("FPS", font, "", {150, 25}, green, 14);
    newText("Ticks", font, "", {150, 40}, green, 14);
    newText("Player", font, "", {0, 0}, white, 13);
    newText("Pause", font, "PAUSED!", {5, 80}, red, 48);
    newText("Score", font, "", {5, 10}, white, 14);
    newText("Hits", font, "", {5, 25}, white, 14);
    newText("Shots", font, "", {5, 40}, white, 14);
    newText("Collisions", font, "", {5, 55}, white, 14);
    newText("Energy", font, "", {5, 70}, white, 14);

    std::string str = "Use WASD or arrows to move, Space to shot and Esc to pause.";
    newText("Help", font, str, {5, windowHeight() - 20.f}, white, 14);

    setTextActivity("Pause", State::Activity::disabled);

    setGameState(State::Game::playing);
}

void Game::newObstacle()
{
    std::string obstacleId = m_obstacleId + std::to_string(m_obstacleCounter++);
    newSprite(obstacleId, m_obstacleId, {{
            stbox::Math::randomize<float>(0, windowWidth()),
            -stbox::Math::randomize<float>(100, 300)
        }, 48, 48
    });
    if (m_obstacleCounter % 5 != 0) {
        setSpriteBox(obstacleId, 10, 10);
        setSpriteLayer(obstacleId, -1);
        setSpriteScale(obstacleId, stbox::Math::randomize(2, 4) * 0.1);
        setSpriteSolid(obstacleId, false);
        setSpritePosition(obstacleId, {stbox::Math::randomize<float>(0, windowWidth()),
                                       -stbox::Math::randomize<float>(0, 2 * windowHeight())
                                      });
    }
    m_obstaclesIds.emplace(obstacleId);
}

void Game::renderEnergyBar(Position position, float width, float height)
{
    SDL_FRect rect{position.X.toFloat(), position.Y.toFloat(), width, height};
    SDL_SetRenderDrawColor(currentRenderer(), 255, 120, 0, 255);
    SDL_RenderFillRectF(currentRenderer(), &rect);
}

void Game::render()
{
    Engine2::render();

    auto player = std::get<0>(sprite(m_playerId));
    float width = m_playerEnergy < 0 ? 0 : (player.width - 10) * m_playerEnergy / 100;
    renderEnergyBar({player.position.X.toFloat() + 5, player.position.Y.toFloat() + player.height - 10}, width, 5);
}

void Game::update()
{
    if (gameStateEqual(State::Game::gameOver)) {
        setSpriteActivity(m_gameOver, State::Activity::activated);
        setAnimationSpeed(m_playerId, 0);
        return;
    }

    // Player
    auto player = std::get<0>(sprite(m_playerId));

    // Update text.
    setText("Dt", "dT : " + std::to_string(performanceReport().deltaTime));
    setText("FPS", "fps : " + std::to_string(performanceReport().framesPerSecond));
    setText("Ticks", "ticks : " + std::to_string(performanceReport().ticks));
    setText("Player", "Position: " + player.position.toStrInt());
    setText("Score", "Score : " + std::to_string(m_score));
    setText("Hits", "Hits : " + std::to_string(m_hits));
    setText("Shots", "Shots : " + std::to_string(m_bulletCounter));
    setText("Collisions", "Collisions: " + std::to_string(m_collisions));
    setText("Energy", "Energy: " + std::to_string(m_playerEnergy));
    setTextPosition("Player", player.position + Position(player.width + 5, player.height + 5));

    // Events.
    if (gameStateEqual(State::Game::playing) && inputStateEqual(State::Input::keyboard_escape)) {
        setTextActivity("Pause", State::Activity::activated);
        setGameState(State::Game::paused);
        setAnimationSpeed(m_playerId, 0);
        inform("Game paused!");
        return;
    }
    if (gameStateEqual(State::Game::paused)) {
        if (inputStateEqual(State::Input::keyboard_escape)) {
            setTextActivity("Pause", State::Activity::disabled);
            setGameState(State::Game::playing);
            setAnimationSpeed(m_playerId, 10);
            inform("Game playing!");
        }
        return;
    }

    float step = m_playerSpeed * performanceReport().deltaTime;
    auto winSize = windowSize<int>();
    switch (inputState()) {
    case State::Input::keyboard_space:
        m_shoot = true;
        break;
    case State::Input::keyboard_up_space:
        m_shoot = true;
    case State::Input::keyboard_up:
        player.move({0, -step}, winSize);
        break;
    case State::Input::keyboard_down_space:
        m_shoot = true;
    case State::Input::keyboard_down:
        player.move({0, step}, winSize);
        break;
    case State::Input::keyboard_left_space:
        m_shoot = true;
    case State::Input::keyboard_left:
        player.move({-step, 0}, winSize);
        break;
    case State::Input::keyboard_right_space:
        m_shoot = true;
    case State::Input::keyboard_right:
        player.move({step, 0}, winSize);
        break;
    case State::Input::keyboard_up_rigth_space:
        m_shoot = true;
    case State::Input::keyboard_up_rigth:
        player.move({step, -step}, winSize);
        break;
    case State::Input::keyboard_down_right_space:
        m_shoot = true;
    case State::Input::keyboard_down_right:
        player.move({step, step}, winSize);
        break;
    case State::Input::keyboard_up_left_space:
        m_shoot = true;
    case State::Input::keyboard_up_left:
        player.move({-step, -step}, winSize);
        break;
    case State::Input::keyboard_down_left_space:
        m_shoot = true;
    case State::Input::keyboard_down_left:
        player.move({-step, step}, winSize);
        break;
    default:
        m_shoot = false;
        break;
    }
    setSpritePosition(player.id, player.position);

    if (m_shoot && !m_bulletLock) {
        std::string bulletId = m_bulletId + std::to_string(m_bulletCounter % 10);
        Position xy = {player.position.X.toFloat() + player.width / 2, player.position.Y.toFloat() + player.height / 2};
        newSprite(bulletId, m_bulletId, {xy, 8, 16});
        m_bulletIds.emplace(bulletId);
        m_bulletLock = true;
        m_bulletCounter++;
    }

    // Update Obstacle.
    for (const auto &obstacleId : m_obstaclesIds) {
        auto obstacle = std::get<0>(sprite(obstacleId));
        auto repositionObstacle = false;
        obstacle.position.Y.value += m_obstacleSpeed * performanceReport().deltaTime;
        obstacle.angle = obstacle.angle > 360 ? 0 : obstacle.angle + 2;
        // Collision : Player x Obstacle
        if (checkCollision(m_playerId, obstacle.id)) {
            repositionObstacle = true;
            m_collisions++;
            m_playerEnergy -= 5;
            m_score -= 5;
            if (m_playerEnergy < 0) {
                setGameState(State::Game::gameOver);
                viewBoxCollider(false);
            }
        }
        if (obstacle.position.Y.value > windowHeight()) {
            repositionObstacle = true;
        }
        // Update Bullet.
        for (const auto &bulletId : m_bulletIds) {
            auto result = sprite(bulletId);
            auto animBullet = std::get<0>(result);
            if (std::get<bool>(result)) {
                if (animBullet.activity == State::Activity::activated) {
                    // Collision: Bullet x Obstacle
                    if (checkCollision(animBullet.id, obstacle.id) && animBullet.animation.frame == 0) {
                        repositionObstacle = true;
                        setSpriteByIndex(animBullet.id, 1);
                        setSpriteScale(animBullet.id, 10);
                        m_bulletLock = false;
                        m_score += 10;
                        m_hits++;
                    }
                    animBullet.position.Y.value -= m_bulletSpeed * performanceReport().deltaTime;
                }
                setSpriteBox(animBullet.id, animBullet.boxCollider());
                if (animBullet.position.Y.value < 0) {
                    eraseSprite(animBullet.id);
                    m_bulletLock = false;
                }
            }
        }
        if (repositionObstacle) {
            obstacle.position.X.value = stbox::Math::randomize(0, windowWidth());
            obstacle.position.Y.value = -stbox::Math::randomize(100, 200);
            obstacle.angle = stbox::Math::randomize(1, 360);
        }
        setSpriteAngle(obstacle.id, obstacle.angle);
        setSpritePosition(obstacle.id, obstacle.position);
    }

    // Update Background
    for (int i = 1; i < 3; ++i) {
        auto background = std::get<0>(sprite(m_bkgId[i]));
        background.position.Y.value += m_bkgSpeed * performanceReport().deltaTime;
        if (background.position.Y.value > windowHeight()) {
            background.position.Y.value = -background.height;
        }
        setSpritePosition(background.id, background.position);
    }
}
