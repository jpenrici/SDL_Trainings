#include "engine.h"
#include "common/smallToolBox/src/smalltoolbox_text.hpp"

Engine::Engine(int screenWidth, int screenHeight, const std::string &title)
{
    m_screenWidth  = screenWidth > m_screenWidth ? screenWidth : m_screenWidth;
    m_screenHeight = screenHeight > m_screenHeight ? screenHeight : m_screenHeight;
    m_title = title.empty() ? m_title : title;

    inform({"Game : C++",
            stbox::Text::toStr("SDL  : ", std::to_string(SDL_MAJOR_VERSION), ".", std::to_string(SDL_MINOR_VERSION)),
            m_title,
            stbox::Text::toStr("Window ", std::to_string(m_screenWidth), " x ", std::to_string(m_screenHeight))},
           true);

    initialize();
}

void Engine::show()
{
    // Start
    if (m_gameState == State::Game::none) {
        m_gameState = State::Game::playing;
    }

    // Run
    if (m_gameState == State::Game::playing || m_gameState == State::Game::paused) {
        gameLoop();
    }

    // End
    if (m_gameState == State::Game::stopped) {
        finalize();
    }
}

void Engine::initialize()
{
    // Prepare SDL
    inform("Initialize the SDL library!");
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        inform({"Failed to initialize SDL!\n", SDL_GetError()});
        exit(EXIT_FAILURE);
    }

    inform("Create Window SDL!");
    m_window = SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                m_screenWidth, m_screenHeight, SDL_WINDOW_SHOWN);
    if (m_window == nullptr) {
        inform({"Failed to create window!\n", SDL_GetError()});
        exit(EXIT_FAILURE);
    }

    inform("Set hint SDL!");
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    inform("Create Renderer SDL!");
    m_renderer =  SDL_CreateRenderer(m_window, -1, 0);
    if (m_renderer == nullptr) {
        inform({"Failed to create render!\n", SDL_GetError()});
        exit(EXIT_FAILURE);
    }

    inform("Set Render Draw Blend Mode: Alpha blending!");
    if (SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND) != 0) {
        inform({"Failed to set render draw blend mode!\n", SDL_GetError()});
    }

    inform("Initialize SDL image to PNG and JPG!");
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) {
        inform({"Failed to initialize SDL Image!\n", IMG_GetError()});
    }

    inform("Initialize SDL TTF!");
    if (TTF_Init() < 0) {
        inform({"Failed to initialize SDL TTF!\n", TTF_GetError()});
    }

    inform("Initialize SDL Mixer to MP3 audio!");
    if (Mix_Init(MIX_INIT_MP3) == 0) {
        inform({"Failed to initialize SDL Mixer!\n", Mix_GetError()});
    }
    inform("Open the default audio device for playback!");
    if (Mix_OpenAudio(44100, MIX_INIT_MP3, 2, 2048) < 0) {
        inform({"Failed to open default audio device!\n", Mix_GetError()});
    }

    m_gamePerformance.ticks = SDL_GetTicks64();
}

void Engine::gameLoop()
{
    do {
        if (m_gameState == State::Game::stopped) {
            break;
        }

        auto current = SDL_GetTicks64();    // Milliseconds since SDL library initialization.
        m_gamePerformance.deltaTime = static_cast<double>(current - m_gamePerformance.ticks) / 1000.0;  // Seconds.
        m_gamePerformance.ticks = current;

        auto start = SDL_GetPerformanceCounter();   // Current value of the high resolution counter.

        // Render all game objects.
        renderScreen();
        // Check all user input.
        inputControl();
        // Update gameplay.
        updateGameplay();

        auto end = SDL_GetPerformanceCounter();
        auto cps = static_cast<double>(SDL_GetPerformanceFrequency());  // Count per second of the high resolution counter.
        auto fpsHR = static_cast<double>(end - start) / cps * 1000.0;

        auto fps = 1000 / 60; // 60 FPS
        Uint32 delay = fpsHR > fps ? 0 : fps - fpsHR;
        m_gamePerformance.framesPerSecond = 1000 / (fpsHR + delay);
        SDL_Delay(delay);

        if (m_gamePerformance.ticks % static_cast<long>(15 * 1000) == 0) {
            inform(stbox::Text::toStr("Ticks: ", m_gamePerformance.ticks, " FPS: ", m_gamePerformance.framesPerSecond,
                               " Delay: ", delay, " Delta time: ", m_gamePerformance.deltaTime, " seconds"));
        }
    }
    while (!m_externalControl);

}

