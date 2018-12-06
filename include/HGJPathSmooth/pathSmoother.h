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

    /**
     * @enum UNSET not set
     * @enum ONEACC speed up to the target
     * @enum SPDUPDOWN speedup to a faster speed then quick down to v2
     * @enum TOMAXSPD can acc to max speed
     */
    enum LinearSpdType {
        /// not set
                UNSET = 0,
        /// speed up to the target
                ONEACC = 1,
        /// speedup to a faster speed then quick down to v2
                SPDUPDOWN = 2,
        /// can acc to max speed
                TOMAXSPD = 3
    };

private:
    // it is not accurate
    double curveLength;
    WayPoints answerCurve;
    std::vector<TurnPoint> turnPoints;
    std::vector<std::pair<LinearSpdType, double>> lineTypes;
    double jerkMax = 2.0;
    double accMax = 1.0;
    double spdMax = 2.0;
    double maxErr = 0.5;
    double beginSpd = 0.0;
    double endSpd = 0.0;
    double ts = 0.01;
    // the little gap rest when last progress not finished
    double resDis;

public:
    pathPlanner();
    const WayPoints &
    genCurv(const WayPoints & wayPoints, double samplePeriod, double maxErr,
            double beginSpd1 = 0.0, double endSpd1 = 0.0);

    void setMaxJerk(double jerkMax);

    void setMaxAcc(double accMax);

    void setMaxSpeed(double spdMax);

    void setMaxErr(double maxErr);

    void setBeginSpd(double beginSpd);

    void setEndSpd(double endSpd);

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

    /**
     * cal the fastest(lowest) speed we can get when we move through a dist
     * @param v0 the begin spd
     * @param dist the dist
     * @param faster find the fastest or the lowest
     * @return the fastest or the lowest speed
     */
    inline double calBestSpdFromDistance(double v0, double dist, bool faster = true);

    /**
     * cal the line type according to the begin speed, end speed and the distance
     * @param v0 begin speed of this line part
     * @param v1 end speed of this line part
     * @param dist the distance of this line
     * @return the line type of this part
     */
    inline std::pair<LinearSpdType, double> calLineType(double v0, double v1, double dist);

    /**
     * cal the min distance needed when using the v0 v1 and vMind
     * @param v0 begin speed
     * @param v1 end speed
     * @param vMid the max speed in the middle[
     * @return the min distance needed
     */
    inline double calLineDistance(double v0, double v1, double vMid);

    /**
     * assign the points to answerPoints at speed change stage
     * @param beginV the begin vel (might be smaller)
     * @param endV the end vel
     */
    void assignSpdChangePoints(double beginV, double endV, const vec3f & posBegin,
                               const vec3f & posEnd);

    /**
     * assign the points to answePoints at constrant speed stage
     * @param spd the speed
     */
    void assignLinearConstantSpeedPoints(double spd, const vec3f & posBegin,
                                         const vec3f & posEnd);

    /**
     * assign the points to answerPoints at turning stage
     * @param turnPoint the turnning part
     * @param firstPart if it is the first bezier part, there are two parts in a turnning
     */
    void assignTurnPartPoints(const TurnPoint & turnPoint, bool firstPart);

    void assignLinePartPoints(uint64_t index);
};


#endif //HELLOCPLEX_PATHPLANNER_H
