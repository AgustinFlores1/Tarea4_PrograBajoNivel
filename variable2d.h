#pragma once
#include<cmath>

struct variable2d {
    double x;
    double y;

    variable2d() {x = 0.0; y = 0.0;}
    variable2d(double xi, double yi) {x = xi; y = yi;}

    variable2d operator+(const variable2d& other) const {
        return variable2d(x + other.x, y + other.y);
    }

    variable2d operator-(const variable2d& other) const {
        return variable2d(x - other.x, y - other.y);
    }

    variable2d operator*(double escalar) const {
        return variable2d(x * escalar, y * escalar);
    }

    variable2d operator/(double escalar) const {
        if (escalar != 0) {
            return variable2d(x / escalar, y / escalar);
        }
        return variable2d(0, 0);
    }

    double magnitud() const {
        return std::sqrt((x * x) + (y * y));
    }
};
