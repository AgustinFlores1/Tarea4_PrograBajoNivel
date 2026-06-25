#pragma once

class cuerpo {
private:
    double radio;
public:
    double masa;
    double x, y;
    double vx, vy;

    cuerpo();
    cuerpo(double xi, double yi, double mass, double radius);
    double getRadius();
    ~cuerpo();
};