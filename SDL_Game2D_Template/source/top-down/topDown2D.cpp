#include "topDown2D.h"


Game::Game() : Engine2(854, 480, "Game Test")
{
    auto blue  = RGBA{  0,   0, 255};
    auto green = RGBA{  0, 255,   0};
    auto red   = RGBA{255,   0,   0};
    auto white = RGBA{255, 255, 255};

    // Game
    viewBoxCollider(true);

    // Background
    loadSpriteSheet(m_bkgId[0], "resources/background.png");
    auto bkgSize = spriteSheetSize(m_bkgId[0]);
    m_bkgSpeed = 10;

    newSprite(m_bkgId[1], m_bkgId[0]);
    setSpriteBoxCollider(m_bkgId[1], {{0, 0}, 0, bkgSize.height});
    setSpriteLayer(m_bkgId[1], BACKGROUND);
    setSpriteOpacity(m_bkgId[1], 50);

    newSprite(m_bkgId[2], m_bkgId[0]);
    setSpriteBoxCollider(m_bkgId[2], {{0, -bkgSize.height}, 1, bkgSize.height});
    setSpriteLayer(m_bkgId[2], BACKGROUND);
    setSpriteOpacity(m_bkgId[2], 60);

    // Game Over
    loadSpriteSheet(m_gameOverId, "resources/gameOver.png");
    newSprite(m_gameOverId, m_gameOverId);
    setSpritePosition(m_gameOverId, {60, 50});
    setSpriteLayer(m_gameOverId, TOP);
    setSpriteActivity(m_gameOverId, State::Activity::disabled);

    // Player
    loadSpriteSheet(m_playerId, 1, 6, "resources/spaceship.png");
    newSprite(m_playerId, m_playerId, {{300, 380}, 64, 64});
    newAnimation(m_playerId, m_playerId, {5, 1, 5, 3, 2, 4}, 8);
    setAnimation(m_playerId, m_playerId);
    setSpriteScale(m_playerId, 0.5);
    setSpriteLayer(m_playerId, PLAYER);
    m_playerSpeed = 380;

    // Bullet
    loadSpriteSheet(m_bulletId, 1, 2, "resources/bullet.png");
    m_bulletCounter = 0;
    m_bulletLock = false;
    m_bulletSpeed = 10;

    // Obstacle
    loadSpriteSheet(m_obstacleId, 1, 2, "resources/meteor.png");
    m_obstacleCounter = 0;
    m_obstacleSpeed = 50;
    for (int i = 0; i < 80; ++i) {
        newObstacle();
    }

    // Load Audio
    loadMusic("music", "resources/music.mp3");
    loadSound("alert", "resources/alert.mp3");
    loadSound("explosion", "resources/explosion.mp3");
    loadSound("shot", "resources/shot.mp3");

    // Load and configure resources.
    std::string font = "NotoSansBlack";
    loadFont(font, "resources/NotoSans-Black.ttf");

    // Texts - Layer 0
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
    newText("Help", font, str, {5, windowHeight<float>() - 20}, white, 14);

    setTextActivity("Pause", State::Activity::disabled);
    setTextOpacity("Player", 50);
    setGameState(State::Game::playing);
    if (!mute) {
        playMusic("music");
    }
}

void Game::newObstacle()
{
    std::string obstacleId = m_obstacleId + std::to_string(m_obstacleCounter++);
    newSprite(obstacleId, m_obstacleId, {{stbox::Math::randomize<float>(windowWidth()), -stbox::Math::randomize<float>(100, 300)}, 48, 48});
    setSpriteLayer(obstacleId, PLAYER);
    if (m_obstacleCounter % 5 != 0) {
        setSpriteEmptyBoxCollider(obstacleId);
        setSpriteOpacity(obstacleId, 80);
        setSpriteByIndex(obstacleId, 1);
        setSpriteLayer(obstacleId, OBSTACLES);
        setSpriteScale(obstacleId, stbox::Math::randomize<float>(2, 4) * 0.1);
        setSpritePosition(obstacleId, {stbox::Math::randomize<float>(windowWidth()), -stbox::Math::randomize<float>(2 * windowHeight())});
    }
    m_obstaclesIds.emplace(obstacleId);
}

void Game::render()
{
    Engine2::render();

    auto player = std::get<0>(sprite(m_playerId));
    float width = m_playerEnergy < 0 ? 0 : player.width * m_playerEnergy / 100;
    renderBar({{player.position.X.value, player.position.Y.value + player.height + 5}, width, 5}, {255, 120, 0, 128});
}

void Game::renderBar(stbox::Math::Rectangle rectangle, RGBA color)
{
    SDL_FRect rect{rectangle.origin.X.toFloat(), rectangle.origin.Y.toFloat(), rectangle.width<float>(), rectangle.height<float>()};
    SDL_SetRenderDrawColor(currentRenderer(), color.R, color.G, color.B, color.A);
    SDL_RenderFillRectF(currentRenderer(), &rect);
}

