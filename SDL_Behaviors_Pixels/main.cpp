/*
 * Movements controlled by functions.
 */

#include <SDL2/SDL.h>

#include <algorithm>
#include <iostream>
#include <vector>
#include <math.h>

struct Point {
    int X = 0;
    int Y = 0;
};

struct Color {
    unsigned int R = 255;
    unsigned int G = 255;
    unsigned int B = 255;
    unsigned int A = 255;
};

struct Pixel {
    Point point;
    Color color;
    int size = 4;
};

int view(unsigned int screen_width, unsigned int screen_height);
void draw(SDL_Renderer *pRenderer, Pixel px);

// Test
int main()
{
    view(500, 500);

    return 0;
}

// Functions
int view(unsigned int screen_width, unsigned int screen_height)
{
    // Initialize
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "Failed to initialize SDL!\n";
        return EXIT_FAILURE;
    }

    SDL_Window *pWindow = 0;
    pWindow = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               screen_width, screen_height, SDL_WINDOW_SHOWN);
    if (pWindow == NULL) {
        std::cout << "Failed to create window!\n";
        return EXIT_FAILURE;
    }

    SDL_Renderer *pRenderer = 0;
    pRenderer =  SDL_CreateRenderer(pWindow, -1, 0);
    if (pRenderer == NULL) {
        std::cout << "Failed to create render!\n";
        return EXIT_FAILURE;
    }

    // Display
    int factor = 16;
    Point speed {factor, factor};
    Point origin {static_cast<int>(screen_width / 2),
                static_cast<int>(screen_height / 2)};
    Point limits {static_cast<int>(screen_width - 2 * speed.X),
                static_cast<int>(screen_height - 2 * speed.Y)};

    // Pixel
    Pixel px;
    px.size = factor;
    px.point = origin;
    int angle = 0;

    // Events
    SDL_Event event;
    bool running = true;
    bool testing = false;
    int number = 0;

    // Behaviors
    auto horizontal = [&px, &limits, &speed]() {
        px.point.X += speed.X;
        if (px.point.X < px.size || px.point.X > limits.X) {
            speed.X *= -1;
        }
    };

    auto vertical = [&px, &limits, &speed]() {
        px.point.Y += speed.Y;
        if (px.point.Y < px.size || px.point.Y > limits.Y) {
            speed.Y *= -1;
        }
    };

    auto circle = [&px, &angle](Point center, int radius, int step) {
        float a = angle * 3.1415 / 180;
        px.point.X = center.X + radius * cos(a);
        px.point.Y = center.Y + radius * sin(a);
        angle += step;
        angle %= 360;
    };

    // Loop
    while (running) {

        // Render
        SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
        SDL_RenderClear(pRenderer);
        SDL_Delay(20);

        if (testing && number == 1) {
            horizontal();
        }
        if (testing && number == 2) {
            vertical();
        }
        if (testing && number == 3) {
            vertical();
            horizontal();
        }
        if (testing && number == 4) {
            vertical();
            horizontal();
            vertical();
        }
        if (testing && number == 5) {
            horizontal();
            vertical();
            horizontal();
        }
        if (testing && number == 6) {
            circle(origin, 100, 2);
        }

        draw(pRenderer, px);
        SDL_RenderPresent(pRenderer);

        // Process Input
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                running = false;
                break;
            }
            else if (event.type == SDL_KEYDOWN) {
                auto k = event.key.keysym.sym;
                switch (k) {
                case SDLK_0: // value = 48
                case SDLK_1:
                case SDLK_2:
                case SDLK_3:
                case SDLK_4:
                case SDLK_5:
                case SDLK_6: // value = 54
                    px.point = origin;
                    testing = true;
                    number = k - SDLK_0;
                    break;
                case SDLK_SPACE:        // Space Bar key
                    testing = false;    // Stop effect
                    break;
                case SDLK_ESCAPE:       // Esc key
                    running = false;
                    break;
                default:
                    break;
                }
            }
        }
    }

    // Terminate
    SDL_DestroyRenderer(pRenderer);
    pRenderer = NULL;

    SDL_DestroyWindow(pWindow);
    pWindow = NULL;

    SDL_Quit();

    return EXIT_SUCCESS;
}

void draw(SDL_Renderer *pRenderer, Pixel px)
{
    SDL_Rect rect = {px.point.X, px.point.Y, px.size, px.size};
    SDL_SetRenderDrawColor(pRenderer, px.color.R, px.color.G, px.color.B, px.color.A);
    SDL_RenderFillRect(pRenderer, &rect);
}
