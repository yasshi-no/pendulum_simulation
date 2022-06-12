#include <canvas.h>
#include <canvas_for_pendulum.h>

#include <vector>

/* PendulumFigureクラス */
PendulumFigure::PendulumFigure(vector<pair<double, double>> coords, double r)
    : coords(coords), r(r)
{
}
void PendulumFigure::draw(SDL_Renderer *renderer, double width, double height,
                          double center_x, double center_y, double scale) const
{
    /* 振り子を描画する関数 */
    int pendulum_num = (int)coords.size();
    // 糸の描画
    double bfr_x = 0.0, bfr_y = 0.0;  // 1つ上の振り子の座標(Canvas)
    for(int i = 0; i < pendulum_num; i++) {
        // 現在の振り子の座標(Canvas)
        double aft_x = coords[i].first;
        double aft_y = coords[i].second;

        // Canvas上で描画
        Line line(bfr_x, bfr_y, aft_x, aft_y);
        line.draw(renderer, width, height, center_x, center_y, scale);

        bfr_x = aft_x;
        bfr_y = aft_y;
    }
    // 振り子の描画
    for(int i = 0; i < pendulum_num; i++) {
        // Circleインスタンスの生成.
        // 半径はscaleに関わらず描画されるように修正する.
        Circle circle(coords[i].first, coords[i].second, r * scale);
        circle.draw(renderer, width, height, center_x, center_y, scale);
    }
    return;
}