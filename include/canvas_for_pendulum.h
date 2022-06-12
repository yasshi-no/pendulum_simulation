#include <canvas.h>
#include <pendulum.h>

class PendulumFigure : public Figure
{
    /* Canvasで扱うPendulumのクラス */
    vector<pair<double, double>> coords;
    double r;

public:
    PendulumFigure(vector<pair<double, double>> coords, double r);
    void draw(SDL_Renderer *renderer, double width, double height,
              double center_x, double center_y, double scale) const;
};