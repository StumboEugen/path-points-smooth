//
// Created by stumbo on 18-11-5.
//

#include <ilcplex/ilocplex.h>

#include "pathSmoother.h"
#include "tools.h"
#include "TurnPoint.h"
#include <iostream>
#include <cmath>
#include <map>
#include <set>

using namespace std;

HGJ::pathPlanner::pathPlanner() = default;

HGJ::WayPoints
HGJ::pathPlanner::genCurv(const WayPoints & wayPoints, double ts, double maxErr,
                          double beginSpd, double endSpd) {
    pathPlanner::maxErr = maxErr;
    pathPlanner::ts = ts;
    pathPlanner::beginSpd = beginSpd;
    pathPlanner::endSpd = endSpd;

    int64_t pointCount = wayPoints.size();
    int64_t lineCount = pointCount - 1;
    int64_t cornerCount = pointCount - 2;

    if (pointCount <= 1) {
        return wayPoints;
    }

    if (pointCount == 2) {
        //TODO smooth
        return wayPoints;
    }

    //TODO consider the count == 3

    lineTypes.clear();
    lineTypes.reserve(static_cast<unsigned long>(lineCount));
    lineTypes.assign(static_cast<unsigned long>(lineCount), make_pair(UNSET, 0));

    turnPoints.clear();
    turnPoints.reserve(static_cast<unsigned long>(cornerCount));
    for (int64_t i = 0; i < cornerCount; i++) {
        turnPoints.emplace_back(wayPoints[i], wayPoints[i + 1], wayPoints[i + 2]);
    }
    vector<TurnPoint> & bezierCorners = turnPoints;

    // the answers will be stored in "bezierCorners"
    lpSolveTheDs(bezierCorners);


    // firstly set the speed of every corner to max
    for (auto & corner : bezierCorners) {
        corner.speed = corner.maxSpd;
    }

    // check the speed of every corner continously until they are all ok
    // map<speed, turns> in order of speed
    map<double, vector<uint64_t>> violations;

    /**
     * check the first and the end turn, to set a better max speed
     */
    auto & firstTurn = turnPoints.front();
    if (beginSpd < firstTurn.speed) {
        auto betterSpd = calBestSpdFromDistance(beginSpd, firstTurn.lenBefore - firstTurn.d);
        if (betterSpd < firstTurn.speed) {
            firstTurn.maxSpd = betterSpd;
            firstTurn.speed = betterSpd;
        }
    }

    auto & endTurn = turnPoints.back();
    if (endSpd < endTurn.speed) {
        auto betterSpd = calBestSpdFromDistance(endSpd, endTurn.lenAfter - endTurn.d);
        if (betterSpd < endTurn.speed) {
            endTurn.maxSpd = betterSpd;
            endTurn.speed = betterSpd;
        }
    }

    /**check lenBefore of every turn
     * if lenBefore is not long enough for the speed change
     * record the slower spd and the index*/
    for (uint64_t i = 1; i < turnPoints.size(); i++) {
        auto & thisTurn = turnPoints[i];
        auto & lastTurn = turnPoints[i - 1];
        auto dis = calChangeSpdDistance(lastTurn.speed, thisTurn.speed);
        if (dis < thisTurn.lenBefore - thisTurn.d - lastTurn.d) {
            if (thisTurn.speed <= lastTurn.speed) {
                violations[thisTurn.speed].push_back(i);
            } else {
                violations[lastTurn.speed].push_back(i);
            }
        }
    }

    for (auto & vioVectorPair: violations) {
        auto & vioIndexVector = vioVectorPair.second;

        // the size of the vioIndexVector may changed
        for (uint64_t vector_i = 0; vector_i < vioIndexVector.size(); vector_i++) {

            auto & thisTurn = turnPoints[vector_i];
            // this means that the turn has been modified, no need to do it again
            if (thisTurn.isSpdModifiedComplete()) {
                continue;
            }

            auto indexOfTurns = vioIndexVector[vector_i];

            if (indexOfTurns != 0) {
                auto & turnBefore = turnPoints[indexOfTurns - 1];
                if (turnBefore.speed > thisTurn.speed) {
                    auto safeSpd = calBestSpdFromDistance(thisTurn.speed, thisTurn.lenBefore);
                    if (turnBefore.speed > safeSpd) {
                        turnBefore.speed = safeSpd;
                        // check if the turnBefore has beforturn too
                        if (indexOfTurns > 1) {
                            auto & turnBBefore = turnPoints[indexOfTurns - 2];
                            if (turnBBefore.speed > turnBefore.speed) {
                                auto safeDis = calChangeSpdDistance
                                        (turnBefore.speed, turnBBefore.speed);
                                if (safeDis > turnBefore.lenBefore
                                              - (turnBBefore.d + turnBefore.d)) {
                                    violations[turnBefore.speed].push_back(indexOfTurns - 1);
                                }
                            }
                        }
                    }
                }
            }

            if (indexOfTurns != cornerCount - 1) {
                auto & turnAfter = turnPoints[indexOfTurns + 1];
                if (turnAfter.speed > thisTurn.speed) {
                    auto safeSpd = calBestSpdFromDistance(thisTurn.speed, thisTurn.lenAfter);
                    if (turnAfter.speed > safeSpd) {
                        turnAfter.speed = safeSpd;
                        if (indexOfTurns < cornerCount - 2) {
                            auto & turnAAfter = turnPoints[indexOfTurns + 2];
                            if (turnAAfter.speed > turnAfter.speed) {
                                auto safeDis = calChangeSpdDistance
                                        (turnAAfter.speed, turnAfter.speed);
                                if (safeDis > turnAAfter.lenAfter
                                              - (turnAAfter.d + turnAfter.d)) {
                                    violations[turnAfter.speed].push_back(indexOfTurns + 1);
                                }
                            }
                        }
                    }
                }
            }

            thisTurn.completeSpdModify();
        }
    }

    if (firstTurn.speed < beginSpd) {
        auto safeDis = calChangeSpdDistance(firstTurn.speed, beginSpd);
        if (safeDis < firstTurn.lenBefore - firstTurn.d) {
            auto safeSpd = calBestSpdFromDistance(firstTurn.speed,
                    firstTurn.lenBefore - firstTurn.d);
            cerr << "[WARNING] the begin speed you set is not safe:" << beginSpd <<
                    " ,the plan will use the max safe speed: " << safeSpd << endl;
            beginSpd = safeSpd;
        }
    }

    if (endTurn.speed < endSpd) {
        auto safeDis = calChangeSpdDistance(endTurn.speed, endSpd);
        if (safeDis < endTurn.lenAfter - endTurn.d) {
            auto safeSpd = calBestSpdFromDistance(endTurn.speed,
                    endTurn.lenAfter - endTurn.d);
            cerr << "[WARNING] the end speed you set is not safe:" << endSpd <<
                    " ,the plan will use the max safe speed: " << safeSpd << endl;
            endSpd = safeSpd;
        }
    }

    /**
     * cal the max speed in the linear duration
     */
    lineTypes.front() = calLineType(beginSpd, turnPoints.front().speed,
            turnPoints.front().lenBefore - turnPoints.front().d);

    for (uint64_t index_line = 0; index_line < lineTypes.size() - 1; ++index_line) {
        auto & beforeTurn = turnPoints[index_line - 1];
        auto & afterTurn = turnPoints[index_line];
        lineTypes[index_line] = calLineType(beforeTurn.speed, afterTurn.speed,
                beforeTurn.lenAfter - beforeTurn.d - afterTurn.d);
    }

    lineTypes.back() = calLineType(turnPoints.back().speed, endSpd,
            turnPoints.back().lenAfter - turnPoints.back().d);

    /**
     * plan the speed according to the ts
     */

    WayPoints ans;

    return ans;
}

