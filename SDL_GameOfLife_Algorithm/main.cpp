/*
 * Reference: Conway's Game of Life
 * https://en.wikipedia.org/wiki/Conway's_Game_of_Life
 * https://pt.wikipedia.org/wiki/Jogo_da_vida
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

    Color() {};
    Color(unsigned char R, unsigned char G, unsigned char B) : R(R), G(G), B(B) {}

    bool equal(Color color)
    {
        return (R == color.R && G == color.G && B == color.B && A == color.A);
    }
};

typedef std::vector<std::vector<Color> > vec2D;

int gameOfLife()
{
    int screen_width = 800;
    int screen_height = 600;

    std::vector<std::string> title {
        "Game Of Life - Drawing [ Press F12 to Running or Delete to restart! ]",
        "Game Of Life - Running [ Press F11 to Drawing or Delete to restart! ]"
    };

    SDL_Window *pWindow = 0;
    SDL_Renderer *pRenderer = 0;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "Failed to initialize SDL!\n";
        exit(EXIT_FAILURE);
    }

    pWindow = SDL_CreateWindow(title[0].c_str(),
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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

    SDL_RenderClear(pRenderer);

    // Drawing Area
    int size = 16; // pixel
    int cols = screen_width / size;
    int rows = screen_height / size;
    bool isDrawing = false;
    bool isPlaying = false;
    bool paint = true;

    Color bkgColor; // (255, 255, 255)
    Color livingCellColor(3, 230, 56);
    Color deadCellColor(230, 150, 90);
    Color gradeColor(0, 0, 127);

    vec2D matrix(rows, std::vector<Color>(cols, bkgColor));

    // Adapted algorithm
    auto livingNeighbors = [&](int row, int col) {

        auto checkPostition = [&](int col, int row) {
            // Checks that it is within the bounds of the matrix.
            return (col >= 0 && col < cols && row >= 0 && row < rows);
        };

        int numberOfLiveNeighbors = 0;
        std::vector<std::vector<int> > mask {
            // {col, row}
            {-1, -1},  {0, -1},  {+1, -1},
            {-1,  0}, /* cell */ {+1,  0},
            {-1, +1},  {0, +1},  {+1, +1}
        };

        for (auto &p : mask) {
            if (checkPostition(col + p.front(), row + p.back())) {
                if (matrix[row + p.back()][col + p.front()].equal(livingCellColor)) {
                    numberOfLiveNeighbors++;
                }
            }
        }

        return numberOfLiveNeighbors;
    };

    auto validate = [&screen_width, &screen_height, &size](int mX, int mY) {
        return (mX >= 0 && mX < screen_width - size &&
                mY >= 0 && mY < screen_height - size);
    };

    int mX, mY; // mouse
    bool running = true;
    while (running) {

        // Events
        SDL_Event event;

        // Process Input
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    // Exit
                    running = false;
                }
                if (event.key.keysym.sym == SDLK_DELETE) {
                    // Clean all
                    SDL_SetWindowTitle(pWindow, title[0].c_str());
                    matrix = vec2D(rows, std::vector<Color>(cols, bkgColor));
                    isDrawing = false;
                    isPlaying = false;
                }
                if (event.key.keysym.sym == SDLK_F11) {
                    // Stop game of life
                    SDL_SetWindowTitle(pWindow, title[0].c_str());
                    isPlaying = false;
                }
                if (event.key.keysym.sym == SDLK_F12) {
                    // Run game of life
                    SDL_SetWindowTitle(pWindow, title[1].c_str());
                    isPlaying = true;
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    // Select a cell
                    isDrawing = true & !isPlaying;
                    paint = true;
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    // Unselect a cell
                    isDrawing = true & !isPlaying;
                    paint = false;
                }
            }
            if (event.type == SDL_MOUSEBUTTONUP) {
                // Stop drawing
                isDrawing = false;
            }
        }

        if (isDrawing) {
            SDL_GetMouseState(&mX, &mY);
            if (validate(mX, mY)) {
                if (paint) {
                    matrix[mY / size][mX / size] = livingCellColor;
                }
                else {
                    matrix[mY / size][mX / size] = bkgColor;
                }
            }
        }

        if (isPlaying) {
            for (int row = 0; row < rows; ++row) {
                for (int col = 0; col < cols; ++col) {
                    int count = livingNeighbors(row, col);
                    // Rule 1: Every dead cell with exactly three live neighbors becomes live (birth).
                    if (matrix[row][col].equal(bkgColor) && count == 3) {
                        matrix[row][col] = livingCellColor;
                        continue;
                    }
                    // Rule 2: Every living cell with less than two living neighbors dies from isolation.
                    if (matrix[row][col].equal(livingCellColor) && count < 2) {
                        matrix[row][col] = deadCellColor;
                        continue;
                    }
                    // Rule 3: Every living cell with more than three living neighbors dies by overpopulation.
                    if (matrix[row][col].equal(livingCellColor) && count > 3) {
                        matrix[row][col] = deadCellColor;
                        continue;
                    }
                    // Rule 4 : Every living cell with two or three living neighbors stays alive.
                    if (matrix[row][col].equal(livingCellColor)
                            && (count == 2 || count == 3)) {
                        matrix[row][col] = livingCellColor;
                    }
                }
            }
        }

        // Render
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                auto color = matrix[row][col];
                SDL_SetRenderDrawColor(pRenderer, color.R, color.G, color.B, color.A);
                SDL_Rect rect{col * size, row * size, size, size};
                if (color.equal(deadCellColor)) {
                    matrix[row][col] = bkgColor;
                    SDL_Delay(1);
                }
                SDL_RenderFillRect(pRenderer, &rect);
                SDL_SetRenderDrawColor(pRenderer, gradeColor.R, gradeColor.G, gradeColor.B, 127);
                SDL_RenderDrawLine(pRenderer, col * size, 0, col * size, rows * size);
            }
            SDL_SetRenderDrawColor(pRenderer, gradeColor.R, gradeColor.G, gradeColor.B, 127);
            SDL_RenderDrawLine(pRenderer, 0, row * size, cols * size, row * size);
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
    gameOfLife();

    return 0;
}
