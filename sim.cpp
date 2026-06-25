#include"sim.h"
#include"cuerpo.h"
#include"variable2d.h"
#include<vector>
#include<cmath>
#include<random>

variable2d sim::getRandomInitialPosition(double distance) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    double randomNumber = dist(gen);

    double pi = std::acos(-1.0);
    double randomAngle = randomNumber * (2.0 * pi);

    double x = distance * std::cos(randomAngle);
    double y = distance * std::sin(randomAngle);

    return variable2d(x, y);
}

void sim::setInitialVelocity(cuerpo& c, double distance) {
    if (distance == 0.0) return;

    double gravConst = 6.674e-11;
    double velocity = std::sqrt((gravConst * estrella.masa) / distance);

    c.vx = velocity * (c.y / distance);
    c.vy = velocity * (-c.x / distance);
}

void sim::solveCollision(cuerpo& bigBody, const cuerpo& smallBody) {
    variable2d momentum1 = variable2d(bigBody.masa * bigBody.vx, bigBody.masa * bigBody.vy);
    variable2d momentum2 = variable2d(smallBody.masa * smallBody.vx, smallBody.masa * smallBody.vy);

    double finalMass = bigBody.masa + smallBody.masa;
    variable2d finalVelocity = (momentum1 + momentum2) / finalMass;
    
    bigBody.masa = finalMass;
    bigBody.vx = finalVelocity.x;
    bigBody.vy = finalVelocity.y;
}

int sim::verifyEliminationState(cuerpo& c) {
    double starDistance = getBodyDistance(c, estrella);
    if (starDistance <= estrella.getRadius()) return 1;

    double planetDistance = getBodyDistance(c, planeta);
    if (planetDistance <= planeta.getRadius()) return 2;

    double rel_x = c.x - estrella.x;
    double rel_y = c.y - estrella.y;
    double rel_vx = c.vx - estrella.vx;
    double rel_vy = c.vy - estrella.vy;

    double product = (rel_x * rel_vx) + (rel_y * rel_vy);
    if (product > 0.0) {
        double gravConst = 6.674e-11;
        double currentVelocity = std::sqrt((rel_vx * rel_vx) + (rel_vy * rel_vy));
        double escapeVelocity = std::sqrt((2.0 * gravConst * estrella.masa) / starDistance);

        if (currentVelocity >= escapeVelocity) return 3;
    }

    return 0;
}

std::vector<variable2d> sim::getAsteroidPositions(int na, double rd) {
    std::vector<variable2d> positions;
    if (na <= 0) return positions;

    double pi = std::acos(-1.0);

    static std::random_device rdDev;
    static std::mt19937 gen(rdDev()); 
    std::uniform_real_distribution<double> dist(0.0, 2.0 * pi);

    double startAngle = dist(gen);
    double degreeSeparation = (2.0 * pi) / na;

    for (int i = 0; i < na; i++) {
        double currentAngle = startAngle +(i * degreeSeparation);
        positions.push_back(
            variable2d(rd * std::cos(currentAngle), rd * std::sin(currentAngle))
        );
    }

    return positions;
}

sim::sim(double ms, double mp, double rp, int na, double rd) {
    estrella = cuerpo(0.0, 0.0, ms, 7.0e8);

    variable2d randomPosition = getRandomInitialPosition(rp);
    planeta = cuerpo(randomPosition.x, randomPosition.y, mp, 7.0e7);

    setInitialVelocity(planeta, rp);

    std::vector<variable2d> positions = getAsteroidPositions(na, rd);
    for (int i = 0; i < na; i++) {
        cuerpo asteroide = cuerpo(positions[i].x, positions[i].y, 1.0e18, 0.0);

        setInitialVelocity(asteroide, rd);

        if (!verifyEliminationState(asteroide)) {
            asteroides.push_back(asteroide);
        }
    }
}
    
double sim::getBodyDistance(const cuerpo& c1, const cuerpo& c2) {
    double dx = c2.x - c1.x;
    double dy = c2.y - c1.y;
    return std::sqrt((dx * dx) + (dy * dy));
}

variable2d sim::getForce(const cuerpo& c1, const cuerpo& c2) {
    double gravConst = 6.674e-11;
    double rMagnitud = getBodyDistance(c1, c2);
    if (rMagnitud == 0.0) {
        return variable2d(0.0, 0.0);
    }

    variable2d unitR = variable2d(c2.x - c1.x, c2.y - c1.y) / rMagnitud;
    double force = (gravConst * c1.masa * c2.masa) / (rMagnitud * rMagnitud);

    return (unitR * force);
}

variable2d sim::getAcceleration(const variable2d& force, const cuerpo& c) {
    return force / c.masa;
}

std::vector<cuerpo> sim::run(int ns) {
    double dt = 84600.0;
    for (int i = 0; i < ns; i++) {
        variable2d sunAcc = variable2d(0.0, 0.0);
        variable2d planetAcc = variable2d(0.0, 0.0);

        variable2d sunPlanetF = getForce(estrella, planeta);
        sunAcc = sunAcc + getAcceleration(sunPlanetF, estrella);
        planetAcc = planetAcc - getAcceleration(sunPlanetF, planeta);

        std::vector<cuerpo> remainingAsteroids;
        std::vector<cuerpo> starCollisions;
        std::vector<cuerpo> planetCollisions;

        for (int j = 0; j < int(asteroides.size()); j++) {
            variable2d asteroidAcc = variable2d(0.0, 0.0);
            
            variable2d asteroidSunF = getForce(asteroides[j], estrella);
            asteroidAcc = asteroidAcc + getAcceleration(asteroidSunF, asteroides[j]);
            sunAcc = sunAcc - getAcceleration(asteroidSunF, estrella);

            variable2d asteroidPlanetF = getForce(asteroides[j], planeta);
            asteroidAcc = asteroidAcc + getAcceleration(asteroidPlanetF, asteroides[j]);
            planetAcc = planetAcc - getAcceleration(asteroidPlanetF, planeta);

            asteroides[j].vx += asteroidAcc.x * dt;
            asteroides[j].vy += asteroidAcc.y * dt;
            asteroides[j].x += asteroides[j].vx * dt;
            asteroides[j].y += asteroides[j].vy * dt;

            int isEliminated = verifyEliminationState(asteroides[j]);
            if (isEliminated == 0) {
                remainingAsteroids.push_back(asteroides[j]);
            }
            else if (isEliminated == 1) {
                starCollisions.push_back(asteroides[j]);
            } 
            else if (isEliminated == 2) {
                planetCollisions.push_back(asteroides[j]);
            }
        }
        asteroides = remainingAsteroids;

        estrella.vx += sunAcc.x * dt;
        estrella.vy += sunAcc.y * dt;
        estrella.x += estrella.vx * dt;
        estrella.y += estrella.vy * dt;

        planeta.vx += planetAcc.x * dt; 
        planeta.vy += planetAcc.y * dt;
        planeta.x += planeta.vx * dt;
        planeta.y += planeta.vy * dt;

        for (int k = 0; k < int(starCollisions.size()); k++) {
            solveCollision(estrella, starCollisions[k]);
        }
        for (int k = 0; k < int(planetCollisions.size()); k++) {
            solveCollision(planeta, planetCollisions[k]);
        }
    }

    std::vector<cuerpo> sistem;
    sistem.push_back(estrella);
    sistem.push_back(planeta);

    for (int i = 0; i < int(asteroides.size()); i++) {
        sistem.push_back(asteroides[i]);
    }

    return sistem;
}

sim::~sim() {}