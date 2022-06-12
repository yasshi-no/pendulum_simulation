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
    int pendulum_num;                        // 振り子の数
    vector<double> pendulum_string_lengths;  // 振り子の糸の長さ
    static const double time_delta;  // 1度の計算で経過する時間
    vector<double>
        pendulum_thetas;  // それぞれの振り子の中心とx軸の正の向きとの成す角
    vector<double> pendulum_velocitys;  // 角速度
    vector<pair<double, double>> pendulum_coords_bfr;
    vector<pair<double, double>> pendulum_coords_aft;

    void compute_A(double** A, const vector<double>& pendulum_thetas,
                   const vector<double>& pendulum_velocitys) const;
    void compute_b(double* b, const vector<double>& pendulum_thetas,
                   const vector<double>& pendulum_velocitys) const;

public:
    static const double PI;
    static const double G;

    Pendulum(int pendulum_num, vector<double> pendulum_string_lengths,
             vector<double> pendulum_thetas, vector<double> pendulum_velocitys);
    vector<pair<double, double>> compute_coords() const;
    double compute_physical_energy() const;
    void move();
    int get_pendulum_num() const;
    vector<pair<double, double>> get_pendulum_coords() const;
};
