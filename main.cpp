#include <SDL.h>
#include <gauss_elimination.h>
#include <pendulum.h>

#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

// 画面サイズ
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;
// ウィンドウに関するオブジェクト
SDL_Window* window = NULL;
// SDL_Surface* screen_surface = NULL;
SDL_Renderer* screen_renderer = NULL;

bool init();
bool load_media();
void close();

const double PI = 3.14159265359;

// SDLを初期化する
// 返り値:成功したか否か
bool init()
{
    // SDLを初期化する
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDLを初期化できませんでした." << std::endl;
        std::cout << "SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    // windowの作成
    // window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
    //                           SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
    //                           SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN,
                                &window, &screen_renderer);
    if(window == NULL || screen_renderer == NULL) {
        std::cout << "windowを作成できませんでした." << std::endl;
        std::cout << "SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    // window surfaceを取得
    // screen_surface = SDL_GetWindowSurface(window);

    return true;
}

// SDLを終了する
void close()
{
    // surfaceを解放する
    // SDL_FreeSurface(screen_surface);

    // rendererを開放する
    SDL_DestroyRenderer(screen_renderer);

    // windowを破棄する
    SDL_DestroyWindow(window);
    window = NULL;

    // SDL subsystemsを終了する
    SDL_Quit();
}

void render_circle(SDL_Renderer* renderer, double x, double y, double r)
{
    for(int i = -(int)r; i < (int)r; i++) {
        double half_span = sqrt(r * r - i * i);
        SDL_RenderDrawLine(screen_renderer, x + i, y - half_span, x + i,
                           y + half_span);
    }
    return;
}

void render_pendulum(SDL_Renderer* renderer, const Pendulum& pendulum, double x,
                     double y, double r)
{
    vector<pair<double, double>> pendulum_coords = pendulum.compute_coords();
    int bfr_x = 0.0, bfr_y = 0.0;
    for(int i = 0; i < pendulum.get_pendulum_num(); i++) {
        int aft_x = (int)pendulum_coords[i].first;
        int aft_y = (int)pendulum_coords[i].second;
        SDL_RenderDrawLine(renderer, bfr_x + (int)x, bfr_y + (int)y,
                           aft_x + (int)x, aft_y + (int)y);
        bfr_x = aft_x;
        bfr_y = aft_y;
    }
    for(int i = 0; i < pendulum.get_pendulum_num(); i++) {
        render_circle(renderer, pendulum_coords[i].first + x,
                      pendulum_coords[i].second + y, r);
    }

    return;
}

int main(int argc, char* argv[])
{
    Pendulum pendulum(20, 20.0);

    // SDLを初期化する
    if(!init()) {
        return 1;
    }

    bool quit = false;  // メインループを終了するか否か
    SDL_Event event;

    while(!quit) {
        pendulum.move();
        // rendererを更新する
        SDL_RenderClear(screen_renderer);
        // 背景の更新
        SDL_SetRenderDrawColor(screen_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(screen_renderer, NULL);
        // 振り子の更新
        SDL_SetRenderDrawColor(screen_renderer, 255, 255, 255,
                               SDL_ALPHA_OPAQUE);
        render_pendulum(screen_renderer, pendulum, (double)(SCREEN_WIDTH / 2),
                        (double)(SCREEN_HEIGHT / 2), 5);
        SDL_RenderPresent(screen_renderer);

        while(SDL_PollEvent(&event) != 0) {
            if(event.type == SDL_QUIT) {
                quit = true;
            }
        }
        SDL_Delay(1);
    }
    close();

    return 0;
}