void Game::update()
{
    if (gameStateEqual(State::Game::gameOver)) {
        setSpriteActivity(m_gameOverId, State::Activity::activated);
        stopAnimation(m_playerId);
        stopMusic();
        if (inputStateEqual(State::Input::keyboard_space)) {
            // Restart
            playMusic("music");
            setGameState(State::Game::playing);
            setSpriteActivity(m_gameOverId, State::Activity::disabled);
            restartAnimation(m_playerId);
            m_playerEnergy = 100;
            m_collisions = 0;
            m_hits = 0;
            m_score = 0 ;
        }
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
    setText("Energy", "Energy: " + std::to_string(static_cast<int>(m_playerEnergy)));
    setTextPosition("Player", player.position + Position(player.width + 5, player.height + 5));

    // Events.
    if (inputStateEqual(State::Input::keyboard_escape)) {
        if (gameStateEqual(State::Game::playing)) {
            setGameState(State::Game::paused);
            setTextActivity("Pause", State::Activity::activated);
            stopAnimation(m_playerId);
            inform("Game paused!");
            if (!mute) {
                pauseMusic();
            }
        }
        else if (gameStateEqual(State::Game::paused)) {
            setGameState(State::Game::playing);
            setTextActivity("Pause", State::Activity::disabled);
            restartAnimation(m_playerId);
            inform("Game playing!");
            if (!mute) {
                resumeMusic();
            }
        }
    }

    if (gameStateEqual(State::Game::paused)) {
        return;
    }

    // Only horizontal and vertical movement.
    player.move4directions(m_playerSpeed * performanceReport().deltaTime, {{0, 0},
        windowWidth<double>(), windowHeight<double>()
    }, inputState());
    setSpritePosition(player.id, player.position);

    // Shot separated from movement.
    if (inputStateEqual(State::Input::keyboard_space) && !m_bulletLock) {
        std::string bulletId = m_bulletId + std::to_string(m_bulletCounter % 10);
        newSprite(bulletId, m_bulletId, {player.center(), 8, 16});
        setSpriteScale(bulletId, 0.3);
        m_bulletIds.emplace(bulletId);
        m_bulletLock = true;
        m_bulletCounter++;
        if (!mute) {
            playSound("shot");
        }
    }

    // Update Obstacle.
    for (const auto &obstacleId : m_obstaclesIds) {
        auto obstacle = std::get<0>(sprite(obstacleId));
        auto repositionObstacle = false;
        //obstacle.moveVertical(m_obstacleSpeed * performanceReport().deltaTime);
        obstacle.move(m_obstacleSpeed * performanceReport().deltaTime, 85);
        obstacle.rotate(2);
        // Collision : Player x Obstacle
        if (checkCollision(m_playerId, obstacle.id)) {
            repositionObstacle = true;
            m_collisions++;
            m_playerEnergy -= 5;
            m_score -= 5;
            if (!mute) {
                playSound("alert");
            }
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
            auto bullet = std::get<0>(result);
            if (std::get<bool>(result)) {
                if (bullet.activity == State::Activity::activated) {
                    // Collision: Bullet x Obstacle
                    if (checkCollision(bullet.id, obstacle.id) && bullet.animation.frame == 0) {
                        repositionObstacle = true;
                        setSpriteLayer(bulletId, 2);
                        setSpriteByIndex(bullet.id, 1);
                        m_bulletLock = false;
                        m_score += 10;
                        m_hits++;
                        if (!mute) {
                            playSound("explosion");
                        }
                    }
                    bullet.move(m_bulletSpeed * performanceReport().deltaTime, -90);
                    if (bullet.animation.frame == 1) {
                        setSpriteScale(bulletId, bullet.scale + 0.005);
                    }
                }
                setSpriteBoxCollider(bullet.id, bullet.boxCollider());
                if (bullet.position.Y.value < 0) {
                    eraseSprite(bullet.id);
                    m_bulletLock = false;
                }
            }
        }
        if (repositionObstacle) {
            obstacle.position = {stbox::Math::randomize<double>(0, windowWidth()), -stbox::Math::randomize<double>(100, 200)};
            obstacle.angle = stbox::Math::randomize(1, 360);
        }
        swapSprite(obstacle.id, obstacle);
    }

    // Update Background
    for (size_t i = 1; i < m_bkgId.size(); ++i) {
        auto background = std::get<0>(sprite(m_bkgId.at(i)));
        background.moveVertical(m_bkgSpeed * performanceReport().deltaTime);
        background.moveTo({0, -background.height}, windowHeight(), true, true);
        setSpritePosition(background.id, background.position);
    }
}
