#pragma once

#include <SDL.h>
#include <canvas.h>
#include <pendulum.h>

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
    void render_circle(SDL_Renderer* renderer, double x, double y, double r);
    void render_pendulum(SDL_Renderer* renderer, const Pendulum& pendulum,
                         double x, double y, double r);
    void run();
};