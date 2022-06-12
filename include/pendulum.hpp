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
    double time;                    // 時間
    vector<double> string_lengths;  // 振り子の糸の長さ
    vector<double> masss;           // 振り子の質量
public:
    vector<double>
        masss_acmsum;  // 振り子の質量の累積和(質量のインデックスは1つ大きくずらす)
private:
    static const double time_delta;  // 1度の計算で経過する時間
    vector<double> thetas;  // それぞれの振り子の中心とx軸の正の向きとの成す角
    vector<double> velocitys;  // 角速度
    vector<pair<double, double>> coords_bfr;
    vector<pair<double, double>> coords_aft;

    void compute_A(double** A, const vector<double>& thetas,
                   const vector<double>& velocitys) const;
    void compute_b(double* b, const vector<double>& thetas,
                   const vector<double>& velocitys) const;

public:
    static const double PI;
    static const double G;

    Pendulum(int pendulum_num, vector<double> string_lengths,
             vector<double> masss, vector<double> thetas,
             vector<double> velocitys);
    vector<pair<double, double>> compute_coords() const;
    double compute_potential_energy() const;
    double compute_kinetic_energy() const;
    double compute_physical_energy() const;
    void move();
    int get_pendulum_num() const;
    double get_time() const;
    vector<pair<double, double>> get_coords() const;
};