void Engine::inputControl()
{
    // Listen to user input.
    const auto *keyState = SDL_GetKeyboardState(nullptr);
    while (SDL_PollEvent(&m_event) != 0) {
        // Window events.
        if (m_event.type == SDL_QUIT) {
            m_gameState = State::Game::stopped;
            inform("Clicked Close Window!");
        }
        // Keyboard Events.
        else if (m_event.type == SDL_KEYDOWN) {
            if (keyState[SDL_SCANCODE_ESCAPE]) {
                m_inputState = State::Input::keyboard_escape;
            }
            // Only Space
            if (keyState[SDL_SCANCODE_SPACE]) {
                m_inputState = State::Input::keyboard_space;
            }
            // 4 Directional Movement
            if (keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP]) {
                m_inputState = State::Input::keyboard_up;
            }
            if (keyState[SDL_SCANCODE_S] || keyState[SDL_SCANCODE_DOWN]) {
                m_inputState = State::Input::keyboard_down;
            }
            if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT]) {
                m_inputState = State::Input::keyboard_left;
            }
            if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]) {
                m_inputState = State::Input::keyboard_right;
            }
            // 8 Directional Movement
            if (keyState[SDL_SCANCODE_UP] && keyState[SDL_SCANCODE_RIGHT]) {
                m_inputState = State::Input::keyboard_up_rigth;
            }
            if (keyState[SDL_SCANCODE_DOWN] && keyState[SDL_SCANCODE_RIGHT]) {
                m_inputState = State::Input::keyboard_down_right;
            }
            if (keyState[SDL_SCANCODE_UP] && keyState[SDL_SCANCODE_LEFT]) {
                m_inputState = State::Input::keyboard_up_left;
            }
            if (keyState[SDL_SCANCODE_DOWN] && keyState[SDL_SCANCODE_LEFT]) {
                m_inputState = State::Input::keyboard_down_left;
            }
            if (keyState[SDL_SCANCODE_W] && keyState[SDL_SCANCODE_D]) {
                m_inputState = State::Input::keyboard_up_rigth;
            }
            if (keyState[SDL_SCANCODE_S] && keyState[SDL_SCANCODE_D]) {
                m_inputState = State::Input::keyboard_down_right;
            }
            if (keyState[SDL_SCANCODE_W] && keyState[SDL_SCANCODE_A]) {
                m_inputState = State::Input::keyboard_up_left;
            }
            if (keyState[SDL_SCANCODE_S] && keyState[SDL_SCANCODE_A]) {
                m_inputState = State::Input::keyboard_down_left;
            }
            // 4 Directional Movement and Space
            if (keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP] && keyState[SDL_SCANCODE_SPACE]) {
                m_inputState = State::Input::keyboard_up;
            }
            if (keyState[SDL_SCANCODE_S] || keyState[SDL_SCANCODE_DOWN] && keyState[SDL_SCANCODE_SPACE]) {
                m_inputState = State::Input::keyboard_down;
            }
            if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT] && keyState[SDL_SCANCODE_SPACE]) {
                m_inputState = State::Input::keyboard_left;
            }
            if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT] && keyState[SDL_SCANCODE_SPACE]) {
                m_inputState = State::Input::keyboard_right;
            }
            // 8 Directional Movement and Space
            if (keyState[SDL_SCANCODE_UP] && keyState[SDL_SCANCODE_RIGHT] && keyState[SDL_SCANCODE_SPACE]) {
                m_inputState = State::Input::keyboard_up_rigth_space;
            }
            if (keyState[SDL_SCANCODE_DOWN] && keyState[SDL_SCANCODE_RIGHT] && keyState[SDL_SCANCODE_SPACE]) {
                m_inputState = State::Input::keyboard_down_right_space;
            }
            if (keyState[SDL_SCANCODE_UP] && keyState[SDL_SCANCODE_LEFT] && keyState[SDL_SCANCODE_SPACE]) {
                m_inputState = State::Input::keyboard_up_left_space;
            }
            if (keyState[SDL_SCANCODE_DOWN] && keyState[SDL_SCANCODE_LEFT] && keyState[SDL_SCANCODE_SPACE]) {
                m_inputState = State::Input::keyboard_down_left_space;
            }
            if (keyState[SDL_SCANCODE_W] && keyState[SDL_SCANCODE_D] && keyState[SDL_SCANCODE_SPACE]) {
                m_inputState = State::Input::keyboard_up_rigth_space;
            }
            if (keyState[SDL_SCANCODE_S] && keyState[SDL_SCANCODE_D] && keyState[SDL_SCANCODE_SPACE]) {
                m_inputState = State::Input::keyboard_down_right_space;
            }
            if (keyState[SDL_SCANCODE_W] && keyState[SDL_SCANCODE_A] && keyState[SDL_SCANCODE_SPACE]) {
                m_inputState = State::Input::keyboard_up_left_space;
            }
            if (keyState[SDL_SCANCODE_S] && keyState[SDL_SCANCODE_A] && keyState[SDL_SCANCODE_SPACE]) {
                m_inputState = State::Input::keyboard_down_left_space;
            }
        }
        else if (m_event.type == SDL_KEYUP) {
            m_inputState = State::Input::none;
        }
        // Mouse Events.
        else if (m_event.type == SDL_MOUSEBUTTONDOWN) {
            switch (m_event.button.button) {
            case SDL_BUTTON_LEFT:
                m_inputState = State::Input::mouse_button_left;
                break;
            case SDL_BUTTON_RIGHT:
                m_inputState = State::Input::mouse_button_right;
                break;
            default:
                // Nothing to do.
                break;
            }
        }
        else if (m_event.type == SDL_MOUSEBUTTONUP) {
            m_inputState = State::Input::none;
        }
    }
}

