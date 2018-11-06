//
// Created by stumbo on 18-11-5.
//

#include "pathSmoother.h"
#include "tools.h"
#include "TurnPoint.h"
#include <iostream>

using namespace std;

HGJ::pathPlanner::pathPlanner() {
    cout << c1 << endl;
}

HGJ::WayPoints HGJ::pathPlanner::genCurv(HGJ::WayPoints wayPoints,
                                         const double ts,
                                         const double maxErr) {
    size_t pointCount = wayPoints.size();

    return HGJ::WayPoints();
}
