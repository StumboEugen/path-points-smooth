//
// Created by stumbo on 18-11-5.
//

#include <ilcplex/ilocplex.h>

#include "pathSmoother.h"
#include "tools.h"
#include "TurnPoint.h"
#include <iostream>

using namespace std;

HGJ::pathPlanner::pathPlanner() {
    cout << c1 << endl;
}

HGJ::WayPoints HGJ::pathPlanner::genCurv(const WayPoints & wayPoints,
                                         const double & ts,
                                         const double & maxErr) {
    size_t pointCount = wayPoints.size();
    size_t cornerCount = pointCount - 2;
    if (pointCount <= 1) {
        return wayPoints;
    }

    if (pointCount == 2) {
        return wayPoints;
    }

    vector<TurnPoint> bezierCorners;
    bezierCorners.reserve(cornerCount);
    for (int i = 0; i < cornerCount; i++) {
        bezierCorners.emplace_back(wayPoints[i], wayPoints[i + 1], wayPoints[i + 2]);
    }


    /**
     * start linear programing
     * using library IBM CPLEX
     */
    IloEnv env;

    IloModel model(env);
    IloNumVarArray d(env);
    IloRangeArray c(env);
    IloObjective obj = IloMinimize(env);

    // adding the d varibles
    for (int i = 0; i < cornerCount; i++) {
        // d[0] ~ d[cc - 1]
        d.add(IloNumVar(env, 0.0, +IloInfinity));
    }
    //max curvature
    d.add(IloNumVar(env, 0.0, +IloInfinity));

    for (int i = 0; i < cornerCount; i++) {

        // the curvature of every corner should be smaller than the max curvature

        c.add(IloRange(env, 0.0, +IloInfinity));
        double coeff_d2Kmax = bezierCorners[i].coeff_d2Kmax();
        c[i].setLinearCoef(d[i], -coeff_d2Kmax);
        c[i].setLinearCoef(d[cornerCount], 1);

        // the optimization target

        obj.setLinearCoef(d[i], -coeff_d2Kmax);
    }
    obj.setLinearCoef(d[cornerCount], - cornerCount + 1);

    double coeff_beta2d = c4 * maxErr;
    for (int i = 0; i < cornerCount; i++) {
        // d should not be too big to overflow the max error

        c.add(IloRange(env, coeff_beta2d / cos(bezierCorners[i].beta) , +IloInfinity));
        c[cornerCount + i].setLinearCoef(d[i], 1);
    }

    for (int i = 0; i < cornerCount - 1; i++) {
        // d0 + d1 <= len(p1p2)

        c.add(IloRange(env, -IloInfinity , bezierCorners[i].len2));
        c[cornerCount * 2 + i].setLinearCoef(d[i], 1);
        c[cornerCount * 2 + i].setLinearCoef(d[i + 1], 1);
    }

    // d0 < len(p0p1)
    c.add(IloRange(env, -IloInfinity , bezierCorners.front().len1));
    c[cornerCount * 3 - 1].setLinearCoef(d[0], 1);

    // dn < len(pn pn+1)
    c.add(IloRange(env, -IloInfinity , bezierCorners.back().len2));
    c[cornerCount * 3 - 1].setLinearCoef(d[cornerCount - 1], 1);

    model.add(obj);
    model.add(c);

    IloCplex cplex(model);

    cplex.solve();

    IloNumArray ds(env);
    IloNumArray vals(env);
    env.out() << "Solution status = " << cplex.getStatus() << endl;
    env.out() << "Solution value  = " << cplex.getObjValue() << endl;
    cplex.getValues(ds, d);
    env.out() << "Values        = " << ds << endl;

    env.end();

    WayPoints ans;
    for (int i = 0; i < ds.getSize(); i++) {
        ans.emplace_back(static_cast<double>(ds[i]), 0.0, 0.0);
    }
    return ans;
}

void HGJ::pathPlanner::setMaxSpeed(double maxSpeed) {
    pathPlanner::maxSpeed = maxSpeed;
}
