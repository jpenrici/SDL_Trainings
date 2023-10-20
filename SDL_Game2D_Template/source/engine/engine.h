#ifndef CUSTOM_GAME_ENGINE_H
#define CUSTOM_GAME_ENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <array>
#include <initializer_list>
#include <map>
#include <string>

#include "state.h"


// SDL Game Basic
class Engine {

public:
    Engine() = delete;
    Engine(const Engine &) = delete;
    Engine(Engine &&) = delete;

    // Default constructor
    Engine(int screenWidth, int screenHeight, const std::string &title = "");

    virtual ~Engine() = default;

    auto operator=(const Engine &) -> Engine & = delete;
    auto operator=(Engine &&) -> Engine & = delete;

    // Window
    void show();

    // Controller
    virtual auto gameStateEqual(State::Game state) -> bool final;
    virtual void setAsExternalGameloopControl() final;

    // Log
    static void inform(const std::string &message);

    // More than one line.
    virtual void inform(std::initializer_list<std::string> messages, bool multipleLines = false) final;

protected:
    // Window
    virtual auto currentRenderer() -> SDL_Renderer *final;
    virtual void setBackgroundColor(SDL_Color color) final;

    // Game
    virtual auto inputState() -> State::Input final;
    virtual auto inputStateEqual(State::Input state) -> bool final;
    virtual void inputClean() final;
    virtual void setGameState(State::Game state) final;

private:
    struct Performance {
        double deltaTime{0};
        double framesPerSecond{0};
        Uint64 ticks{0};
    } m_gamePerformance;

protected:
    virtual auto gameTitle() -> std::string final;
    virtual auto windowPosition() -> std::array<int, 2> final;

    template<typename T = int>
    auto windowSize() -> std::array<T, 2>
    {
        int height{0};
        int width{0};
        SDL_GetWindowSize(m_window, &width, &height);
        return {static_cast<T>(width), static_cast<T>(height)};
    }

    template<typename T = int>
    auto windowWidth() -> T
    {
        return windowSize().front();
    }

    template<typename T = int>
    auto windowHeight() -> T
    {
        return windowSize().back();
    }

    virtual auto gameState() -> State::Game final;
    virtual auto performanceReport() -> Performance final;

    virtual auto loadFont(const std::string &id, const std::string &filename) -> bool final;
    virtual auto loadMusic(const std::string &id, const std::string &filename) -> bool final;
    virtual auto loadSound(const std::string &id, const std::string &filename) -> bool final;
    virtual auto loadTexture(const std::string &id, const std::string &filename) -> bool final;
    virtual auto loadTexture(const std::string &id, int rows, int  columns, const std::string &filename) -> bool final;

    virtual auto playSound(const std::string &id) -> bool final;
    virtual auto soundExists(const std::string &id) -> bool final;

    virtual auto musicExists(const std::string &id) -> bool final;
    virtual auto playMusic(const std::string &id) -> bool final;
    virtual void pauseMusic() final;
    virtual void resumeMusic() final;
    virtual void stopMusic() final;

    virtual auto renderTextFont(const std::string &fontId, const std::string &text, const SDL_FPoint &position,
                                const SDL_Color &foreground, int fontSize, double angle, float scale,
                                Uint8 opacity, SDL_RendererFlip flip = SDL_FLIP_NONE) -> bool final;

    virtual auto renderTexture(const std::string &id) -> bool final;
    virtual auto renderTexture(const std::string &id, SDL_Rect clip, SDL_FRect box, double angle, float scale, Uint8 opacity, SDL_RendererFlip flip = SDL_FLIP_NONE) -> bool final;
    virtual auto renderTexture(const std::string &id, float x, float y, double angle, float scale, Uint8 opacity) -> bool final;


    virtual auto textFont(const std::string &id) -> TTF_Font *final;
    virtual auto textFontExists(const std::string &id) -> bool final;
    virtual auto texture(const std::string &id) -> SDL_Texture *final;
    virtual auto textureExists(const std::string &id) -> bool final;
    virtual auto textureSize(const std::string &id) -> std::array<int, 2> final;

private:
    // SDL2
    SDL_Window   *m_window   = nullptr;
    SDL_Renderer *m_renderer = nullptr;
    SDL_Event m_event;

    SDL_Color m_bkgColor{0, 0, 0, 255};

    std::map<std::string, Mix_Chunk *> m_sounds;
    std::map<std::string, Mix_Music *> m_songs;
    std::map<std::string, SDL_Texture *> m_textures;
    std::map<std::string, TTF_Font *> m_fonts;

    // Flag for external Game Loop control.
    bool m_externalControl{false};

    // State
    State::Game  m_gameState = State::Game::none;
    State::Input m_inputState = State::Input::none;

    // Config
    std::string m_title{"Game"};
    int m_screenWidth{150};  // minimum horizontal
    int m_screenHeight{150}; // and vertical value

    // Main scope
    virtual void initialize() final;
    virtual void gameLoop() final;
    virtual void finalize() final;

    // Control function
    virtual void inputControl() final;
    virtual void renderScreen() final;
    virtual void updateGameplay() final;

    // Game Scope
    virtual void render() { /* Reserved for custom rendering code. */ };
    virtual void update() { /* Reserved for custom gameplay code and other actions. */ };
    virtual void quit()   { /* Reserved for custom code for freeing/destroying SDL objects. */ };

    // Render
    virtual auto renderTexture(SDL_Texture *texture, SDL_Rect clip, SDL_FRect box, double angle, float scale, Uint8 opacity, SDL_RendererFlip flip = SDL_FLIP_NONE) -> bool final;
};

#endif
