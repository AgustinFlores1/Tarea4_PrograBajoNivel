#pragma once
#include"cuerpo.h"
#include"variable2d.h"
#include<vector>

class sim {
private:
    cuerpo estrella;
    cuerpo planeta;
    std::vector<cuerpo> asteroides;
public:
    sim(double ms, double mp, double rp, int na, double rd);
    
    std::vector<variable2d> getAsteroidPositions(int na, double rd);
    variable2d getRandomInitialPosition(double distance);
    
    double getBodyDistance(const cuerpo& v1, const cuerpo& v2);
    void setInitialVelocity(cuerpo& c, double distance);

    void solveCollision(cuerpo& big_body, const cuerpo& small_body);
    int verifyEliminationState(cuerpo& c);

    variable2d getForce(const cuerpo& c1, const cuerpo& c2);
    variable2d getAcceleration(const variable2d& force, const cuerpo& c);

    std::vector<cuerpo> run(int ns);

    ~sim();
};