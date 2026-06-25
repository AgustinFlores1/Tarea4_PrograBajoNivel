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

    double random_number = dist(gen);

    double pi = std::acos(-1.0);
    double random_angle = random_number * (2.0 * pi);

    double x = distance * std::cos(random_angle);
    double y = distance * std::sin(random_angle);

    return variable2d(x, y);
}

void sim::setInitialVelocity(cuerpo& c, double distance) {
    if (distance == 0.0) return;

    double gravConst = 6.674e-11;
    double velocity = std::sqrt((gravConst * estrella.masa) / distance);

    c.vx = velocity * (c.y / distance);
    c.vy = velocity * (-c.x / distance);
}

void sim::solveCollision(cuerpo& big_body, const cuerpo& small_body) {
    variable2d momentum1 = variable2d(big_body.masa * big_body.vx, big_body.masa * big_body.vy);
    variable2d momentum2 = variable2d(small_body.masa * small_body.vx, small_body.masa * small_body.vy);

    double final_mass = big_body.masa + small_body.masa;
    variable2d final_velocity = (momentum1 + momentum2) / final_mass;
    
    big_body.masa = final_mass;
    big_body.vx = final_velocity.x;
    big_body.vy = final_velocity.y;
}

int sim::verifyEliminationState(cuerpo& c) {
    double star_distance = getBodyDistance(c, estrella);
    if (star_distance <= estrella.getRadius()) return 1;

    double planet_distance = getBodyDistance(c, planeta);
    if (planet_distance <= planeta.getRadius()) return 2;

    double product = (c.x * c.vx) + (c.y * c.vy);
    if (product > 0.0) {
        double gravConst = 6.674e-11;
        double current_velocity = std::sqrt((c.vx * c.vx) + (c.vy * c.vy));
        double escape_velocity = std::sqrt((2.0 * gravConst * estrella.masa) / star_distance);

        if (current_velocity >= escape_velocity) return 3;
    }

    return 0;
}

std::vector<variable2d> sim::getAsteroidPositions(int na, double rd) {
    std::vector<variable2d> positions;
    if (na <= 0) return positions;

    double pi = std::acos(-1.0);

    static std::random_device rd_dev;
    static std::mt19937 gen(rd_dev()); 
    std::uniform_real_distribution<double> dist(0.0, 2.0 * pi);

    double start_angle = dist(gen);
    double degree_separation = (2.0 * pi) / na;

    for (int i = 0; i < na; i++) {
        double current_angle = start_angle +(i * degree_separation);
        positions.push_back(
            variable2d(rd * std::cos(current_angle), rd * std::sin(current_angle))
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
    double r_mag = getBodyDistance(c1, c2);
    if (r_mag == 0.0) {
        return variable2d(0.0, 0.0);
    }

    variable2d unit_r = variable2d(c2.x - c1.x, c2.y - c1.y) / r_mag;
    double force = (gravConst * c1.masa * c2.masa) / (r_mag * r_mag);

    return (unit_r * force);
}

variable2d sim::getAcceleration(const variable2d& force, const cuerpo& c) {
    return force / c.masa;
}

std::vector<cuerpo> sim::run(int ns) {}