void HGJ::pathPlanner::lpSolveTheDs(vector<TurnPoint> & turnPoints) {
    /**
     * start linear programing
     * using library IBM CPLEX
     */

    int64_t cornerCount = turnPoints.size();

    IloEnv env;

    IloModel model(env);
    /**
     * d[0] ~ d[cornerCount - 1] is d
     * d[cornerCount] is the max curvature
     */
    IloNumVarArray d(env);
    /**
     * constrictions
     */
    IloRangeArray c(env);
    /**
     * the optimize opject
     */
    IloObjective obj = IloMinimize(env);

    // adding the d varibles
    for (int i = 0; i < cornerCount; i++) {
        d.add(IloNumVar(env, 0.0, +IloInfinity));
    }
    d.add(IloNumVar(env, 0.0, +IloInfinity));

    for (int i = 0; i < cornerCount; i++) {

        // the curvature of every corner should be smaller than the max curvature
        c.add(IloRange(env, -IloInfinity, 0.0));
        double coeff_d2Kmax = turnPoints[i].coeff_d2Kmax;
        c[i].setLinearCoef(d[i], -coeff_d2Kmax);
        c[i].setLinearCoef(d[cornerCount], 1);

        // the optimization target
        obj.setLinearCoef(d[i], -coeff_d2Kmax);
    }
    obj.setLinearCoef(d[cornerCount], - cornerCount + 1.0);

    double coeff_beta2d = c4 * maxErr;
    for (int i = 0; i < cornerCount; i++) {

        // d should not be too big to bigger than the max error
        c.add(IloRange(env, -IloInfinity, coeff_beta2d / cos(turnPoints[i].beta)));
        c[cornerCount + i].setLinearCoef(d[i], 1);
    }

    for (int i = 0; i < cornerCount - 1; i++) {

        // d0 + d1 <= len(p1p2)
        c.add(IloRange(env, -IloInfinity , turnPoints[i].lenAfter));
        c[cornerCount * 2 + i].setLinearCoef(d[i], 1);
        c[cornerCount * 2 + i].setLinearCoef(d[i + 1], 1);
    }

    // d0 < len(p0p1) / 2
    c.add(IloRange(env, -IloInfinity , turnPoints.front().lenBefore / 2.0));
    c[cornerCount * 3 - 1].setLinearCoef(d[0], 1);

    // dn < len(pn pn+1) / 2
    c.add(IloRange(env, -IloInfinity , turnPoints.back().lenAfter / 2.0));
    c[cornerCount * 3].setLinearCoef(d[cornerCount - 1], 1);

    model.add(obj);
    model.add(c);

    IloCplex cplex(model);

    cplex.solve();

    IloNumArray ds(env);
    IloNumArray vals(env);
    cplex.getValues(ds, d);

//    env.out() << "Solution status = " << cplex.getStatus() << endl;
//    env.out() << "Solution value  = " << cplex.getObjValue() << endl;
//    env.out() << "Values        = " << ds << endl;

    for (int i = 0; i < ds.getSize() - 1; i++) {
        auto & tp = turnPoints[i];
        tp.d = static_cast<double>(ds[i]);
        tp.maxSpd = min(sqrt(accMax * tp.d * tp.coeff_d2Kmax), spdMax);
    }

    env.end();
}

