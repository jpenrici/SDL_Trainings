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

int paint()
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

    SDL_RenderClear(pRenderer);

    // Drawing Area
    int size = 8; // pixel
    int cols = screen_width / size;
    int rows = screen_height / size;
    bool isDrawing = false;

    vec2D matrix(rows, std::vector<Color>(cols, Color()));

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
                    running = false;
                }
                if (event.key.keysym.sym == SDLK_DELETE) {
                    matrix = vec2D(rows, std::vector<Color>(cols, Color()));
                    isDrawing = false;
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    isDrawing = true;
                }
            }
            if (event.type == SDL_MOUSEBUTTONUP) {
                isDrawing = false;
            }
        }

        if (isDrawing) {
            SDL_GetMouseState(&mX, &mY);
            if (validate(mX, mY)) {
                matrix[mY / size][mX / size] = Color(0, 0, 0);
            }
        }

        // Render
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                auto color = matrix[row][col];
                SDL_SetRenderDrawColor(pRenderer, color.R, color.G, color.B, color.A);
                SDL_Rect rect{col * size, row * size, size, size};
                SDL_RenderFillRect(pRenderer, &rect);
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
    paint();

    return 0;
}
