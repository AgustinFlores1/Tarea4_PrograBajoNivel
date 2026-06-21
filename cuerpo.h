#pragma once
#include<string>

class cuerpo {
private:
    std::string tipo;
    double radio;
public:
    double masa;
    double x, y;
    double vx, vy;

    cuerpo();
    cuerpo(double x_inicial, double y_inicial, double masa, std::string tipo);
    double getRadius();
    ~cuerpo();
};