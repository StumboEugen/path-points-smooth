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

const HGJ::WayPoints &
HGJ::pathPlanner::genCurv(const WayPoints & wayPoints, double ts, double maxErr,
                          double beginSpdInput, double endSpdInput) {

    /// load the setting
    pathPlanner::maxErr = maxErr;
    pathPlanner::ts = ts;
    pathPlanner::beginSpd = beginSpdInput;
    pathPlanner::endSpd = endSpdInput;

    /// cal the point count
    int64_t pointCount = wayPoints.size();
    int64_t lineCount = pointCount - 1;
    int64_t cornerCount = pointCount - 2;

    /// clear the last calculation
    resDis = 0.0;
    answerCurve.clear();

    /// only one(zero) point is inputed, return the point
    if (pointCount <= 1) {
        cerr << "you only add 1 point" << endl;
        answerCurve = wayPoints;
        return wayPoints;
    }

    /// only two points are added, smooth the line
    if (pointCount == 2) {
        cerr << "you only add 2 points" << endl;

        turnPoints.clear();
        lineTypes.clear();

        turnPoints.emplace_back(wayPoints[0], wayPoints[0], wayPoints[1]);

        /// the distance may be not long enough for the speed change
        double safeDis = calChangeSpdDistance(pathPlanner::beginSpd, pathPlanner::endSpd);
        double theDis = (wayPoints[1] - wayPoints[0]).len();

        /// the distance is not long enough, the end speed need to be modified
        if (theDis < safeDis) {
            pathPlanner::endSpd = calBestSpdFromDistance(
                    pathPlanner::beginSpd, theDis,
                    (pathPlanner::endSpd >pathPlanner::beginSpd));
            cerr << "the end spd is not possible, fall back to:" << pathPlanner::endSpd << endl;
            lineTypes.emplace_back(ONEACC, pathPlanner::endSpd);
        } else {
            lineTypes.push_back(
                    calLineType(pathPlanner::beginSpd, pathPlanner::endSpd, theDis));
        }

        assignLinePartPoints(0);
        return answerCurve;
    }

    /// cal the curveLength, not used for now
    curveLength = 0.0;
    for (uint64_t i = 1; i < pointCount; ++i) {
        curveLength += (wayPoints[i] - wayPoints[i-1]).len();
    }

    /// init the containers
    lineTypes.clear();
    lineTypes.reserve(static_cast<unsigned long>(lineCount));
    lineTypes.assign(static_cast<unsigned long>(lineCount), make_pair(UNSET, 0));

    turnPoints.clear();
    turnPoints.reserve(static_cast<unsigned long>(cornerCount));
    for (int64_t i = 0; i < cornerCount; i++) {
        /// in the constructor, parameters about the corner are all calculated
        turnPoints.emplace_back(wayPoints[i], wayPoints[i + 1], wayPoints[i + 2]);
    }

    /// the answers(d) will be stored in "turnPoints"
    lpSolveTheDs(turnPoints);

    /// firstly set the speed of every corner to max
    for (auto & corner : turnPoints) {
        corner.speed = corner.maxSpd;
    }

    ///  check the speed of every corner continously until they are all ok.

    /**
     *  the violations: the length between two turns can't satisfy the speed change between
     *  two max speed. the results are stored in map(speed, turns) in order of speed
     */
    map<double, vector<uint64_t>> violations;

    /// check the first and the end turn, to set a better max speed
    /// which solve the condition when the begin/end speed is too small for the first/end turn
    auto & firstTurn = turnPoints.front();
    if (pathPlanner::beginSpd < firstTurn.speed) {
        auto safeSpd = calBestSpdFromDistance(
                pathPlanner::beginSpd, firstTurn.lenBefore - firstTurn.d);
        if (safeSpd < firstTurn.speed) {
            firstTurn.maxSpd = safeSpd;
            firstTurn.speed = safeSpd;
        }
    }

    auto & endTurn = turnPoints.back();
    if (pathPlanner::endSpd < endTurn.speed) {
        auto safeSpd = calBestSpdFromDistance(
                pathPlanner::endSpd, endTurn.lenAfter - endTurn.d);
        if (safeSpd < endTurn.speed) {
            endTurn.maxSpd = safeSpd;
            endTurn.speed = safeSpd;
        }
    }

    /**check lenBefore of every turn
     * if lenBefore is not long enough for the speed change
     * record the slower spd and the index*/
    for (uint64_t i = 1; i < turnPoints.size(); i++) {
        auto & thisTurn = turnPoints[i];
        auto & lastTurn = turnPoints[i - 1];
        auto minDis = calChangeSpdDistance(lastTurn.speed, thisTurn.speed);
        if (minDis > thisTurn.lenBefore - thisTurn.d - lastTurn.d) {
            if (thisTurn.speed < lastTurn.speed) {
                violations[thisTurn.speed].push_back(i);
            } else {
                violations[lastTurn.speed].push_back(i-1);
            }
        }
    }

    /// modify the speed continusly, from the slower to faster to ensure the plan is the best.
    /// in the progress, new violations might be added
    for (auto & vioVectorPair: violations) {

        /// the index which violated at speed of "vioVectorPair.first"
        auto & vioIndexVector = vioVectorPair.second;

        /// the size of the vioIndexVector may changed, C++11 for (:) CAN NOT be used
        for (uint64_t vector_i = 0; vector_i < vioIndexVector.size(); vector_i++) {

            auto indexOfTurns = vioIndexVector[vector_i];
            auto & thisTurn = turnPoints[indexOfTurns];

            /// this means that the turn has been modified, no need to do it again
            if (thisTurn.isSpdModifiedComplete()) {
                continue;
            }

            if (indexOfTurns != 0) { /// it's not the first turn, check the spd of perious turn
                auto & turnBefore = turnPoints[indexOfTurns - 1];
                if (turnBefore.speed > thisTurn.speed) {
                    auto safeSpd = calBestSpdFromDistance(
                            thisTurn.speed, thisTurn.lenBefore - thisTurn.d - turnBefore.d);
                    if (turnBefore.speed > safeSpd) { /// turnBefore.speed is too fast!
                        turnBefore.speed = safeSpd;
                        if (indexOfTurns > 1) { /// check if the BBfore Turn becomces violation
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

            /// it's almost the same as last section
            if (indexOfTurns != cornerCount - 1) {
                auto & turnAfter = turnPoints[indexOfTurns + 1];
                if (turnAfter.speed > thisTurn.speed) {
                    auto safeSpd = calBestSpdFromDistance(
                            thisTurn.speed, thisTurn.lenAfter - thisTurn.d - turnAfter.d);
                    if (turnAfter.speed > safeSpd) {
                        turnAfter.speed = safeSpd;
                        if (indexOfTurns < cornerCount - 2) {
                            auto & turnAAfter = turnPoints[indexOfTurns + 2];
                            if (turnAAfter.speed > turnAfter.speed) {
                                auto safeDis = calChangeSpdDistance
                                        (turnAAfter.speed, turnAfter.speed);
                                if (safeDis > turnAfter.lenAfter
                                              - (turnAAfter.d + turnAfter.d)) {
                                    violations[turnAfter.speed].push_back(indexOfTurns + 1);
                                }
                            }
                        }
                    }
                }
            }

            /// one turn may be modified twice
            thisTurn.completeSpdModify();
        }
    }


    /// at here, the spd of every turn has been set to the fastest speed.
    /// if the beginSpd or the endSpd are too fast, they need to be slower down
    if (firstTurn.speed < pathPlanner::beginSpd) {
        auto safeDis = calChangeSpdDistance(firstTurn.speed, pathPlanner::beginSpd);
        auto theReadDis = firstTurn.lenBefore - firstTurn.d;
        if (safeDis > theReadDis) {
            auto safeSpd = calBestSpdFromDistance(firstTurn.speed, theReadDis);
            cerr << "[WARNING] the begin speed you set is not safe:" << pathPlanner::beginSpd <<
                    " ,the plan will use the max safe speed: " << safeSpd << endl;
            pathPlanner::beginSpd = safeSpd;
        }
    }

    if (endTurn.speed < pathPlanner::endSpd) {
        auto safeDis = calChangeSpdDistance(endTurn.speed, pathPlanner::endSpd);
        auto theReadDis = endTurn.lenAfter - endTurn.d;
        if (safeDis > theReadDis) {
            auto safeSpd = calBestSpdFromDistance(endTurn.speed, theReadDis);
            cerr << "[WARNING] the end speed you set is not safe:" << pathPlanner::endSpd <<
                    " ,the plan will use the max safe speed: " << safeSpd << endl;
            pathPlanner::endSpd = safeSpd;
        }
    }

    /**
     * cal the speed type of erery line progress
     */
    lineTypes.front() = calLineType(pathPlanner::beginSpd, turnPoints.front().speed,
            turnPoints.front().lenBefore - turnPoints.front().d);

    for (uint64_t index_line = 1; index_line < lineTypes.size() - 1; ++index_line) {
        auto & beforeTurn = turnPoints[index_line - 1];
        auto & afterTurn = turnPoints[index_line];
        lineTypes[index_line] = calLineType(beforeTurn.speed, afterTurn.speed,
                beforeTurn.lenAfter - beforeTurn.d - afterTurn.d);
    }

    lineTypes.back() = calLineType(turnPoints.back().speed, pathPlanner::endSpd,
            turnPoints.back().lenAfter - turnPoints.back().d);

    /**
     * plan the speed according to the ts
     */
    for (uint32_t i = 0; i < turnPoints.size(); ++i) {
        assignLinePartPoints(i);
        assignTurnPartPoints(turnPoints[i], true);
        assignTurnPartPoints(turnPoints[i], false);
    }
    assignLinePartPoints(lineTypes.size() - 1);

    return answerCurve;
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
    IloRangeArray constrictions(env);
    /**
     * the optimize target  (-sum(d) - n*maxcurvature)  minnest
     */
    IloObjective optimizeTarget = IloMinimize(env);

    /// adding the d varibles
    for (int i = 0; i < cornerCount; i++) {
        double minLen = min(turnPoints[i].lenAfter, turnPoints[i].lenBefore);
        /// adding 0.45 for some speed change is available in some situations
        d.add(IloNumVar(env, 0.0, minLen * 0.45));
    }
    /// this is the max curvature
    d.add(IloNumVar(env, 0.0, +IloInfinity));

    for (int i = 0; i < cornerCount; i++) {
        /// the curvature of every corner should be smaller than the max curvature
        constrictions.add(IloRange(env, -IloInfinity, 0.0));
        double coeff_d2Kmax = turnPoints[i].coeff_d2Rmin;
        constrictions[i].setLinearCoef(d[i], -coeff_d2Kmax);
        constrictions[i].setLinearCoef(d[cornerCount], 1);

        /// the optimization target of Ds
        optimizeTarget.setLinearCoef(d[i], -coeff_d2Kmax);
    }
    /// the maxcurvature with coeff of n-1
    optimizeTarget.setLinearCoef(d[cornerCount], - cornerCount + 1);

    double coeff_beta2d = c4 * maxErr;
    for (int i = 0; i < cornerCount; i++) {
        /// d should not be too big to bigger than the global max error
        constrictions.add(IloRange(env, -IloInfinity, coeff_beta2d / cos(turnPoints[i].beta)));
        constrictions[cornerCount + i].setLinearCoef(d[i], 1);
    }

    /// d0 + d1 <= len(p1p2), might be useless with 0.45 coeff
    for (int i = 0; i < cornerCount - 1; i++) {
        constrictions.add(IloRange(env, -IloInfinity , turnPoints[i].lenAfter));
        constrictions[cornerCount * 2 + i].setLinearCoef(d[i], 1);
        constrictions[cornerCount * 2 + i].setLinearCoef(d[i + 1], 1);
    }

    /// d0 < len(p0p1) / 2 (leave the space for speed up
    constrictions.add(IloRange(env, -IloInfinity , turnPoints.front().lenBefore / 2.0));
    constrictions[cornerCount * 3 - 1].setLinearCoef(d[0], 1);

    /// dn < len(pn pn+1) / 2
    constrictions.add(IloRange(env, -IloInfinity , turnPoints.back().lenAfter / 2.0));
    constrictions[cornerCount * 3].setLinearCoef(d[cornerCount - 1], 1);

    model.add(optimizeTarget);
    model.add(constrictions);

    IloCplex cplex(model);

    cplex.solve();

    IloNumArray ds(env);
    cplex.getValues(ds, d);

//    env.out() << "Solution status = " << cplex.getStatus() << endl;
//    env.out() << "Solution value  = " << cplex.getObjValue() << endl;
    env.out() << "Values        = " << ds << endl;

    /// load the answers(d)
    for (int i = 0; i < ds.getSize() - 1; i++) {
        auto & tp = turnPoints[i];
        tp.setD(static_cast<double>(ds[i]), spdMax, accMax);
        /// use precise len may speed up the progress. but not tested
//        tp.calPreciseHalfLen();
        curveLength += 2 * (tp.halfLength - tp.d);
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

double HGJ::pathPlanner::calChangeSpdDistance(double v0, double v1) {
    return (v0 + v1) / 2.0 * calChangeSpdDuration(v0 - v1);
}

/**
 * represent the maxJerk when speed change form v0 to v1
 * f(v1) = v1^3 + v0v1^2 - v0^2v1 - v0^3
 * @return f(v1)
 */
inline double calFunc(double v0, double v1) {
    return (v1 + v0) * (v1 - v0) * (v1 + v0);
}

/**
 * the gradient of the previous function
 * f(v1) = v1^3 + v0v1^2 - v0^2v1 - v0^3
 * f'(v1) = 3v1^2 + 2v0v1 - v0^2
 * @return f'(v1)
 */
inline double calSlope(double v0, double v1) {
    return (v1 + v0) * (3.0 * v1 - v0);
}

double HGJ::pathPlanner::calBestSpdFromDistance(double v0, double dist, bool faster) {

    if (!faster) {
        cout << "[calBestSpdFromDistance] slower example" << endl;
    }

    if (abs(dist) < 0.01) {
        return v0;
    }

    /// cal the vel from the accMax constract
    double tempSqrtDiff = 1.06666666666 * accMax * dist;
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

    /// cal the vel from the jerkMax constract
    /// solve the qubic function using gradient descent
    /// targetY represent the speed related to the JerkMax
    double targetY = 0.69282 * dist * dist * jerkMax;
    if (!faster) {
        targetY = - targetY;
        /// the function cal the min jerk, there is a local min at v0/3
        double minJerk = calFunc(v0, v0 / 3.0);
        /// if the min is small enough, the constraction is from the accMax, return the v1Acc
        if (minJerk < targetY) {
            return v1Acc;
        }
    }

    /// starting at v0, the gradient descent progress may be not fast
    double v1Jerk = v0;
    double currentY = 0;
    double threshold = targetY * 1e-5;

    while (abs(currentY - targetY) > threshold) {
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

    /// the min lenth means the speed change is only speed up or speed down from v0 to v1
    double minDist = calChangeSpdDistance(v0, v1);
    if (dist <= minDist * 1.025) {
        return make_pair(ONEACC, max(v0, v1));
    }

    /// the min dist needed if we want to speed up to spdMax
    double minDist2SpdMax = calLineSpdUpDownMinDistance(v0, v1, spdMax);
    double distDiff = minDist2SpdMax - dist;

    /**
     * if maxDist < dist, means we can speed up to the max speed in the line
     */
    if (distDiff < 0) {
        return make_pair(TOMAXSPD, spdMax);
    }

    /// if not, using dichotomy to find the max speed we can reach
    double upperBound = spdMax;
    double lowerBound = max(v0, v1);
    double midSpd = (upperBound + lowerBound) / 2;
    double tollerance = dist / 1000.0;

    while (true) {
        distDiff = calLineSpdUpDownMinDistance(v0, v1, midSpd) - dist;
        /**
         * CaledDist > realDist  ==>  spd need to be slower
         */
        if (distDiff > tollerance) {
            upperBound = midSpd;
            midSpd = (midSpd + lowerBound) / 2.0;
        }
        /**
         * CaledDist > realDist  ==>  spd can be higher
         */
        else if(distDiff < 0) {
            lowerBound = midSpd;
            midSpd = (midSpd + upperBound) / 2.0;
        } else {
            break;
        }
    }

    return make_pair(SPDUPDOWN, midSpd);
}

double HGJ::pathPlanner::calLineSpdUpDownMinDistance(double v0, double v1, double vMid) {
    auto dist = calChangeSpdDistance(v0, vMid);
    dist += calChangeSpdDistance(vMid, v1);
    return dist;
}

double HGJ::pathPlanner::calChangeSpdPosition(double v0, double dv, double t, double sumT) {
    double tRatio = t / sumT;
    double ans = 2.5 - (3 - tRatio) * tRatio;
    ans = ans * tRatio * tRatio * tRatio * dv + v0;
    return ans * t;
}

void HGJ::pathPlanner::assignSpdChangePoints(double beginV, double endV,
                                             const vec3f & posBegin, const vec3f & posEnd) {
    auto unitVec = posEnd - posBegin;

    /// means there is noting to assign
    if (unitVec.len() == 0.0) {
        return;
    }

    unitVec /= unitVec.len();
    double dv = endV - beginV;
    double sumT = calChangeSpdDuration(dv);

    /// at the begin of the whole progress, the progress t_init will be NAN
    double t = ts - resDis / beginV;
    if (beginV == 0.0) {
        t = 0.0;
    }

    double distProgress;

    /// build the points
    while (t < sumT) {
        distProgress = calChangeSpdPosition(beginV, dv, t, sumT);
        answerCurve.emplace_back(posBegin + unitVec * distProgress);
        t += ts;
    }

    if (!answerCurve.empty()) {
        resDis = (answerCurve.back() - posEnd).len();
    }

    if (resDis < 0) {
        cerr << "[pathPlanner::calChangeSpdPosition] reDis:" << resDis
             <<" is smaller than 0!" << endl;
    }
}

void HGJ::pathPlanner::assignLinearConstantSpeedPoints(double spd, const vec3f & posBegin,
                                                       const vec3f & posEnd) {
    auto unitVec = posEnd - posBegin;
    if (unitVec.len() == 0.0) {
        return;
    }
    double sumT = unitVec.len() / spd;
    unitVec /= unitVec.len();
    double t = ts - resDis / spd;

    while (t < sumT) {
        answerCurve.emplace_back(t * spd * unitVec + posBegin);
        t += ts;
    }

    if (!answerCurve.empty()) {
        resDis = (answerCurve.back() - posEnd).len();
    }

    if (resDis < 0) {
        cerr << "[pathPlanner::assignLinearConstantSpeedPoints] reDis:" << resDis
             <<" is smaller than ""0!" << endl;
    }
}

void HGJ::pathPlanner::assignTurnPartPoints(const TurnPoint & turnPoint, bool firstPart) {

    /// at the corner, the speed is constrant, so the point gap is constrant
    double ds = ts * turnPoint.speed;

    if (ds < resDis) {
        cerr << "[pathPlanner::assignTurnPartPoints] the first Inc is < 0 !\nfirst part:"
             << firstPart << endl;
        resDis = ds;
    }

    /// the dU between the gap is about ds/wholeLen
    double increaseU = ds / turnPoint.halfLength;
    double firstU = (ds - resDis) / turnPoint.halfLength;

    double lastU = 0.0;
    vec3f lastPoint;
    if (firstPart) {
        lastPoint = turnPoint.B1[0];
    } else {
        lastPoint = turnPoint.B2[3];
    }

    /// considering the res error, the first currentU, targetLen is different
    double currentU = firstU;
    vec3f currentPoint = turnPoint.calPoint(currentU, firstPart);
    double targetLen = ds - resDis;
    double lenNow = (currentPoint - lastPoint).len();
    double error = targetLen - lenNow;

    double tolleranceError = ds / 20000.0;

    while (true) {
        while (abs(error) > tolleranceError) {
            /// we need to modify the current U
            /// de/du = -len(C0C1) / delta(U)
            double dedu = lenNow / (currentU - lastU);
            /// newU = lastU + error / (de/du)
            currentU = currentU + error / dedu;

            /// after climbing it is still over 1? the curve is finished
            if (currentU > 1.0) {
                if (firstPart) {
                    resDis = (answerCurve.back() - turnPoint.B1[3]).len();
                } else {
                    resDis = (answerCurve.back() - turnPoint.B2[0]).len();
                }
                return;
            }

            currentPoint = turnPoint.calPoint(currentU, firstPart);
            lenNow = (currentPoint - lastPoint).len();
            error = targetLen - lenNow;
        }

        /// come to here, we reslove a U that is accurate to record
        lastPoint = currentPoint;
        answerCurve.emplace_back(currentPoint);
        targetLen = ds;
        lastU = currentU;
        currentU = lastU + increaseU;
        /// if U is bigger than 1.0, we set it to 1.0,
        /// if it can go smaller than 1, it is a good point
        if (currentU > 1.0) {
            currentU = 1.0;
        }
        currentPoint = turnPoint.calPoint(currentU, firstPart);
        lenNow = (currentPoint - lastPoint).len();
        error = targetLen - lenNow;
    }
}

void HGJ::pathPlanner::assignLinePartPoints(uint64_t index) {
    vec3f beginPos, endPos;
    double beginSpd, endSpd;

    /// at the start point and the end point, the spd and pos are special
    if (index == 0) {
        beginPos = turnPoints.front().p0;
        beginSpd = pathPlanner::beginSpd;
    } else {
        beginPos = turnPoints[index - 1].B2[0];
        beginSpd = turnPoints[index - 1].speed;
    }

    if (index == lineTypes.size() - 1) {
        endPos = turnPoints.back().p2;
        endSpd = pathPlanner::endSpd;
    } else {
        endPos = turnPoints[index].B1[0];
        endSpd = turnPoints[index].speed;
    }

    auto lineType = lineTypes[index].first;
    auto lineMaxSpd = lineTypes[index].second;

    if (lineType == ONEACC) {
        assignSpdChangePoints(beginSpd, endSpd, beginPos, endPos);
        return;

    } else if (lineType == SPDUPDOWN || lineType == TOMAXSPD) {
        vec3f unitVec = (endPos - beginPos) / (endPos - beginPos).len();

        auto firstMaxSpdDis = calChangeSpdDistance(beginSpd, lineMaxSpd);
        auto firstMaxSpdPos = (unitVec * firstMaxSpdDis) + beginPos;

        auto endSpdChangeDis = calChangeSpdDistance(lineMaxSpd, endSpd);
        auto endMaxSpdPos = endPos - unitVec * endSpdChangeDis;

        assignSpdChangePoints(beginSpd, lineMaxSpd, beginPos, firstMaxSpdPos);

        if (lineType == TOMAXSPD) {
            assignLinearConstantSpeedPoints(lineMaxSpd, firstMaxSpdPos, endMaxSpdPos);
            assignSpdChangePoints(lineMaxSpd, endSpd, endMaxSpdPos, endPos);
        } else {
            assignSpdChangePoints(lineMaxSpd, endSpd, firstMaxSpdPos, endPos);
        }
        return;
    } else if (lineType == UNSET) {
        cerr << "assign a line with UNSET type!" << endl;
        throw;
    } else {
        cerr << "assign a line with UNKNOW type!" << endl;
        throw;
    }
}

void HGJ::pathPlanner::setMaxJerk(double jerkMax) {
    if (jerkMax > 0) {
        pathPlanner::jerkMax = jerkMax;
    }
}

void HGJ::pathPlanner::setMaxAcc(double accMax) {
    if (accMax > 0) {
        pathPlanner::accMax = accMax;
    }
}

void HGJ::pathPlanner::setMaxSpeed(double spdMax) {
    if (spdMax > 0) {
        pathPlanner::spdMax = spdMax;
    }
}

void HGJ::pathPlanner::setMaxErr(double maxErr) {
    if (maxErr > 0) {
        pathPlanner::maxErr = maxErr;
    }
}

const HGJ::WayPoints & HGJ::pathPlanner::getLastGeneratedCurve() {
    return answerCurve;
}



