#include <SDL2/SDL.h>

#include <algorithm>
#include <iostream>
#include <vector>

enum Direction {UP, DOWN, LEFT, RIGHT};
enum Action    {NONE, FLIP, MIRROR, REVERSE};

struct Point {
    int X = 0;
    int Y = 0;
};

struct Element {
    Point position;
    Direction direction = UP;

    int pixel = 4;  // size
    std::vector<char> colors{};

    void setColor(unsigned int row, unsigned int col, char color)
    {
        if (!colors.empty()) {
            int size = sqrt(colors.size());
            if (row >= 0 && row < size && col >= 0 && col < size) {
                colors[row * size + col] = color;
            }
        }
    }

    char getColor(unsigned int row, unsigned int col)
    {
        if (!colors.empty()) {
            int size = sqrt(colors.size());
            if (row >= 0 && row < size && col >= 0 && col < size) {
                return colors[row * size + col];
            }
        }
        return 0;
    }

    void change(Action action = NONE)
    {
        if (!colors.empty() && action != NONE) {
            int size = sqrt(colors.size());
            std::vector<char> temp(size * size, 0);
            for (int row = 0; row < size; row++) {
                for (int col = 0; col < size; col++) {
                    if (action == FLIP) {
                        temp[row * size + col] = getColor(size - row - 1, col);
                    }
                    if (action == MIRROR) {
                        temp[row * size + col] = getColor(row, size - col - 1);
                    }
                    if (action == REVERSE) {
                        temp[row * size + col] = getColor(col, row);
                    }
                }
            }
            colors = temp;
        }
    }
};

Point normal(int x, int y);
Point effect_random(int x, int y);

int view(unsigned int screen_width, unsigned int screen_height);
void draw(SDL_Renderer *pRenderer, Element element, Point(*func)(int, int));
std::vector<char> getDrawing(int index = 0);

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

    // Element
    std::vector<char> base = getDrawing(3);
    int size = sqrt(base.size());

    Element element;
    element.pixel = 8;
    element.colors = base;
    int speed = 2 * element.pixel;

    // Display
    int x = screen_width / 2;
    int y = screen_height / 2;
    int limitX = screen_width - speed - element.pixel * size;
    int limitY = screen_height - speed - element.pixel * size + 1;

    // Events
    SDL_Event event;
    bool running = true;
    bool testing = false;
    int  count = 0;

    // Loop
    while (running) {

        // Render
        SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
        SDL_RenderClear(pRenderer);

        if (!testing) {
            element.position = {x, y};
            draw(pRenderer, element, &normal);
        }
        else {
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
                        element.colors = base;
                        element.direction = UP;
                    }
                    break;
                case SDLK_DOWN:
                    y += speed * (y < limitY && !testing);
                    if (element.direction != DOWN) {
                        element.colors = base;
                        element.change(FLIP);
                        element.direction = DOWN;
                    }
                    break;
                case SDLK_LEFT:
                    x -= speed * (x > speed && !testing);
                    if (element.direction != LEFT) {
                        element.colors = base;
                        element.change(REVERSE);
                        element.direction = LEFT;
                    }
                    break;
                case SDLK_RIGHT:
                    x += speed * (x < limitX && !testing);
                    if (element.direction != RIGHT) {
                        element.colors = base;
                        element.change(REVERSE);
                        element.change(MIRROR);
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

void draw(SDL_Renderer *pRenderer, Element element, Point(*func)(int, int))
{
    int size = sqrt(element.colors.size());
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            auto p = func(element.position.X + j * element.pixel,
                          element.position.Y + i * element.pixel);
            SDL_Rect rect = {p.X, p.Y, element.pixel, element.pixel};
            SDL_SetRenderDrawColor(pRenderer,
                                   element.getColor(i, j) == 'R' ? 255 : 0,
                                   element.getColor(i, j) == 'G' ? 255 : 0,
                                   element.getColor(i, j) == 'B' ? 255 : 0,
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

std::vector<char> getDrawing(int index)
{
    // Sq
    std::vector<std::string> vec {
        // 1 x 1
        {
            "R"
        },
        // 2 x 2
        {
            "RG"
            "GB"
        },
        // 4 x 4
        {
            "RRGG"
            "RRGG"
            "GGBB"
            "GGBB"
        },
        // 9 x 9
        {
            "----R----"
            "---RRR---"
            "--RRRRR--"
            "-R-GBG-R-"
            "R--GBG--R"
            "B--BGB--G"
            "---GBG---"
            "---GBG---"
            "---GBG---"
        },
    };

    if (index < 0 || index >= vec.size()) {
        index = 0;
    }

    return std::vector<char>(vec[index].begin(), vec[index].end());
}
