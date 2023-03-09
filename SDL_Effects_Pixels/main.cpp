/*
 * Drawing created by reading a Char Vector containing the basic colors (RGB).
 * The Rect function is used to visualize the pixels.
 * Small manipulations of size, movement and key-triggered effects have been implemented.
 */

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

struct Pixel
{
    Point point;
    char color;
    int size;
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

std::vector<Pixel> normal(Element element);
std::vector<Pixel> effect_random(Element element);
std::vector<Pixel> effect_distance(Element element);

int view(unsigned int screen_width, unsigned int screen_height);
void draw(SDL_Renderer *pRenderer, std::vector<Pixel> pixels);
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
    int limitX, limitY;

    // Events
    SDL_Event event;
    bool running = true;
    bool testing = false;
    int number = 1;

    // Loop
    while (running) {

        // Render
        SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
        SDL_RenderClear(pRenderer);

        limitX = screen_width - speed - element.pixel * size;
        limitY = screen_height - speed - element.pixel * size + 1;

        if (!testing) {
            element.position = {x, y};
            draw(pRenderer, normal(element));
        }
        if (testing && number == 1) {
            draw(pRenderer, effect_random(element));
            SDL_Delay(50);
        }
        if (testing && number == 2) {
            draw(pRenderer, effect_distance(element));
            SDL_Delay(10);
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
                case SDLK_UP:   // Up arrow key
                    y -= speed * (y > speed && !testing);
                    if (element.direction != UP) {
                        element.colors = base;
                        element.direction = UP;
                    }
                    break;
                case SDLK_DOWN: // Down arrow key
                    y += speed * (y < limitY && !testing);
                    if (element.direction != DOWN) {
                        element.colors = base;
                        element.change(FLIP);
                        element.direction = DOWN;
                    }
                    break;
                case SDLK_LEFT: // Left arrow key
                    x -= speed * (x > speed && !testing);
                    if (element.direction != LEFT) {
                        element.colors = base;
                        element.change(REVERSE);
                        element.direction = LEFT;
                    }
                    break;
                case SDLK_RIGHT: // Right arrow key
                    x += speed * (x < limitX && !testing);
                    if (element.direction != RIGHT) {
                        element.colors = base;
                        element.change(REVERSE);
                        element.change(MIRROR);
                        element.direction = RIGHT;
                    }
                    break;
                case SDLK_KP_MINUS:     // - key
                    element.pixel = element.pixel > 4 ? element.pixel / 2 : element.pixel;
                    break;
                case SDLK_KP_PLUS:      // + Key
                    element.pixel = element.pixel < 32 ? element.pixel * 2 : element.pixel;
                    break;
                case SDLK_SPACE:        // Space Bar key
                    testing = false;    // Stop effect
                    break;
                case SDLK_1:            // Number Key
                    testing = true;
                    number = 1;
                    break;
                case SDLK_2:            // Number Key
                    testing = true;
                    number = 2;
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

void draw(SDL_Renderer *pRenderer, std::vector<Pixel> pixels)
{
    for (auto& px : pixels) {
        SDL_Rect rect = {px.point.X, px.point.Y, px.size, px.size};
        SDL_SetRenderDrawColor(pRenderer,
                               px.color == 'R' ? 255 : 0,
                               px.color == 'G' ? 255 : 0,
                               px.color == 'B' ? 255 : 0,
                               255);
        SDL_RenderFillRect(pRenderer, &rect);
    }
}

std::vector<Pixel> effect_random(Element element)
{
    std::vector<Pixel> pixels{};
    int size = sqrt(element.colors.size());
    int factor = 50;

    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            int x = element.position.X + col * element.pixel + (random() % factor) - (random() % factor);
            int y = element.position.Y + row * element.pixel + (random() % factor) - (random() % factor);
            pixels.push_back(Pixel{Point{x, y}, element.getColor(row, col), element.pixel});
        }
    }

    return pixels;
}

std::vector<Pixel> effect_distance(Element element)
{
    std::vector<Pixel> pixels{};
    int size = sqrt(element.colors.size());
    int dist = 20 * element.pixel;
    int middle = size / 2;

    static int disp = element.pixel;
    if (disp++ > dist) {
        disp = element.pixel;
    }

    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            int x = element.position.X + col * element.pixel + (disp * (col > middle)) - (disp * (col < middle));
            int y = element.position.Y + row * element.pixel + (disp * (row > middle)) - (disp * (row < middle));
            pixels.push_back(Pixel{Point{x, y}, element.getColor(row, col), element.pixel});
        }
    }

    return pixels;
}

std::vector<Pixel> normal(Element element)
{
    std::vector<Pixel> pixels{};
    int size = sqrt(element.colors.size());

    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            int x = element.position.X + col * element.pixel;
            int y = element.position.Y + row * element.pixel;
            pixels.push_back(Pixel{Point{x, y}, element.getColor(row, col), element.pixel});
        }
    }

    return pixels;
}

std::vector<char> getDrawing(int index)
{
    // Square Shape
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
