#ifndef EXAMPLE_H
#define EXAMPLE_H

#include "../engine/engine.h"

// Simple Example
class Example : public Engine {

public:
    Example();

private:
    SDL_FRect m_dummy{0, 0, 0, 0};

    // Control function.
    void render() final;
    void update() final;

};

#endif
