#include <SDL.h>

#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

// 画面サイズ
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
// ウィンドウに関するオブジェクト
SDL_Window* window = NULL;
// SDL_Surface* screen_surface = NULL;
SDL_Renderer* screen_renderer = NULL;

bool init();
bool load_media();
void close();

const double PI = 3.14159265359;

class Pendulum
{
    /*
    振り子を表すクラス.
    振り子の座標は, 左の方向にx軸, 下の方向にy軸が伸びている.
    振り子の角度は, x軸の正の向きとの成す角.
    */
private:
    int pendulum_num;               // 振り子の数
    double pendulum_string_length;  // 振り子の間の糸の長さ
    // vector<pair<double, double>> pendulum_coords; //
    // それぞれの振り子の中心の座標
    vector<double>
        pendulum_thetas;  // それぞれの振り子の中心とx軸の正の向きとの成す角

public:
    Pendulum(int pendulum_num, double pendulum_string_length)
        : pendulum_num(pendulum_num),
          pendulum_string_length(pendulum_string_length)
    {
        // 下方向に振り子が伸びるよう初期化
        // pendulum_coords = vector<pair<double, double>>(pendulum_num);
        pendulum_thetas = vector<double>(pendulum_num, PI / 2.0);
        // for(int i = 0; i < pendulum_num; i++) {
        //     pendulum_coords[i] = {0.0,
        //                           pendulum_string_length * (double)(i + 1)};
        // }
    }

    vector<pair<double, double>> compute_coords() const
    {
        /*固定端を原点とする振り子の座標を返す.*/
        vector<pair<double, double>> ret(pendulum_num);
        double now_x = 0.0, now_y = 0.0;  // 1つ上の振り子の座標
        for(int i = 0; i < pendulum_num; i++) {
            now_x = now_x + pendulum_string_length * cos(pendulum_thetas[i]);
            now_y = now_y + pendulum_string_length * sin(pendulum_thetas[i]);
            ret[i] = {now_x, now_y};
        }
        return ret;
    }

    int get_pendulum_num() const { return pendulum_num; }
};

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
    int bfr_x = 0, bfr_y = 0;
    for(int i = 0; i < pendulum.get_pendulum_num(); i++) {
        int aft_x = (int)pendulum_coords[i].first;
        int aft_y = (int)pendulum_coords[i].second;
        SDL_RenderDrawLine(renderer, bfr_x + (int)x, bfr_y + (int)y,
                           aft_x + (int)x, aft_y + (int)y);
        bfr_x = aft_x;
        aft_x = aft_y;
    }
    for(int i = 0; i < pendulum.get_pendulum_num(); i++) {
        render_circle(renderer, pendulum_coords[i].first + x,
                      pendulum_coords[i].second + y, r);
    }

    return;
}

int main(int argc, char* argv[])
{
    Pendulum pendulum(3, 50.0);

    // SDLを初期化する
    if(!init()) {
        return 1;
    }

    // surfaceを白で埋める
    // SDL_FillRect(screen_surface, NULL,
    //              SDL_MapRGB(screen_surface->format, 0xFF, 0xFF, 0xFF));

    // rendererを更新する
    SDL_UpdateWindowSurface(window);
    SDL_SetRenderDrawColor(screen_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    // SDL_RenderDrawLine(screen_renderer, 0, 0, 100, 200);
    render_circle(screen_renderer, 100, 200, 10);
    render_pendulum(screen_renderer, pendulum, 100.0, 100.0, 10);
    SDL_RenderDrawLine(screen_renderer, -1, -200, 200, 100);
    SDL_RenderPresent(screen_renderer);

    // 待機する
    SDL_Delay(2000);

    bool quit = false;  // メインループを終了するか否か
    SDL_Event event;

    while(!quit) {
        while(SDL_PollEvent(&event) != 0) {
            if(event.type == SDL_QUIT) {
                quit = true;
            }
        }
    }
    close();

    return 0;
}