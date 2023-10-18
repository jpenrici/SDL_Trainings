#include "example.h"
#include "../engine/smallToolBox.h"

using stbox::Math;

Example::Example(): Engine(640, 480, "Example")
{
    // Background
    setBackgroundColor({0, 90, 60, 255});

    // Load and configure resources.
    loadFont("fontEx", "resources/NotoSans-Black.ttf");

    // Load Image
    loadTexture("image", "resources/gameOver.png");

    // Config player.
    m_dummy = {windowWidth<float>() / 2, windowHeight<float>() / 2, 15, 15};
}

void Example::render()
{
    auto blue  = SDL_Color{0, 0, 255};
    auto green = SDL_Color{0, 255, 0};
    //auto red   = SDL_Color{255, 0, 0};
    auto white = SDL_Color{255, 255, 255};

    // Random lines.
    SDL_Rect rectangle{Math::randomize(windowWidth()), Math::randomize(windowHeight()), 2, Math::randomize(windowHeight())};
    SDL_SetRenderDrawColor(currentRenderer(), Math::randomize(255), Math::randomize(255), Math::randomize(255), 255);
    SDL_RenderFillRect(currentRenderer(), &rectangle);

    // Image
    renderTexture("image", 150, 50, 0, 1);

    // Dummy player.
    SDL_SetRenderDrawColor(currentRenderer(), 255, 150, 0, 255);
    SDL_RenderFillRectF(currentRenderer(), &m_dummy);

    // Texts.
    auto strDt    = "dT : " + std::to_string(performanceReport().deltaTime);
    auto strFps   = "fps : " + std::to_string(performanceReport().framesPerSecond);
    auto strTicks = "ticks : " + std::to_string(performanceReport().ticks);
    auto dummyPos = std::to_string(static_cast<int>(m_dummy.x)) + "," + std::to_string(static_cast<int>(m_dummy.y));
    renderTextFont("fontEx", gameTitle(), {windowWidth<float>() - 100, 10}, blue, 20, 0, 1);
    renderTextFont("fontEx", strDt, {5, 10}, green, 14, 0, 1);
    renderTextFont("fontEx", strFps, {5, 25}, green, 14, 0, 1);
    renderTextFont("fontEx", strTicks, {5, 40}, green, 14, 0, 1);
    renderTextFont("fontEx", dummyPos, {m_dummy.x + m_dummy.w + 2, m_dummy.y + m_dummy.h + 2}, green, 13, 0, 1);
    renderTextFont("fontEx", "Use WASD or Arrows to move.",  {5, windowHeight<float>() - 20}, white, 14, 0, 1);
    renderTextFont("fontEx", "Simple DirectMedia Layer", {130, 70}, white, 24, 344, 1);
}

void Example::update()
{
    // Events.
    switch (inputState()) {
    case State::Input::keyboard_up:
        m_dummy.y -= m_dummy.y < m_dummy.h ? 0 : m_dummy.h;
        break;
    case State::Input::keyboard_down:
        m_dummy.y += (m_dummy.y + m_dummy.h) < windowHeight<float>() ? m_dummy.h : 0;
        break;
    case State::Input::keyboard_left:
        m_dummy.x -= m_dummy.x < m_dummy.w ? 0 : m_dummy.w;
        break;
    case State::Input::keyboard_right:
        m_dummy.x += (m_dummy.x + 2 * m_dummy.w) < windowWidth<float>() ? m_dummy.w : 0;
        break;
    case State::Input::keyboard_up_rigth:
        m_dummy.x += (m_dummy.x + 2 * m_dummy.w) < windowWidth<float>() ? m_dummy.w : 0;
        m_dummy.y -= m_dummy.y < m_dummy.h ? 0 : m_dummy.h;
        break;
    case State::Input::keyboard_down_right:
        m_dummy.x += (m_dummy.x + 2 * m_dummy.w) < windowWidth<float>() ? m_dummy.w : 0;
        m_dummy.y += (m_dummy.y + m_dummy.h) < windowHeight<float>() ? m_dummy.h : 0;
        break;
    case State::Input::keyboard_up_left:
        m_dummy.x -= m_dummy.x < m_dummy.w ? 0 : m_dummy.w;
        m_dummy.y -= m_dummy.y < m_dummy.h ? 0 : m_dummy.h;
        break;
    case State::Input::keyboard_down_left:
        m_dummy.x -= m_dummy.x < m_dummy.w ? 0 : m_dummy.w;
        m_dummy.y += (m_dummy.y + m_dummy.h) < windowHeight<float>() ? m_dummy.h : 0;
        break;
    default:
        break;
    }
}
