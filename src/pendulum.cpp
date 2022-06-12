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
const double Pendulum::time_delta = 0.0001;
Pendulum::Pendulum(int pendulum_num, vector<double> pendulum_string_lengths,
                   vector<double> pendulum_masss,
                   vector<double> pendulum_thetas,
                   vector<double> pendulum_velocitys)
    : pendulum_num(pendulum_num),
      pendulum_string_lengths(pendulum_string_lengths),
      pendulum_masss(pendulum_masss),
      pendulum_thetas(pendulum_thetas),
      pendulum_velocitys(pendulum_velocitys)
{
    // 質量の累積和の生成
    pendulum_masss_acmsum = vector<double>(pendulum_num + 1);
    pendulum_masss_acmsum[0] = 0.0;
    for(int i = 0; i < pendulum_num; i++) {
        pendulum_masss_acmsum[i + 1] =
            pendulum_masss_acmsum[i] + pendulum_masss[i];
    }

    // 座標の初期化
    // pendulum_coords_bfr = compute_coords();
    pendulum_coords_aft = compute_coords();
}

vector<pair<double, double>> Pendulum::compute_coords() const
{
    /* 固定端を原点とする振り子の座標を返す. */
    vector<pair<double, double>> ret(pendulum_num);
    double now_x = 0.0, now_y = 0.0;  // 1つ上の振り子の座標
    for(int i = 0; i < pendulum_num; i++) {
        now_x = now_x + pendulum_string_lengths[i] * cos(pendulum_thetas[i]);
        now_y = now_y + pendulum_string_lengths[i] * sin(pendulum_thetas[i]);
        ret[i] = {now_x, now_y};
    }
    return ret;
}

void Pendulum::compute_A(double** A, const vector<double>& pendulum_thetas,
                         const vector<double>& pendulum_velocitys) const
{
    /* thetaの2回微分についての方程式の係数行列をAに格納する. */
    for(int i = 0; i < pendulum_num; i++) {
        for(int j = 0; j < pendulum_num; j++) {
            A[i][j] = (pendulum_masss_acmsum[pendulum_num] -
                       pendulum_masss_acmsum[max(i, j)]) *
                      pendulum_string_lengths[j] *
                      cos((PI / 2.0 - pendulum_thetas[i]) -
                          (PI / 2.0 - pendulum_thetas[j]));
        }
    }
    return;
}

