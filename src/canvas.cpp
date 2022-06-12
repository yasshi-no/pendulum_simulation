#include <SDL.h>
#include <canvas.h>

#include <vector>
using namespace std;

void render_circle(SDL_Renderer* renderer, double x, double y, double r)
{
    /* 円を描画する */
    for(int i = -(int)r; i < (int)r; i++) {
        double half_span = sqrt(r * r - i * i);
        SDL_RenderDrawLine(renderer, x + i, y - half_span, x + i,
                           y + half_span);
    }
    return;
}

/* Figureクラス */
void Figure::draw(SDL_Renderer* renderer, double center_x, double center_y,
                  double scale) const
{
    return;
}

/* Circleクラス */
Circle::Circle(double x, double y, double r) : x(x), y(y), r(r) {}
void Circle::draw(SDL_Renderer* renderer, double center_x, double center_y,
                  double scale) const
{
    /* 位置を修正して描画する. */
    // 中心からの変位
    double dx = x - center_x;
    double dy = y - center_y;
    // 中心からの変位をscale倍する
    double new_x = center_x + dx * scale;
    double new_y = center_y + dy * scale;
    // 半径をscale倍する
    double new_r = r * scale;

    // 描画
    render_circle(renderer, new_x, new_y, new_r);
    return;
}

/* Lineクラス */
Line::Line(double x1, double y1, double x2, double y2)
    : x1(x1), y1(y1), x2(x2), y2(y2)
{
}
void Line::draw(SDL_Renderer* renderer, double center_x, double center_y,
                double scale) const
{
    /* 位置を修正して描画する. */
    // 中心からの変位
    double dx1 = x1 - center_x;
    double dy1 = y1 - center_y;
    double dx2 = x2 - center_x;
    double dy2 = y2 - center_y;
    // 中心からの変位をscale倍する
    double new_x1 = center_x + dx1 * scale;
    double new_y1 = center_y + dy1 * scale;
    double new_x2 = center_x + dx2 * scale;
    double new_y2 = center_y + dy2 * scale;

    // 描画
    SDL_RenderDrawLine(renderer, (int)new_x1, (int)new_y1, (int)new_x2,
                       (int)new_y2);
    return;
}

/* Canvasクラス */
Canvas::Canvas(int width, int height) : width(width), height(height)
{
    figures = vector<Figure>();  // Figureオブジェクトを格納する配列
}
void Canvas::clear()
{
    /* Figureオブジェクトを消去する. */
    figures.clear();
    return;
}
void Canvas::add_figure(Figure figure)
{
    /* Figureオブジェクトを追加する. */
    figures.push_back(figure);
    return;
};
void Canvas::draw(SDL_Renderer* renderer, double center_x, double center_y,
                  double scale) const
{
    /* Figureオブジェクトの位置を修正して描画する. */
    for(const Figure& figure : figures) {
        figure.draw(renderer, center_x, center_y, scale);
    }
    return;
};