double HGJ::pathPlanner::calChangeSpdDuration(double dv) {
    dv = abs(dv);
    double t1 = 1.875 * dv / accMax;
    double t2 = 5.7735 * dv / jerkMax;
    t2 = sqrt(t2);
    return max(t1, t2);
}

double HGJ::pathPlanner::calChangeSpdPosition(double v0, double dv, double t, double sumT) {
    double tRatio = t / sumT;
    double ans = 2.5 - (3 - tRatio) * tRatio;
    ans = ans * tRatio * tRatio * tRatio * dv + v0;
    cout << ans << endl;
    return ans * t;
}

double HGJ::pathPlanner::calChangeSpdDistance(double v0, double v1) {
    return (v0 + v1) / 2.0 * calChangeSpdDuration(v0 - v1);
}

/**
 * f(v1) = v1^3 + v0v1^2 - v0^2v1 - v0^3
 * f'(v1) = 3v1^2 + 2v0v1 - v0^2
 * @return f'(v1)
 */
inline double calSlope(double v0, double v1) {
    return (3 * v1 * v1) + (2 * v0 * v1) - (v0 * v0);   //TODO speed up
}

/**
 * f(v1) = v1^3 + v0v1^2 - v0^2v1 - v0^3
 * @return f(v1)
 */
