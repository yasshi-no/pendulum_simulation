#include <SDL.h>
#include <application.h>
#include <pendulum.h>

#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

// 画面サイズ
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

int main(int argc, char* argv[])
{
    // 振り子の生成
    double pendulum_num = 10;
    vector<double> string_lengths(pendulum_num, 20.0);
    for(int i = 0; i < pendulum_num; i++) {
        string_lengths[i] = 20.0 + 10.0 * (double)(i % 2);
    }
    vector<double> masss(pendulum_num, 1.0);
    masss[pendulum_num - 1] = 10.0;
    vector<double> thetas(pendulum_num, 0.0);
    vector<double> velocitys(pendulum_num, 0.01);
    Pendulum pendulum(pendulum_num, string_lengths, masss, thetas, velocitys);

    Application application(SCREEN_WIDTH, SCREEN_HEIGHT, pendulum);
    application.run();

    return 0;
}