void Engine::renderScreen()
{
    SDL_SetRenderDrawColor(m_renderer, m_bkgColor.r, m_bkgColor.g, m_bkgColor.b, m_bkgColor.a);
    SDL_RenderClear(m_renderer);
    render();   // Custom
    SDL_RenderPresent(m_renderer);
}

void Engine::updateGameplay()
{
    update();   // Custom
}

void Engine::finalize()
{
    inform("End Game!");
    m_gameState = State::Game::quit;

    // Free fonts.
    for (auto &font : m_fonts) {
        TTF_CloseFont(font.second);
        font.second = nullptr;
    }
    m_fonts.clear();
    inform("Free Text Fons.");

    // Free images.
    for (auto &texture : m_textures) {
        texture.second = nullptr;
    }
    m_textures.clear();
    inform("Free Textures");

    // Free custom objects.
    quit();
    inform("Free objects!");

    // Free audio.
    for (auto &audio : m_songs) {
        Mix_FreeMusic(audio.second);
        audio.second = nullptr;
    }
    for (auto &audio : m_sounds) {
        Mix_FreeChunk(audio.second);
        audio.second = nullptr;
    }
    m_songs.clear();
    m_sounds.clear();
    inform("Free Audios!");

    // Free renderer.
    SDL_DestroyRenderer(m_renderer);
    m_renderer = nullptr;
    inform("Destroy Renderer SDL!");

    // Free window.
    SDL_DestroyWindow(m_window);
    m_window = nullptr;
    inform("Destroy Window SDL!");

    // Exit.
    TTF_Quit();
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
    inform("Game finished!");
}

auto Engine::currentRenderer() -> SDL_Renderer *
{
    return m_renderer;
}

auto Engine::windowPosition() -> std::array<int, 2>
{
    int x{0};
    int y{0};
    SDL_GetWindowPosition(m_window, &x, &y);
    return {x, y};
}

auto Engine::gameTitle() -> std::string
{
    return m_title;
}

auto Engine::gameState() -> State::Game
{
    return m_gameState;
}

auto Engine::gameStateEqual(State::Game state) -> bool
{
    return m_gameState == state;
}

auto Engine::inputState() -> State::Input
{
    return m_inputState;
}

void Engine::inputClean()
{
    m_inputState = State::Input::none;
}

auto Engine::inputStateEqual(State::Input state) -> bool
{
    return m_inputState == state;
}

auto Engine::performanceReport() -> Performance
{
    return m_gamePerformance;
}

void Engine::inform(const std::string &message)
{
    SDL_Log("%s", message.c_str());
}

void Engine::inform(std::initializer_list<std::string> messages, bool multipleLines)
{
    if (!multipleLines) {
        inform(stbox::Text::toStr(messages));
        return;
    }
    for (const auto &message : messages) {
        inform(message);
    }
}

auto Engine::loadFont(const std::string &id, const std::string &filename) -> bool
{
    if (id.empty() || filename.empty()) {
        inform("Empty Font ID or font file path!");
        return false;
    }

    if (textFontExists(id)) {
        inform("Font ID: " + id + ", already exists!");
        return false;
    }

    int fontSize = 12;
    auto *font = TTF_OpenFont(filename.c_str(), fontSize);
    if (font == nullptr) {
        inform({"Font ID: " + id + ", failed to open TTF font!\n", TTF_GetError()}, true);
        return false;
    }

    m_fonts[id] = font;
    inform("Added: Font [" + id + ", " + std::to_string(fontSize) + " pt], " + filename);

    return true;
}