inline double calFunc(double v0, double v1) {
    return pow(v1,3) + v0 * v1 * v1 - v0 * v0 * v1 - pow(v0,3); //TODO speed up
}

//TODO debug required
double HGJ::pathPlanner::calBestSpdFromDistance(double v0, double dist, bool faster) {

    if (!faster) {
        cout << "slower example" << endl;
    }

    if (abs(dist) < 0.01) {
        return v0;
    }

    // cal the v1Acc
    double tempSqrtDiff = 16.0 * accMax / 15.0 * dist;
    double v1Acc;
    if (faster) {
        v1Acc = sqrt(v0 * v0 + tempSqrtDiff);
    } else {
        if (v0 * v0 - tempSqrtDiff > 0) {
            v1Acc = sqrt(v0 * v0 - tempSqrtDiff);
        } else{
            v1Acc = 0;
        }
    }

    // cal the v1Jerk
    // solve the qubic function
    double targetY = 0.69282 * dist * dist * jerkMax;
    if (!faster) {
        targetY = - targetY;
        double min = calFunc(v0, v0 / 3.0);
        if (min < targetY) {
            return v1Acc;
        }
    }

    double v1Jerk = v0; //maybe not faster to start at v0
    double currentY = 0;
    // TODO determine the threshld
    while (abs(currentY - targetY) > 0.0001) {
        v1Jerk += (targetY - currentY) / calSlope(v0, v1Jerk);
        currentY = calFunc(v0, v1Jerk);
    }

    if (faster) {
        return min(v1Acc, v1Jerk);
    } else {
        return max(v1Acc, v1Jerk);
    }
}

pair<HGJ::pathPlanner::LinearSpdType, double>
HGJ::pathPlanner::calLineType(double v0, double v1, double dist) {
    double minDist = calChangeSpdDistance(v0, v1);
    if (dist < minDist * 1.05) {
        return make_pair(ONEACC, max(v0, v1));
    }

    double maxDist = calLineDistance(v0, v1, spdMax);
    double distDiff = maxDist - dist;

    if (distDiff < 0) {
        return make_pair(SPDUPDOWN, spdMax);
    }

    double midSpd = spdMax;
    double biggerV = max(v0, v1);

    while (true) {
        distDiff = calLineDistance(v0, v1, midSpd) - dist;
        if (distDiff > 0.0) {
            midSpd = (midSpd + biggerV) / 2.0;
        }
        else if(distDiff < - dist / 10000.0) {
            midSpd = (midSpd + spdMax) / 2.0;
        }
        break;
    }

    return make_pair(SPDUPDOWN, midSpd);
}

double HGJ::pathPlanner::calLineDistance(double v0, double v1, double vMid) {
    auto dist = calChangeSpdDistance(v0, vMid);
    dist += calChangeSpdDistance(vMid, v1);
    return dist;
}


void HGJ::pathPlanner::setMaxJerk(double jerkMax) {
    pathPlanner::jerkMax = jerkMax;
}

void HGJ::pathPlanner::setMaxAcc(double accMax) {
    pathPlanner::accMax = accMax;
}

void HGJ::pathPlanner::setMaxSpeed(double spdMax) {
    pathPlanner::spdMax = spdMax;
}

void HGJ::pathPlanner::setMaxErr(double maxErr) {
    pathPlanner::maxErr = maxErr;
}

void HGJ::pathPlanner::setBeginSpd(double beginSpd) {
    pathPlanner::beginSpd = beginSpd;
}

void HGJ::pathPlanner::setEndSpd(double endSpd) {
    pathPlanner::endSpd = endSpd;
}
