//
// Created by stumbo on 18-11-5.
//

#include <ilcplex/ilocplex.h>

#include "pathSmoother.h"
#include "tools.h"
#include "TurnPoint.h"
#include <iostream>

using namespace std;

HGJ::pathPlanner::pathPlanner() = default;

HGJ::WayPoints HGJ::pathPlanner::genCurv(const WayPoints & wayPoints,
                                         const double & ts,
                                         const double & maxErr) {
    pathPlanner::maxErr = maxErr;
    pathPlanner::ts = ts;

    int64_t pointCount = wayPoints.size();
    int64_t cornerCount = pointCount - 2;
    if (pointCount <= 1) {
        return wayPoints;
    }

    if (pointCount == 2) {
        //TODO smooth
        return wayPoints;
    }

    vector<TurnPoint> bezierCorners;
    bezierCorners.reserve(static_cast<unsigned long>(cornerCount));
    for (int i = 0; i < cornerCount; i++) {
        bezierCorners.emplace_back(wayPoints[i], wayPoints[i + 1], wayPoints[i + 2]);
    }

    auto ds = lpSolveTheDs(bezierCorners);

    WayPoints ans;
    for (auto & d: ds) {
        ans.emplace_back((d,0,0));
    }

    return ans;
}

void HGJ::pathPlanner::setMaxSpeed(double maxSpeed) {
    pathPlanner::maxSpeed = maxSpeed;
}

std::vector<double> HGJ::pathPlanner::lpSolveTheDs(const vector<HGJ::TurnPoint> & turnPoints) {
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
        double coeff_d2Kmax = turnPoints[i].coeff_d2Kmax();
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
        c.add(IloRange(env, -IloInfinity , turnPoints[i].len2));
        c[cornerCount * 2 + i].setLinearCoef(d[i], 1);
        c[cornerCount * 2 + i].setLinearCoef(d[i + 1], 1);
    }

    // d0 < len(p0p1)
    c.add(IloRange(env, -IloInfinity , turnPoints.front().len1));
    c[cornerCount * 3 - 1].setLinearCoef(d[0], 1);

    // dn < len(pn pn+1)
    c.add(IloRange(env, -IloInfinity , turnPoints.back().len2));
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

    vector<double> ansD;
    ansD.reserve(static_cast<unsigned long>(ds.getSize()));
    for (int i = 0; i < ds.getSize(); i++) {
        ansD.emplace_back(static_cast<double>(ds[i]));
    }

    env.end();

    return std::move(ansD);
}
