//
// Created by stumbo on 18-11-5.
//

#ifndef HELLOCPLEX_PATHPLANNER_H
#define HELLOCPLEX_PATHPLANNER_H

#include "vec3f.h"
#include "TurnPoint.h"
#include <vector>

namespace HGJ {
    class pathPlanner;
    typedef std::vector<vec3f> WayPoints;
}

class HGJ::pathPlanner {
private:
    double maxSpeed = 2.0;
    double maxErr = 0.5;
    double ts = 0.01;

public:
    pathPlanner();
    HGJ::WayPoints genCurv(const WayPoints & wayPoints, const double & samplePeriod,
                           const double & maxErr);

    void setMaxSpeed(double maxSpeed);

private:
    std::vector<double> lpSolveTheDs(const std::vector<TurnPoint> & turnPoints);
};


#endif //HELLOCPLEX_PATHPLANNER_H
