#include <iostream>
#include <ilcplex/ilocplex.h>
#include "HGJPathSmooth/pathSmoother.h"

using namespace std;

int main() {

    HGJ::pathPlanner planner;
    HGJ::WayPoints wayPoints;
    wayPoints.push_back({0,0,0});
    wayPoints.push_back({0.5,0,0});
    wayPoints.push_back({0.5,-1,0});
    wayPoints.push_back({-0.5,-1,0});
    wayPoints.push_back({-0.5,-1,0});
    wayPoints.push_back({0,0,0});
    planner.genCurv({{0,0,0},{0.5,0,0},{0.5,-1,0},{-0.5,-1,0},{-0.5,0,0},{0,0,0}}, 0.01, 0.08);
    planner.genCurv({{0,0,0},{0.5,0,0},{0.5,-1,0},{-0.5,-1,0},{-0.5,0,0},{0,0,0}}, 0.01, 0.08);
//    planner.genCurv({{0,0,0},{0.5,0,0},{0.5,-1,0},{-0.5,-1,0},{-0.5,0,0},{0,0,0}}, 0.01, 0.08);
//    planner.genCurv({{0,0,0},{0.5,0,0},{0.5,-1,0},{-0.5,-1,0},{-0.5,0,0},{0,0,0}}, 0.01, 0.08);
//    planner.genCurv({{0,0,0},{0.5,0,0},{0.5,-1,0},{-0.5,-1,0},{-0.5,0,0},{0,0,0}}, 0.01, 0.08);
//    planner.genCurv({{0,0,0},{0.5,0,0},{0.5,-1,0},{-0.5,-1,0},{-0.5,0,0},{0,0,0}}, 0.01, 0.08);
//    planner.genCurv({{0,0,0},{0.5,0,0},{0.5,-1,0},{-0.5,-1,0},{-0.5,0,0},{0,0,0}}, 0.01, 0.08);
}