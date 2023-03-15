/*
 * Reference: Bresenham's line algorithm
 * https://en.wikipedia.org/wiki/Bresenham's_line_algorithm
 *
 */
#include <SDL2/SDL.h>

#include <iostream>
#include <math.h>
#include <vector>

struct Line {
    SDL_FPoint p1, p2;
    Line(float x1, float y1, float x2, float y2)
        : p1(SDL_FPoint{x1, y1}), p2(SDL_FPoint{x2, y2}) {}
};

std::vector<SDL_FPoint> bresenhamLine(Line line)
{
    std::vector<SDL_FPoint> points;

    SDL_FPoint p1 = line.p1;
    SDL_FPoint p2 = line.p2;

    float dx = abs(p2.x - p1.x);
    float sx = p1.x < p2.x ? 1 : -1;

    float dy = -abs(p2.y - p1.y);
    float sy = p1.y < p2.y ? 1 : - 1;

    float error = dx + dy;

    while (true) {
        points.push_back(SDL_FPoint{p1.x, p1.y});
        if (p1.x == p2.x && p1.y == p2.y) {
            break;
        }
        float e2 = 2 * error;
        if (e2 >= dy) {
            if (p1.x == p2.x) {
                break;
            }
            error += dy;
            p1.x += sx;
        }
        if (e2 <= dx) {
            if (p1.y == p2.y) {
                break;
            }
            error += dx;
            p1.y += sy;
        }
    }

    return points;
}

int view(std::vector<Line> lines)
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

        SDL_SetRenderDrawColor(pRenderer, 0, 255, 0, 255);
        for (auto &line : lines) {
            auto points = bresenhamLine(line);
            for (auto &point : points) {
                SDL_RenderDrawPointsF(pRenderer, &point, 1);
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
    float x = 200;
    float y = 200;
    float w = 200;
    float h = 200;

    view(std::vector<Line> {
             // rectangle
             Line(x, y, x + w, y),
             Line(x + w, y, x + w, y + h),
             Line(x + w, y + h, x, y + h),
             Line(x, y + h, x, y),
             // diagonal
             Line(x, y, x + w, y + h),
             Line(x, y + h, x + w, y),
             // triangle
             Line(0, 0, w / 2, h / 2),
             Line(w / 2, h / 2, 0, h / 2),
             Line(0, h / 2, 0, 0)
         });

    return 0;
}
