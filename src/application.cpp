#include <SDL.h>

#include <application.hpp>
#include <canvas.hpp>
#include <canvas_for_pendulum.hpp>
#include <cmath>
#include <iostream>
#include <pendulum.hpp>
#include <vector>

using namespace std;

Application::Application(int screen_width, int screen_height, Pendulum pendulum)
    : screen_width(screen_width),
      screen_height(screen_height),
      pendulum(pendulum)
{
    pendulum = pendulum;
    // canvas = Canvas(screen_width, screen_height);
    init();
}

bool Application::init()
{
    /* SDLを初期化する
    返り値:成功したか否か */
    // SDLを初期化する
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDLを初期化できませんでした." << std::endl;
        std::cout << "SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    // windowとrendererの作成
    SDL_CreateWindowAndRenderer(screen_width, screen_height, SDL_WINDOW_SHOWN,
                                &window, &screen_renderer);
    if(window == NULL || screen_renderer == NULL) {
        std::cout << "windowを作成できませんでした." << std::endl;
        std::cout << "SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void Application::close()
{
    /* SDLを終了する. */

    // rendererを開放する
    SDL_DestroyRenderer(screen_renderer);

    // windowを破棄する
    SDL_DestroyWindow(window);
    window = NULL;

    // SDL subsystemsを終了する
    SDL_Quit();
}

void Application::run()
{
    bool quit = false;  // メインループを終了するか否か
    SDL_Event event;
    int cnt = 0;
    double scale = 1.0;
    double scale_change_rate = 1.1;

    while(!quit) {
        pendulum.move();
        if(cnt++ % 100 != 0) {
            continue;
        }
        // rendererを更新する
        SDL_RenderClear(screen_renderer);
        // 背景の更新
        SDL_SetRenderDrawColor(screen_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(screen_renderer, NULL);
        // 振り子の更新
        SDL_SetRenderDrawColor(screen_renderer, 255, 255, 255,
                               SDL_ALPHA_OPAQUE);
        Canvas canvas(screen_width, screen_height);
        canvas.add_figure(
            make_shared<PendulumFigure>(pendulum.compute_coords(), 5.0));
        // canvasの描画
        canvas.draw(screen_renderer, screen_width, screen_height, 0.0, 0.0,
                    scale);
        // 画面の更新
        SDL_RenderPresent(screen_renderer);

        // イベントの処理
        while(SDL_PollEvent(&event) != 0) {
            switch(event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_MOUSEWHEEL:
                    SDL_Log("%d %d\n", event.wheel.x, event.wheel.y);
                    if(event.wheel.y > 0) {
                        scale /= scale_change_rate;
                    } else if(event.wheel.y < 0) {
                        scale *= scale_change_rate;
                    }
                    break;
                default:
                    break;
            }
        }

        // エネルギー保存の確認
        double potential_energy = pendulum.compute_potential_energy();
        double kinetic_energy = pendulum.compute_kinetic_energy();
        // SDL_Log("time=%f U=%f K=%f E=%f\n", pendulum.get_time(),
        //         potential_energy, kinetic_energy,
        //         potential_energy + kinetic_energy);
        // SDL_Delay(1);
    }
    close();
}