void Pendulum::compute_b(double* b, const vector<double>& pendulum_thetas,
                         const vector<double>& pendulum_velocitys) const
{
    /* thetaの2回微分についての方程式の定数項ベクトルをbに格納する. */
    for(int i = 0; i < pendulum_num; i++) {
        b[i] = 0.0;
        for(int j = 0; j < pendulum_num; j++) {
            if(i == j) {
                b[i] = b[i] + (pendulum_masss_acmsum[pendulum_num] -
                               pendulum_masss_acmsum[i]) *
                                  G * sin(PI / 2.0 - pendulum_thetas[i]);
            } else {
                b[i] = b[i] + (pendulum_masss_acmsum[pendulum_num] -
                               pendulum_masss_acmsum[max(i, j)]) *
                                  pendulum_string_lengths[j] *
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
    // メモリの確保
    double** A = (double**)malloc(sizeof(double*) * pendulum_num);
    double* b = (double*)malloc(sizeof(double) * pendulum_num);
    double* x = (double*)malloc(sizeof(double) * pendulum_num);
    for(int i = 0; i < pendulum_num; i++) {
        A[i] = (double*)malloc(sizeof(double) * pendulum_num);
    }

    // 4次のルンゲ・クッタ法により計算する
    vector<vector<double>> ks_thetas(
        pendulum_num, vector<double>(4));  // ルンゲ・クッタ法の係数
    vector<vector<double>> ks_velocitys(
        pendulum_num, vector<double>(4));  // ルンゲ・クッタ法の係数
    // 1つ目の傾きを求める
    vector<double> velocitys = pendulum_velocitys;
    vector<double> thetas = pendulum_thetas;
    compute_A(A, thetas, velocitys);
    compute_b(b, thetas, velocitys);
    gauss_elimination(A, b, x, pendulum_num);
    for(int i = 0; i < pendulum_num; i++) {
        ks_velocitys[i][0] = x[i];
        ks_thetas[i][0] = pendulum_velocitys[i];
    }
    // 2つ目の傾きを求める
    for(int i = 0; i < pendulum_num; i++) {
        velocitys[i] =
            pendulum_velocitys[i] + ks_velocitys[i][0] * time_delta / 2.0;
        thetas[i] = pendulum_thetas[i] + ks_thetas[i][0] * time_delta / 2.0;
    }
    compute_A(A, thetas, velocitys);
    compute_b(b, thetas, velocitys);
    gauss_elimination(A, b, x, pendulum_num);
    for(int i = 0; i < pendulum_num; i++) {
        ks_velocitys[i][1] = x[i];
        ks_thetas[i][1] = pendulum_velocitys[i];
    }
    // 3つ目の傾きを求める
    for(int i = 0; i < pendulum_num; i++) {
        velocitys[i] =
            pendulum_velocitys[i] + ks_velocitys[i][1] * time_delta / 2.0;
        thetas[i] = pendulum_thetas[i] + ks_thetas[i][1] * time_delta / 2.0;
    }
    compute_A(A, thetas, velocitys);
    compute_b(b, thetas, velocitys);
    gauss_elimination(A, b, x, pendulum_num);
    for(int i = 0; i < pendulum_num; i++) {
        ks_velocitys[i][2] = x[i];
        ks_thetas[i][2] = pendulum_velocitys[i];
    }
    // 4つ目の傾きを求める
    for(int i = 0; i < pendulum_num; i++) {
        velocitys[i] =
            pendulum_velocitys[i] + ks_velocitys[i][2] * time_delta / 2.0;
        thetas[i] = pendulum_thetas[i] + ks_thetas[i][2] * time_delta / 2.0;
    }
    compute_A(A, thetas, velocitys);
    compute_b(b, thetas, velocitys);
    gauss_elimination(A, b, x, pendulum_num);
    for(int i = 0; i < pendulum_num; i++) {
        ks_velocitys[i][3] = x[i];
        ks_thetas[i][3] = pendulum_velocitys[i];
    }

    // 傾きの加重平均をとり, 角速度と角を更新する
    for(int i = 0; i < pendulum_num; i++) {
        pendulum_velocitys[i] =
            pendulum_velocitys[i] +
            (ks_velocitys[i][0] + 2.0 * ks_velocitys[i][1] +
             2.0 * ks_velocitys[i][2] + ks_velocitys[i][3]) *
                time_delta / 6.0;
        pendulum_thetas[i] =
            pendulum_thetas[i] + (ks_thetas[i][0] + 2.0 * ks_thetas[i][1] +
                                  2.0 * ks_thetas[i][2] + ks_thetas[i][3]) *
                                     time_delta / 6.0;
    }

    // オイラー法
    // compute_A(A, pendulum_thetas, pendulum_velocitys);
    // compute_b(b, pendulum_thetas, pendulum_velocitys);
    // gauss_elimination(A, b, x, pendulum_num);
    // for(int i = 0; i < pendulum_num; i++) {
    //     pendulum_velocitys[i] = pendulum_velocitys[i] + x[i] * time_delta;
    //     pendulum_thetas[i] =
    //         pendulum_thetas[i] + pendulum_velocitys[i] * time_delta;
    // }

    // 現在と1単位時間前の直交座標を更新
    pendulum_coords_bfr = pendulum_coords_aft;
    pendulum_coords_aft = compute_coords();

    // メモリの解放
    for(int i = 0; i < pendulum_num; i++) {
        free(A[i]);
    }
    free(A);
    free(b);
    free(x);
}

double Pendulum::compute_physical_energy() const
{
    /* 力学的エネルギーを返す. */
    double potential_energy = 0.0;  // 位置エネルギー

    // 直交座標からO(pendulum_num)で計算
    for(int i = 0; i < pendulum_num; i++) {
        potential_energy +=
            -pendulum_masss[i] * G * pendulum_coords_aft[i].second;
    }

    // // 極座標からO(pendulum_num)で計算
    // double bfr_y = 0.0;  // 1つ上の振り子のy座標
    // for(int i = 0; i < pendulum_num; i++) {
    //     double aft_y = bfr_y + pendulum_string_length *
    //     sin(pendulum_thetas[i]); potential_energy = potential_energy - aft_y;
    //     bfr_y = aft_y;
    // }
    // potential_energy = potential_energy * G;

    // // 極座標からO(pendulum_num^2)で計算
    // for(int i = 0; i < pendulum_num; i++) {
    //     for(int j = 0; j <= i; j++) {
    //         potential_energy =
    //             potential_energy + cos((PI / 2.0 - pendulum_thetas[j]));
    //     }
    // }
    // potential_energy = potential_energy * G * pendulum_string_length;

    double kinetic_energy = 0.0;  // 運動エネルギー

    // 直交座標からO(pendulum_num)で計算
    for(int i = 0; i < pendulum_num; i++) {
        kinetic_energy +=
            ((pendulum_coords_aft[i].first - pendulum_coords_bfr[i].first) *
                 (pendulum_coords_aft[i].first - pendulum_coords_bfr[i].first) +
             (pendulum_coords_aft[i].second - pendulum_coords_bfr[i].second) *
                 (pendulum_coords_aft[i].second -
                  pendulum_coords_bfr[i].second)) *
            pendulum_masss[i] / time_delta / time_delta;
    }
    kinetic_energy = kinetic_energy / 2.0;

    // 極座標からO(pendulum_num^3)で計算(上手くいかない)
    // for(int i = 0; i < pendulum_num; i++) {
    //     for(int j = 0; j <= i; j++) {
    //         kinetic_energy += pendulum_velocitys[j] * pendulum_velocitys[j];
    //         for(int k = j + 1; k <= i; k++) {
    //             kinetic_energy += 2.0 * pendulum_velocitys[j] *
    //                               pendulum_velocitys[k] *
    //                               cos(pendulum_thetas[j] -
    //                               pendulum_thetas[i]);
    //         }
    //     }
    // }
    // kinetic_energy =
    //     pendulum_string_length * pendulum_string_length * kinetic_energy
    //     / 2.0;

    // 極座標からO(pendulum_num^2)で計算(上手くいかない)
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

    double ret = kinetic_energy + potential_energy;
    // double ret = (kinetic_energy - potential_energy) /
    //  (kinetic_energy + potential_energy);
    // double ret = kinetic_energy / potential_energy;
    // double ret = kinetic_energy;
    // double ret = potential_energy;
    return ret;
}

int Pendulum::get_pendulum_num() const { return pendulum_num; }
vector<pair<double, double>> Pendulum::get_pendulum_coords() const
{
    return pendulum_coords_aft;
}