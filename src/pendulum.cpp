#include <gauss_elimination.h>
#include <pendulum.h>

#include <cmath>
#include <vector>
using namespace std;

/*
振り子を表すクラス.
振り子の座標は, 左の方向にx軸, 下の方向にy軸が伸びている.
振り子の角度は, x軸の正の向きとの成す角.
*/
const double Pendulum::PI = 3.1415926535;
const double Pendulum::G = 4.0;
const double Pendulum::time_delta = 0.01;
Pendulum::Pendulum(int pendulum_num, double pendulum_string_length)
    : pendulum_num(pendulum_num), pendulum_string_length(pendulum_string_length)
{
    // 下に振り子が伸びるように初期化
    // pendulum_thetas = vector<double>(pendulum_num, PI / 2.0);
    pendulum_thetas = vector<double>(pendulum_num, 0.0);
    pendulum_velocitys = vector<double>(pendulum_num, 0.0);
    pendulum_coords_bfr = compute_coords();
    // for(int i = 0; i < pendulum_num; i++) {
    //   pendulum_thetas[i] = PI / 2.0 * (double)i;
    // }
}

vector<pair<double, double>> Pendulum::compute_coords() const
{
    /* 固定端を原点とする振り子の座標を返す. */
    vector<pair<double, double>> ret(pendulum_num);
    double now_x = 0.0, now_y = 0.0;  // 1つ上の振り子の座標
    for(int i = 0; i < pendulum_num; i++) {
        now_x = now_x + pendulum_string_length * cos(pendulum_thetas[i]);
        now_y = now_y + pendulum_string_length * sin(pendulum_thetas[i]);
        ret[i] = {now_x, now_y};
    }
    return ret;
}

void Pendulum::compute_A(double** A) const
{
    /* thetaの2回微分についての方程式の係数行列をAに格納する. */
    for(int i = 0; i < pendulum_num; i++) {
        for(int j = 0; j < pendulum_num; j++) {
            A[i][j] = (double)(pendulum_num - (double)max(i, j)) *
                      pendulum_string_length *
                      cos((PI / 2.0 - pendulum_thetas[i]) -
                          (PI / 2.0 - pendulum_thetas[j]));
        }
    }
    return;
}

void Pendulum::compute_b(double* b) const
{
    for(int i = 0; i < pendulum_num; i++) {
        b[i] = 0.0;
        for(int j = 0; j < pendulum_num; j++) {
            if(i == j) {
                b[i] = b[i] + (pendulum_num - i) * G *
                                  sin(PI / 2.0 - pendulum_thetas[i]);
            } else {
                b[i] = b[i] + (double)(pendulum_num - max(i, j)) *
                                  pendulum_string_length *
                                  pendulum_velocitys[j] *
                                  pendulum_velocitys[j] *
                                  sin((PI / 2.0 - pendulum_thetas[i]) -
                                      (PI / 2.0 - pendulum_thetas[j]));
            }
        }
    }
    return;
}

void Pendulum::move()
{
    /* 振り子を1単位時間動かす. */
    double** A = (double**)malloc(sizeof(double*) * pendulum_num);
    double* b = (double*)malloc(sizeof(double) * pendulum_num);
    double* x = (double*)malloc(sizeof(double) * pendulum_num);
    for(int i = 0; i < pendulum_num; i++) {
        A[i] = (double*)malloc(sizeof(double) * pendulum_num);
    }
    compute_A(A);
    compute_b(b);
    gauss_elimination(A, b, x, pendulum_num);
    for(int i = 0; i < pendulum_num; i++) {
        pendulum_velocitys[i] = pendulum_velocitys[i] + x[i] * time_delta;
        pendulum_thetas[i] =
            pendulum_thetas[i] + pendulum_velocitys[i] * time_delta;
    }
    pendulum_coords_bfr = pendulum_coords_aft;
    pendulum_coords_aft = compute_coords();
}

double Pendulum::compute_physical_energy() const
{
    /* 力学的エネルギーを返す. */
    double bfr_y = 0.0;             // 1つ上の振り子のy座標
    double potential_energy = 0.0;  // 位置エネルギー
    // for(int i = 0; i < pendulum_num; i++) {
    //     double aft_y = bfr_y + pendulum_string_length *
    //     sin(pendulum_thetas[i]); potential_energy = potential_energy - aft_y;
    //     bfr_y = aft_y;
    // }
    // potential_energy = potential_energy * G;
    for(int i = 0; i < pendulum_num; i++) {
        for(int j = 0; j <= i; j++) {
            potential_energy =
                potential_energy + cos((PI / 2.0 - pendulum_thetas[j]));
        }
    }
    potential_energy = potential_energy * G * pendulum_string_length;

    // for(int i = 0; i < pendulum_num; i++) {
    //     potential_energy += G * pendulum_coords_aft[i].second;
    // }

    double kinetic_energy = 0.0;  // 運動エネルギー

    for(int i = 0; i < pendulum_num; i++) {
        for(int j = 0; j <= i; j++) {
            kinetic_energy += pendulum_velocitys[j] * pendulum_velocitys[j];
            for(int k = j + 1; k <= i; k++) {
                kinetic_energy += 2 * pendulum_velocitys[j] *
                                  pendulum_velocitys[k] *
                                  cos(pendulum_thetas[j] - pendulum_thetas[i]);
            }
        }
    }
    kinetic_energy =
        pendulum_string_length * pendulum_string_length * kinetic_energy / 2.0;

    // for(int i = 0; i < pendulum_num; i++) {
    //     kinetic_energy +=
    //         ((pendulum_coords_aft[i].first - pendulum_coords_bfr[i].first) *
    //              (pendulum_coords_aft[i].first -
    //              pendulum_coords_bfr[i].first) +
    //          (pendulum_coords_aft[i].second - pendulum_coords_bfr[i].second)
    //          *
    //              (pendulum_coords_aft[i].second -
    //               pendulum_coords_bfr[i].second)) /
    //         time_delta / time_delta;
    // }
    // kinetic_energy = kinetic_energy / 2.0;

    // double test = 0.0;
    // double bfr_v = 0.0;  // 1つ上の振り子の速度
    // for(int i = 0; i < pendulum_num; i++) {
    //     double aft_v = bfr_v;
    //     for(int j = 0; j <= i; j++) {
    //         aft_v = aft_v + pendulum_string_length * pendulum_string_length *
    //                             pendulum_velocitys[j] * pendulum_velocitys[i]
    //                             * cos((PI / 2.0 - pendulum_thetas[j]) -
    //                                 (PI / 2.0 - pendulum_thetas[i]));
    //     }
    //     test += (aft_v) * (aft_v) / 2.0;
    //     bfr_v = aft_v;
    // }

    double ret = kinetic_energy - potential_energy;
    // double ret = (kinetic_energy - potential_energy) /
    //  (kinetic_energy + potential_energy);
    // double ret = kinetic_energy / potential_energy;
    // double ret = kinetic_energy;
    // double ret = potential_energy;
    return ret;
}

int Pendulum::get_pendulum_num() const { return pendulum_num; }
