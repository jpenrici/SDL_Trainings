#include <SDL2/SDL.h>

#include <algorithm>
#include <iostream>
#include <vector>

enum Direction {UP, DOWN, LEFT, RIGHT};

struct Point {
    int X, Y;
};

struct Element {
    Point point;
    Direction direction;
    int pixelWidth, pixelHeight;
    std::vector<std::string> pixelStr;
};

Point normal(int x, int y);
Point effect_random(int x, int y);

int view(unsigned int screen_width, unsigned int screen_height);
void draw(SDL_Renderer *pRenderer, Element element, Point(*func)(int, int));
std::vector<std::string> getDrawing(unsigned int index = 0);

int main()
{
    view(500, 500);

    return 0;
}

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

    Element element;
    element.point = {0, 0};
    element.pixelWidth = 8;
    element.pixelHeight = 8;
    element.direction = UP;
    element.pixelStr = getDrawing();

    int x = screen_width / 2;
    int y = screen_height / 2;
    int speed = std::max(element.pixelWidth, element.pixelHeight);
    int limitX = screen_width - speed - element.pixelWidth * element.pixelStr.front().size();
    int limitY = screen_height - speed - element.pixelHeight * element.pixelStr.size() + 1;

    // Events
    SDL_Event event;
    bool running = true;
    bool testing = false;
    int count = 0;

    // Loop
    while (running) {

        // Render
        SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
        SDL_RenderClear(pRenderer);

        if (!testing) {
            element.point = {x, y};
            draw(pRenderer, element, &normal);
        } else {
            draw(pRenderer, element, &effect_random);
            SDL_Delay(50);
            if (count++ > 3) {
                testing = false;
                count = 0;
            }
        }

        SDL_RenderPresent(pRenderer);

        // Process Input
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                running = false;
                break;
            }
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_UP:
                    y -= speed * (y > speed && !testing);
                    if (element.direction != UP) {
                        element.pixelStr = getDrawing();
                        element.direction = UP;
                    }
                    break;
                case SDLK_DOWN:

                    y += speed * (y < limitY && !testing);
                    if (element.direction != DOWN) {
                        std::reverse(element.pixelStr.begin(), element.pixelStr.end());
                        element.direction = DOWN;
                    }
                    break;
                case SDLK_LEFT:
                    x -= speed * (x > speed && !testing);
                    if (element.direction != LEFT) {
                        for(auto& e : element.pixelStr) {
                            std::reverse(e.begin(), e.end());
                        }
                        element.direction = LEFT;
                    }
                    break;
                case SDLK_RIGHT:
                    x += speed * (x < limitX && !testing);
                    if (element.direction != RIGHT) {
                        for(auto& e : element.pixelStr) {
                            std::reverse(e.begin(), e.end());
                        }
                        element.direction = RIGHT;
                    }
                    break;
                case SDLK_SPACE:
                    testing = true;
                    break;
                case SDLK_ESCAPE:
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

void draw(SDL_Renderer *pRenderer, Element element, Point (*func)(int, int))
{
    for (int i = 0; i < element.pixelStr.size(); ++i) {
        for (int j = 0; j < element.pixelStr[i].size(); ++j) {
            auto p = func(element.point.X + j * element.pixelWidth,
                          element.point.Y + i * element.pixelHeight);
            SDL_Rect rect = {p.X, p.Y, element.pixelWidth, element.pixelHeight};
            SDL_SetRenderDrawColor(pRenderer,
                                   element.pixelStr[i][j] == 'R' ? 255 : 0,
                                   element.pixelStr[i][j] == 'G' ? 255 : 0,
                                   element.pixelStr[i][j] == 'B' ? 255 : 0,
                                   255);
            SDL_RenderFillRect(pRenderer, &rect);
        }
    }
}

Point effect_random(int x, int y)
{
    int factor = 50;
    x += (random() % factor) - (random() % factor);
    y += (random() % factor) - (random() % factor);
    return Point{x, y};
}

Point normal(int x, int y)
{
    return Point{x, y};
}

std::vector<std::string> getDrawing(unsigned int index)
{
    std::vector<std::vector<std::string>> v {
        {
            "RRRRRRRR",
            "RGGGGGGR",
            "BGGGGGGR",
            "BGGGGGGR",
            "GBBBBBBR",
            "GBBBBBBR",
            "RBBBBBBR",
            "RRRRRRRR"
        },
    };

    if (index < 0 || index >= v.size()) {
        return v[0];
    }

    return v[index];
}
