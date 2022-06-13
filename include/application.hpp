#pragma once

#include <SDL.h>

#include <canvas.hpp>
#include <pendulum.hpp>

class Application
{
    /* SDL2のアプリケーションを管理するクラス. */
private:
    double screen_width;
    double screen_height;
    Pendulum pendulum;
    SDL_Window* window;
    SDL_Renderer* screen_renderer;
    // Canvas canvas;

    bool init();
    bool load_media();
    void close();

public:
    Application(int screen_width, int screen_height, Pendulum pendulum);
    void run();
};