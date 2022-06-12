#include <SDL.h>
#include <application.h>
#include <canvas.h>
#include <pendulum.h>

#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

const double PI = 3.14159265359;

Application::Application(int screen_width, int screen_height, Pendulum pendulum)
    : screen_width(screen_width),
      screen_height(screen_height),
      pendulum(pendulum)
{
    pendulum = pendulum;
    // canvas = Canvas(screen_width, screen_height);
    init();
}
void Application::render_circle(SDL_Renderer* renderer, double x, double y,
                                double r)
{
    /* 円を描画する */
    for(int i = -(int)r; i < (int)r; i++) {
        double half_span = sqrt(r * r - i * i);
        SDL_RenderDrawLine(renderer, x + i, y - half_span, x + i,
                           y + half_span);
    }
    return;
}
void Application::render_pendulum(SDL_Renderer* renderer,
                                  const Pendulum& pendulum, double x, double y,
                                  double r)
{
    /* 振り子を描画する. */
    vector<pair<double, double>> coords = pendulum.get_coords();
    // canvas.clear();
    // 糸の描画
    int bfr_x = 0.0, bfr_y = 0.0;
    for(int i = 0; i < pendulum.get_pendulum_num(); i++) {
        int aft_x = (int)coords[i].first;
        int aft_y = (int)coords[i].second;
        SDL_RenderDrawLine(renderer, bfr_x + (int)x, bfr_y + (int)y,
                           aft_x + (int)x, aft_y + (int)y);
        // canvas.add_figure(Line(bfr_x, bfr_y, aft_x, aft_y));
        bfr_x = aft_x;
        bfr_y = aft_y;
    }
    // 振り子の描画
    for(int i = 0; i < pendulum.get_pendulum_num(); i++) {
        render_circle(renderer, coords[i].first + x, coords[i].second + y, r);
    }
    return;
};

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

    while(!quit) {
        Canvas canvas(screen_width, screen_height);
        canvas.add_figure(Line(100, 200, 400, 500));

        // pendulum.move();
        // if(cnt++ % 100 != 0) {
        //     continue;
        // }
        // rendererを更新する
        SDL_RenderClear(screen_renderer);
        // 背景の更新
        SDL_SetRenderDrawColor(screen_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(screen_renderer, NULL);
        // // 振り子の更新
        SDL_SetRenderDrawColor(screen_renderer, 255, 255, 255,
                               SDL_ALPHA_OPAQUE);
        canvas.draw(screen_renderer, 0.0, 0.0, 1.0);

        // render_pendulum(screen_renderer, pendulum, (double)(screen_width
        // / 2),
        //                 (double)(screen_height / 2), 5);
        SDL_RenderPresent(screen_renderer);

        // イベントの処理
        while(SDL_PollEvent(&event) != 0) {
            if(event.type == SDL_QUIT) {
                quit = true;
            }
        }
        // double potential_energy = pendulum.compute_potential_energy();
        // double kinetic_energy = pendulum.compute_kinetic_energy();
        // SDL_Log("time=%f U=%f K=%f E=%f\n", pendulum.get_time(),
        //         potential_energy, kinetic_energy,
        //         potential_energy + kinetic_energy);
        // SDL_Delay(1);
    }
    close();
}
