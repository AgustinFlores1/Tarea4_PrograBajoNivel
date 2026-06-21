#pragma once
#include"cuerpo.h"
#include<vector>

class sim {
private:
    cuerpo estrella;
    cuerpo planeta;
    std::vector<cuerpo> asteroides;
public:
    sim(double ms, double mp, double rp, int na, double rd);
    std::vector<cuerpo> run(int ns);
    
    double calculateDistance(double x1, double y1, double x2, double y2);
    std::vector<double> calculateAcceleration(std::vector<double> force);
};