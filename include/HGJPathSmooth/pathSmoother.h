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
    std::vector<TurnPoint> turnPoints;
    double jerkMax = 2.0;
    double accMax = 1.0;
    double spdMax = 2.0;
    double maxErr = 0.5;
    double ts = 0.01;

public:
    pathPlanner();
    HGJ::WayPoints
    genCurv(const WayPoints & wayPoints, double samplePeriod, double maxErr,
            double beginSpd = 0.0, double endSpd = 0.0);

    void setMaxJerk(double jerkMax);

    void setMaxAcc(double accMax);

    void setMaxSpeed(double spdMax);

    void setMaxErr(double maxErr);

private:
    void lpSolveTheDs(std::vector<TurnPoint> & turnPoints);
    /**
     * cal the time needed in the linear speed change progress
     * according to the jerkMax and the accMax
     * @param dv the spd difference
     * @return the Duration
     */
    inline double calChangeSpdDuration(double dv);

    /**
     * cal the pos in the linear speed change progress
     * @param v0 the start speed
     * @param dv the speed change
     * @param t the moment when the pos need to be calculated
     * @param sumT the whole time in the progress
     * @return the distence since the start point
     */
    inline double calChangeSpdPosition(double v0, double dv, double t, double sumT);

    /**
     * cal the distance needed in the linear change progress
     * @param v0 the start speed
     * @param v1 the end speed
     * @return the distances needed
     */
    inline double calChangeSpdDistance(double v0, double v1);

    inline double calBestSpdFromDistance(double v0, double dist, bool faster = true);
};


#endif //HELLOCPLEX_PATHPLANNER_H