auto Engine::loadMusic(const std::string &id, const std::string &filename) -> bool
{
    if (id.empty() || filename.empty()) {
        inform("Empty Music ID or font file path!");
        return false;
    }

    if (musicExists(id)) {
        inform("Music ID: " + id + ", already exists!");
        return false;
    }

    auto *audio = Mix_LoadMUS(filename.c_str());
    if (audio == nullptr) {
        inform({"Music ID: " + id + ", failed to open file!\n", TTF_GetError()}, true);
        return false;
    }

    m_songs[id] = audio;
    inform("Added: Music [" + id + "], " + filename);

    return true;
}

auto Engine::loadSound(const std::string &id, const std::string &filename) -> bool
{
    if (id.empty() || filename.empty()) {
        inform("Empty Sound ID or font file path!");
        return false;
    }

    if (soundExists(id)) {
        inform("Sound ID: " + id + ", already exists!");
        return false;
    }

    auto *audio = Mix_LoadWAV(filename.c_str());
    if (audio == nullptr) {
        inform({"Sound ID: " + id + ", failed to open file!\n", TTF_GetError()}, true);
        return false;
    }

    m_sounds[id] = audio;
    inform("Added: Sound [" + id + "], " + filename);

    return true;
}

auto Engine::loadTexture(const std::string &id, int rows, int  columns, const std::string &filename) -> bool
{
    if (id.empty() || filename.empty()) {
        inform("Empty Image ID or image file path!");
        return false;
    }

    if (textureExists(id)) {
        inform("Texture ID: " + id + ", already exists!");
        return false;
    }

    auto *surface = IMG_Load(filename.c_str());
    if (surface == nullptr) {
        inform({"Texture ID: " + id + ", failed to open image [" + filename + "]!\n", IMG_GetError()}, true);
        return false;
    }

    auto *texture = SDL_CreateTextureFromSurface(currentRenderer(), surface);
    SDL_FreeSurface(surface);
    if (texture == nullptr) {
        inform({"Image ID: " + id + ", failed to create Texture [" + id + "]!\n", SDL_GetError()}, true);
        return false;
    }

    auto height = surface->h;
    auto width = surface->w;

    m_textures[id] = texture;
    inform(stbox::Text::toStr("Added Texture: ", id, ", Rows [", rows, "], Columns [", columns,
                       "], Texture [", width, " x ", height, "], ", filename, "]"));

    return true;
}

auto Engine::loadTexture(const std::string &id, const std::string &filename) -> bool
{
    return loadTexture(id, 0, 0, filename);
}

auto Engine::renderTextFont(const std::string &fontId, const std::string &text, const SDL_FPoint &position,
                            const SDL_Color &foreground, int fontSize, double angle, float scale, Uint8 opacity, SDL_RendererFlip flip) -> bool
{
    fontSize = fontSize < 0 ? 6 : fontSize;
    fontSize = fontSize > windowHeight() ? windowHeight() : fontSize;

    auto *font = textFont(fontId);
    if (font == nullptr) {
        return false;
    }

    TTF_SetFontSize(font, fontSize);
    SDL_Color color = {foreground.r, foreground.g, foreground.b, foreground.a};
    auto *fontSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (fontSurface != nullptr) {
        auto *textTexture = SDL_CreateTextureFromSurface(currentRenderer(), fontSurface);
        return renderTexture(textTexture, {0, 0, 0, 0}, {position.x, position.y, 0, 0}, angle, scale, opacity, flip);
    }

    return false;
}

auto Engine::renderTextFont(const std::string &fontId, const std::string &text, const SDL_FPoint &position,
                            const SDL_Color &foreground, int fontSize, double angle, float scale, Uint8 opacity) -> bool
{
    return renderTextFont(fontId, text, position, foreground, fontSize, angle, scale, opacity, SDL_FLIP_NONE);
}

auto Engine::renderTextFont(const std::string &fontId, const std::string &text, const SDL_FPoint &position,
                            const SDL_Color &foreground, int fontSize) -> bool
{
    return renderTextFont(fontId, text, position, foreground, fontSize, 0, 1, 255, SDL_FLIP_NONE);
}

