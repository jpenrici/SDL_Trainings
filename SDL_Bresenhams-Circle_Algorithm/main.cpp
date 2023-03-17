/*
 * Reference: Bresenham's Circle algorithm
 * https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm
 *
 */
#include <SDL2/SDL.h>

#include <iostream>
#include <math.h>
#include <vector>

struct Color {
    unsigned char R = 255;
    unsigned char G = 255;
    unsigned char B = 255;
    unsigned char A = 255;

    Color(){};
    Color(unsigned char R, unsigned char G, unsigned char B) : R(R), G(G), B(B) {}
};

struct Circle {
    SDL_Point center;
    int radius;

    Color color;

    Circle(){};
    Circle(int x, int y, int radius) : center(SDL_Point{x, y}), radius(radius) {}
    Circle(int x, int y, int radius, Color color)
        : center(SDL_Point{x, y}), radius(radius), color(color) {}
};

std::vector<SDL_Point> bresenhamCircle(Circle circle)
{
    std::vector<SDL_Point> points;

    auto putPixels = [&points] (int xC, int yC, int x, int y) {
        points.push_back(SDL_Point{xC + x, yC + y});
        points.push_back(SDL_Point{xC - x, yC + y});
        points.push_back(SDL_Point{xC + x, yC - y});
        points.push_back(SDL_Point{xC - x, yC - y});
        points.push_back(SDL_Point{xC + y, yC + x});
        points.push_back(SDL_Point{xC - y, yC + x});
        points.push_back(SDL_Point{xC + y, yC - x});
        points.push_back(SDL_Point{xC - y, yC - x});
    };

    int x = 0;
    int y = circle.radius;
    int d = 3 - 2 * circle.radius;

    putPixels(circle.center.x, circle.center.y, x, y);
    while(x <= y) {
        x++;
        if (d < 0) {
            d = d + 4 * x + 6;
        } else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        putPixels(circle.center.x, circle.center.y, x, y);
    }

    return points;
}

int view(std::vector<Circle> circles)
{
    int screen_width = 800;
    int screen_height = 600;

    SDL_Window *pWindow = 0;
    SDL_Renderer *pRenderer = 0;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "Failed to initialize SDL!\n";
        exit(EXIT_FAILURE);
    }

    pWindow = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               screen_width, screen_height, SDL_WINDOW_SHOWN);
    if (pWindow == NULL) {
        std::cout << "Failed to create window!\n";
        exit(EXIT_FAILURE);
    }

    pRenderer =  SDL_CreateRenderer(pWindow, -1, 0);
    if (pRenderer == NULL) {
        std::cout << "Failed to create render!\n";
        exit(EXIT_FAILURE);
    }

    bool running = true;
    while (running) {

        // Events
        SDL_Event event;

        // Process Input
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            }
        }

        // Render
        SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
        SDL_RenderClear(pRenderer);

        for (auto &circle : circles) {
            SDL_SetRenderDrawColor(pRenderer,
                                   circle.color.R,
                                   circle.color.G,
                                   circle.color.B,
                                   circle.color.A);
            auto points = bresenhamCircle(circle);
            for (auto &point : points) {
                SDL_RenderDrawPoints(pRenderer, &point, 1);
            }
        }

        SDL_RenderPresent(pRenderer);
    }

    // Terminate
    SDL_DestroyRenderer(pRenderer);
    pRenderer = NULL;

    SDL_DestroyWindow(pWindow);
    pWindow = NULL;

    SDL_Quit();

    return 0;
}

int main()
{
    view(std::vector<Circle> {
             Circle(150, 300,  50, Color(255, 0, 0)),
             Circle(100, 300, 100, Color(0, 255, 0)),
             Circle(300, 300, 200, Color(0, 0, 255)),
             Circle(400, 300, 250),
         });

    return 0;
}
