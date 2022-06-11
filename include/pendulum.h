#pragma once
#include <cmath>
#include <vector>
using namespace std;

/*
振り子を表すクラス.
振り子の座標は, 左の方向にx軸, 下の方向にy軸が伸びている.
振り子の角度は, x軸の正の向きとの成す角.
*/
class Pendulum
{
private:
    int pendulum_num;               // 振り子の数
    double pendulum_string_length;  // 振り子の間の糸の長さ
    vector<double>
        pendulum_thetas;  // それぞれの振り子の中心とx軸の正の向きとの成す角

public:
    static const double PI;

    Pendulum(int pendulum_num, double pendulum_string_length);
    vector<pair<double, double>> compute_coords() const;
    int get_pendulum_num() const;
};