auto Engine::renderTexture(SDL_Texture *texture, SDL_Rect clip, SDL_FRect box, double angle, float scale,
                           Uint8 opacity, SDL_RendererFlip flip) -> bool
{
    if (texture == nullptr) {
        return false;
    }

    scale = scale <= 0 ? 1 : scale;

    int height{0};
    int width{0};
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

    if (SDL_RectEmpty(&clip)) {
        clip = {0, 0, width, height};
    }
    if (clip.x < 0 || clip.x > width || clip.w < 0 || clip.w > width) {
        return false;
    }
    if (clip.y < 0 || clip.y > height || clip.h < 0 || clip.h > height) {
        return false;
    }

    box.w = box.w < 1 ? static_cast<float>(width) : box.w;
    box.h = box.h < 1 ? static_cast<float>(height) : box.h;

    SDL_FRect dest{box.x, box.y, static_cast<float>(clip.w) *scale, static_cast<float>(clip.h) *scale};
    if (box.w != dest.w) {
        dest.x = box.x + (box.w - dest.w) / 2;
    }
    if (box.h != dest.h) {
        dest.y = box.y + (box.h - dest.h) / 2;
    }

    opacity = opacity % 256;    // 255 = 100%
    SDL_SetTextureAlphaMod(texture, opacity);

    SDL_FPoint center{dest.w / 2, dest.h / 2};
    SDL_Rect src{clip.x, clip.y, clip.w, clip.h};
    SDL_RenderCopyExF(currentRenderer(), texture, &src, &dest, angle, &center, flip);

    return true;
}

auto Engine::renderTexture(const std::string &id, SDL_Rect clip, SDL_FRect box, double angle, float scale,
                           Uint8 opacity, SDL_RendererFlip flip) -> bool
{
    auto *currentTexture = texture(id);
    return renderTexture(currentTexture, clip, box, angle, scale, opacity, flip);
}

auto Engine::renderTexture(const std::string &id, float x, float y, double angle, float scale, Uint8 opacity) -> bool
{
    return renderTexture(id, {0, 0, 0, 0}, {x, y, 0, 0}, angle, scale, opacity, SDL_FLIP_NONE);
}

auto Engine::renderTexture(const std::string &id, float x, float y) -> bool
{
    return renderTexture(id, {0, 0, 0, 0}, {x, y, 0, 0}, 0, 1, 255, SDL_FLIP_NONE);
}

auto Engine::renderTexture(const std::string &id) -> bool
{
    return renderTexture(id, {0, 0, 0, 0}, {0, 0, 0, 0}, 0, 1, 255, SDL_FLIP_NONE);
}

auto Engine::textFont(const std::string &id) -> TTF_Font *
{
    if (!textFontExists(id)) {
        return nullptr;
    }

    return m_fonts[id];
}

auto Engine::textFontExists(const std::string &id) -> bool
{
    return m_fonts.contains(id);
}

auto Engine::texture(const std::string &id) -> SDL_Texture *
{
    if (!textureExists(id)) {
        return nullptr;
    }

    return m_textures[id];
}

auto Engine::textureExists(const std::string &id) -> bool
{
    return m_textures.contains(id);
}

auto Engine::textureSize(const std::string &id) -> std::array<int, 2>
{
    if (!textureExists(id)) {
        return {0, 0};
    }

    int height{0};
    int width{0};
    auto *texture = m_textures[id];
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

    return {width, height};
}

auto Engine::playSound(const std::string &id) -> bool
{
    if (soundExists(id)) {
        Mix_PlayChannel(-1, m_sounds[id], 0);
        return true;
    }

    return false;
}

auto Engine::playMusic(const std::string &id) -> bool
{
    if (musicExists(id)) {
        if (Mix_PlayingMusic() == 0) {
            Mix_PlayMusic(m_songs[id], -1);
            return true;
        }
        inform("Stop the music before playing another one!");
        return false;
    }

    return false;
}

void Engine::pauseMusic()
{
    if (Mix_PlayingMusic() == 1) {
        Mix_PauseMusic();
    };
}

void Engine::resumeMusic()
{
    if (Mix_PausedMusic() == 1) {
        Mix_ResumeMusic();
    };
}

void Engine::stopMusic()
{
    Mix_HaltMusic();
}

auto Engine::musicExists(const std::string &id) -> bool
{
    return m_songs.contains(id);
}

auto Engine::soundExists(const std::string &id) -> bool
{
    return m_sounds.contains(id);
}


void Engine::setGameState(State::Game state)
{
    m_gameState = state;
}

void Engine::setBackgroundColor(SDL_Color color)
{
    m_bkgColor = color;
}

// Attention! Once changed there is no way to change the control!
void Engine::setAsExternalGameloopControl()
{
    if (m_gameState == State::Game::none) {
        m_externalControl = true;
        inform("External control activated!");
    }

    if (m_externalControl) {
        inform("Control configured as external!");
    }
}
